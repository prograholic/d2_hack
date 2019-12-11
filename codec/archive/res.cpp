#include <d2_hack/codec/archive/res.h>

#include <memory>
#include <fstream>

#include <OgreException.h>

#include <d2_hack/common/offset_data_stream.h>

#include "res_file_info.h"

namespace d2_hack
{
namespace codec
{
namespace archive
{

ResArchive::ResArchive(const Ogre::String& name, const Ogre::String& archType)
    : Ogre::Archive(name, archType)
    , m_archiveInfo(std::make_unique<ResFileInfo>())
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

    ReadFileInfo(archiveFile, *m_archiveInfo);

    for (auto it = m_archiveInfo->info.begin(); it != m_archiveInfo->info.end(); ++it)
    {
        Ogre::FileInfo info;

        info.archive = this;
        info.filename = it->first;
        Ogre::StringUtil::splitFilename(it->first, info.basename, info.path);
        info.compressedSize = it->second.size;
        info.uncompressedSize = it->second.size;

        m_fileInfoList.push_back(info);
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
    if (*stdStream)
    {
        Ogre::DataStreamPtr fileStream(new Ogre::FileStreamDataStream(stdStream));

        if (findEntry(filename, entry))
        {
            Ogre::DataStreamPtr result(new common::OffsetDataStream(fileStream, entry.offset, entry.size));

            return result;
        }
        OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS, "file [" + filename + "] canot be found in archive [" + mName + "]");
    }
    OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS, "Failed to open stream [" + mName + "]");
}

Ogre::StringVectorPtr ResArchive::list(bool recursive, bool dirs) const
{
    Ogre::StringVectorPtr ret = Ogre::StringVectorPtr(OGRE_NEW_T(Ogre::StringVector, Ogre::MEMCATEGORY_GENERAL)(), Ogre::SPFM_DELETE_T);

    Ogre::FileInfoList::const_iterator i, iend;
    iend = m_fileInfoList.end();
    for (i = m_fileInfoList.begin(); i != iend; ++i)
    {
        if ((dirs == (i->compressedSize == size_t (-1))) && (recursive || i->path.empty()))
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

    return Ogre::FileInfoListPtr(fil, Ogre::SPFM_DELETE_T);
}

Ogre::StringVectorPtr ResArchive::find(const Ogre::String& pattern, bool recursive, bool dirs) const
{
    Ogre::StringVectorPtr ret =
        Ogre::StringVectorPtr(OGRE_NEW_T(Ogre::StringVector, Ogre::MEMCATEGORY_GENERAL)(), Ogre::SPFM_DELETE_T);
    // If pattern contains a directory name, do a full match
    bool full_match = (pattern.find ('/') != Ogre::String::npos) || (pattern.find ('\\') != Ogre::String::npos);
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
    ResEntry entry;

    return findEntry(filename, entry);
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
    Ogre::FileInfoListPtr ret = Ogre::FileInfoListPtr(OGRE_NEW_T(Ogre::FileInfoList, Ogre::MEMCATEGORY_GENERAL)(), Ogre::SPFM_DELETE_T);
    // If pattern contains a directory name, do a full match
    bool full_match = (pattern.find ('/') != Ogre::String::npos) || (pattern.find ('\\') != Ogre::String::npos);
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



bool ResArchive::findEntry(const Ogre::String & filename, ResEntry & entry) const
{
    auto pos = m_archiveInfo->info.find(filename);
    if (pos != m_archiveInfo->info.end())
    {
        entry = pos->second;
        return true;
    }

    return false;
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

} //namespace archive
} // namespace codec
} // namespace d2_hack  
