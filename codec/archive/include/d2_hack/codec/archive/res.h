#ifndef D2_HACK_CODEC_ARCHIVE_INCLUDE_D2_HACK_CODEC_ARCHIVE_RES_H
#define D2_HACK_CODEC_ARCHIVE_INCLUDE_D2_HACK_CODEC_ARCHIVE_RES_H

#include <d2_hack/common/platform.h>

#include <memory>

#include <OgreArchive.h>
#include <OgreArchiveFactory.h>

namespace d2_hack
{
namespace codec
{
namespace archive
{

struct ResEntry;
struct ResFileInfo;

class ResArchive: public Ogre::Archive
{
public:
    ResArchive(const Ogre::String& name, const Ogre::String& archType);
    ~ResArchive();

    virtual bool isCaseSensitive(void) const;

    virtual void load();

    virtual void unload();

    virtual Ogre::DataStreamPtr open(const Ogre::String& filename, bool readOnly = true) const;

    virtual Ogre::StringVectorPtr list(bool recursive = true, bool dirs = false) const;

    virtual Ogre::FileInfoListPtr listFileInfo(bool recursive = true, bool dirs = false) const;

    virtual Ogre::StringVectorPtr find(const Ogre::String& pattern, bool recursive = true, bool dirs = false) const;

    virtual bool exists(const Ogre::String& filename) const;

    virtual time_t getModifiedTime(const Ogre::String& filename) const;

    virtual Ogre::FileInfoListPtr findFileInfo(const Ogre::String& pattern, bool recursive = true, bool dirs = false) const;
    
    class Factory: public Ogre::ArchiveFactory
    {
    public:
        virtual const Ogre::String& getType() const;

        virtual Ogre::Archive* createInstance(const Ogre::String& name, bool bReadOnly);

        virtual void destroyInstance(Ogre::Archive* archive);

    };

private:
    std::unique_ptr<ResFileInfo> m_archiveInfo;
    Ogre::FileInfoList m_fileInfoList;

    bool findEntry(const Ogre::String & filename, ResEntry& entry) const;

};




} //namespace archive
} // namespace codec
} // namespace d2_hack  

#endif // D2_HACK_CODEC_ARCHIVE_INCLUDE_D2_HACK_CODEC_ARCHIVE_RES_H
