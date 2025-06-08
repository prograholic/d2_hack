#include <d2_hack/resource/archive/res_archive.h>

#include <fstream>

#include <OgreException.h>

#include <d2_hack/resource/archive/res_extensions.h>
#include <d2_hack/common/offset_data_stream.h>

#include "res_file_info.h"
#include "ogre_material_generator.h"

namespace d2_hack
{
namespace resource
{
namespace archive
{
namespace res
{

static std::string GetResId(const std::string& fileName)
{
    std::string baseName;
    std::string extension;
    std::string path;
    Ogre::StringUtil::splitFullFilename(fileName, baseName, extension, path);

    return baseName;
}

ResArchive::ResArchive(const Ogre::String& name, const Ogre::String& archType)
    : Ogre::Archive(name, archType)
    , m_archiveInfo(std::make_unique<ResFileInfo>())
    , m_resId(GetResId(name))
{
}

ResArchive::~ResArchive() = default;

bool ResArchive::isCaseSensitive(void) const
{
    return true;
}

void ResArchive::load()
{
    std::ifstream archiveFile(mName.c_str(), std::ios_base::binary);
    if (!archiveFile)
    {
        OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS, "Failed to open file [" + mName + "]");
    }

    Ogre::FileStreamDataStream dataStream(&archiveFile, false);

    ReadFileInfo(m_resId, dataStream, *m_archiveInfo);

    for (const auto& resEntry : m_archiveInfo->info)
    {
        Ogre::FileInfo info;

        info.archive = this;
        info.filename = resEntry.name;
        Ogre::StringUtil::splitFilename(resEntry.name, info.basename, info.path);
        info.compressedSize = resEntry.size;
        info.uncompressedSize = resEntry.size;

        m_fileInfoList.push_back(info);

        if (info.filename.ends_with(extensions::MaterialExt))
        {
            // add info for generated Ogre material
            info.filename = GetPublicFilename(info.filename);
            info.basename = GetPublicFilename(info.basename);
            m_fileInfoList.push_back(info);
        }
    }
}

void ResArchive::unload()
{
    m_archiveInfo->info.clear();
    m_fileInfoList.clear();
}


Ogre::DataStreamPtr ResArchive::open(const Ogre::String& filename, bool /* readOnly */) const
{
    ResEntry entry;

    std::ifstream* stdStream = OGRE_NEW_T(std::ifstream, Ogre::MEMCATEGORY_GENERAL)(mName.c_str(), std::ios_base::binary);
    if (!*stdStream)
    {
        OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS, "Failed to open stream [" + mName + "]");
    }
    
    bool isOgreMaterial = false;
    std::string underlyingFileName = GetInternalFileName(filename, isOgreMaterial);
    if (!FindResEntry(underlyingFileName, entry))
    {
        OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS, "file [" + filename + "] cannot be found in archive [" + mName + "]");
    }

    Ogre::DataStreamPtr fileStream(new Ogre::FileStreamDataStream(stdStream));
    Ogre::DataStreamPtr streamForFile(new common::OffsetDataStream(fileStream, entry.offset, entry.size));

    if (isOgreMaterial)
    {
        return GenerateMaterial(*m_archiveInfo, filename, streamForFile);
    }


    return streamForFile;
}

Ogre::StringVectorPtr ResArchive::list(bool recursive, bool dirs) const
{
    Ogre::StringVectorPtr ret = Ogre::StringVectorPtr{new Ogre::StringVector{}};

    Ogre::FileInfoList::const_iterator i, iend;
    iend = m_fileInfoList.end();
    for (i = m_fileInfoList.begin(); i != iend; ++i)
    {
        if ((dirs == (i->compressedSize == size_t(-1))) && (recursive || i->path.empty()))
        {
            ret->push_back(i->filename);
        }
    }

    return ret;
}

Ogre::FileInfoListPtr ResArchive::listFileInfo(bool recursive, bool dirs) const
{
    Ogre::FileInfoList* fil = OGRE_NEW_T(Ogre::FileInfoList, Ogre::MEMCATEGORY_GENERAL)();
    Ogre::FileInfoList::const_iterator i, iend;
    iend = m_fileInfoList.end();
    for (i = m_fileInfoList.begin(); i != iend; ++i)
    {
        if ((dirs == (i->compressedSize == size_t(-1))) &&
            (recursive || i->path.empty()))
        {
            fil->push_back(*i);
        }
    }

    return Ogre::FileInfoListPtr{fil};
}

Ogre::StringVectorPtr ResArchive::find(const Ogre::String& pattern, bool recursive, bool dirs) const
{
    Ogre::StringVectorPtr ret = Ogre::StringVectorPtr{new Ogre::StringVector{}};
    // If pattern contains a directory name, do a full match
    bool full_match = (pattern.find('/') != Ogre::String::npos) || (pattern.find('\\') != Ogre::String::npos);
    bool wildCard = pattern.find('*') != Ogre::String::npos;

    Ogre::FileInfoList::const_iterator i, iend;
    iend = m_fileInfoList.end();
    for (i = m_fileInfoList.begin(); i != iend; ++i)
    {
        if ((dirs == (i->compressedSize == size_t(-1))) &&
            (recursive || full_match || wildCard))
            // Check basename matches pattern (zip is case insensitive)
        {
            if (Ogre::StringUtil::match(full_match ? i->filename : i->basename, pattern, false))
            {
                ret->push_back(i->filename);
            }
        }
    }

    return ret;
}

bool ResArchive::exists(const Ogre::String& filename) const
{
    for (const auto& fileInfo : m_fileInfoList)
    {
        if (fileInfo.filename == filename)
        {
            return true;
        }
    }

    return false;
}

time_t ResArchive::getModifiedTime(const Ogre::String& /* filename */) const
{
    struct stat tagStat;
    bool ret = (stat(mName.c_str(), &tagStat) == 0);

    if (ret)
    {
        return tagStat.st_mtime;
    }
    else
    {
        return 0;
    }
}

Ogre::FileInfoListPtr ResArchive::findFileInfo(const Ogre::String& pattern, bool recursive, bool dirs) const
{
    Ogre::FileInfoListPtr ret = Ogre::FileInfoListPtr{new Ogre::FileInfoList{}};
    // If pattern contains a directory name, do a full match
    bool full_match = (pattern.find('/') != Ogre::String::npos) || (pattern.find('\\') != Ogre::String::npos);
    bool wildCard = pattern.find('*') != Ogre::String::npos;

    Ogre::FileInfoList::const_iterator i, iend;
    iend = m_fileInfoList.end();
    for (i = m_fileInfoList.begin(); i != iend; ++i)
    {
        if ((dirs == (i->compressedSize == size_t(-1))) && (recursive || full_match || wildCard))
        {
            // Check name matches pattern (zip is case insensitive)
            if (Ogre::StringUtil::match(full_match ? i->filename : i->basename, pattern, false))
            {
                ret->push_back(*i);
            }
        }
    }

    return ret;
}



bool ResArchive::FindResEntry(const Ogre::String& filename, ResEntry& entry) const
{
    for (const auto& e : m_archiveInfo->info)
    {
        if (e.name == filename)
        {
            entry = e;
            return true;
        }
    }

    return false;
}

std::string ResArchive::GetInternalFileName(const std::string& filename, bool& isOgreMaterial)
{
    isOgreMaterial = false;
    if (filename.ends_with(".material"))
    {
        isOgreMaterial = true;
        return filename.substr(0, filename.size() - strlen(".material")) + extensions::MaterialExt;
    }

    return filename;
}
std::string ResArchive::GetPublicFilename(const std::string& filename)
{
    if (filename.ends_with(extensions::MaterialExt))
    {
        return filename.substr(0, filename.size() - strlen(extensions::MaterialExt)) + ".material";
    }

    return filename;
}


/////////////////////////////////////////////////////////////////////////////////////////

namespace
{
Ogre::String ResourceType = "D2Res";
}

const Ogre::String& ResArchive::Factory::getType() const
{
    return ResourceType;
}

Ogre::Archive* ResArchive::Factory::createInstance(const Ogre::String& name, bool /* bReadOnly */)
{
    return new ResArchive(name, ResourceType);
}

void ResArchive::Factory::destroyInstance(Ogre::Archive* archive)
{
    delete archive;
}

} // namespace res
} //namespace archive
} // namespace resource
} // namespace d2_hack  
