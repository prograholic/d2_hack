#include "d2_res_archive.h"

#include <fstream>

#include "offset_data_stream.h"
#include "d2_hack_common.h"




D2ResArchive::D2ResArchive(const Ogre::String& name, const Ogre::String& archType)
    : Ogre::Archive(name, archType)
    , m_archiveInfo()
{
}


bool D2ResArchive::isCaseSensitive(void) const
{
    return true;
}

void D2ResArchive::load()
{
    std::ifstream archiveFile(mName.c_str(), std::ios_base::binary);
    if (!archiveFile)
    {
        OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS,
                    "Failed to open file [" + mName + "]",
                    "D2ResArchive::load");
    }

    readFileInfo(archiveFile, m_archiveInfo);



    for (D2ResFileInfo::const_iterator it = m_archiveInfo.begin(); it != m_archiveInfo.end(); ++it)
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

void D2ResArchive::unload()
{
    m_archiveInfo.clear();
    m_fileInfoList.clear();
}


Ogre::DataStreamPtr D2ResArchive::open(const Ogre::String& filename, bool /* readOnly */) const
{
    D2ResEntry entry;

    std::ifstream* stdStream = OGRE_NEW_T(std::ifstream, Ogre::MEMCATEGORY_GENERAL)(mName.c_str(), std::ios_base::binary);
    if (*stdStream)
    {
        Ogre::DataStreamPtr fileStream(new Ogre::FileStreamDataStream(stdStream));

        if (findEntry(filename, entry))
        {
            Ogre::DataStreamPtr result(new OffsetDataStream(fileStream, entry.offset, entry.size));

            return result;
        }
        OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS,
                    "file [" + filename + "] canot be found in archive [" + mName + "]",
                    "D2ResArchive::open");
    }
    OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS,
                "Failed to open stream [" + mName + "]",
                "D2ResArchive::open");
}

Ogre::StringVectorPtr D2ResArchive::list(bool recursive, bool dirs) const
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

Ogre::FileInfoListPtr D2ResArchive::listFileInfo(bool recursive, bool dirs) const
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

Ogre::StringVectorPtr D2ResArchive::find(const Ogre::String& pattern, bool recursive, bool dirs) const
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

bool D2ResArchive::exists(const Ogre::String& filename) const
{
    D2ResEntry entry;

    return findEntry(filename, entry);
}

time_t D2ResArchive::getModifiedTime(const Ogre::String& /* filename */) const
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

Ogre::FileInfoListPtr D2ResArchive::findFileInfo(const Ogre::String& pattern, bool recursive, bool dirs) const
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



bool D2ResArchive::findEntry(const Ogre::String & filename, D2ResEntry & entry) const
{
    D2ResFileInfo::const_iterator pos = m_archiveInfo.find(filename);
    if (pos != m_archiveInfo.end())
    {
        entry = pos->second;
        return true;
    }

    return false;
}


/////////////////////////////////////////////////////////////////////////////////////////

namespace
{
    Ogre::String d2ResFactoryType = "D2Res";
}

const Ogre::String& D2ResArchiveFactory::getType() const
{
    return d2ResFactoryType;
}

Ogre::Archive* D2ResArchiveFactory::createInstance(const Ogre::String& name, bool /* bReadOnly */)
{
    return new D2ResArchive(name, d2ResFactoryType);
}

void D2ResArchiveFactory::destroyInstance(Ogre::Archive* archive)
{
    delete archive;
}
