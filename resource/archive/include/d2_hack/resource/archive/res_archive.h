#ifndef D2_HACK_RESOURCE_ARCHIVE_INCLUDE_D2_HACK_RESOURCE_ARCHIVE_RES_ARCHIVE_H
#define D2_HACK_RESOURCE_ARCHIVE_INCLUDE_D2_HACK_RESOURCE_ARCHIVE_RES_ARCHIVE_H

#include <d2_hack/common/platform.h>

#include <memory>

#include <OgreArchive.h>
#include <OgreArchiveFactory.h>

namespace d2_hack
{
namespace resource
{
namespace archive
{
namespace res
{

struct ResEntry;
struct ResFileInfo;

class ResArchive : public Ogre::Archive
{
public:
    ResArchive(const Ogre::String& name, const Ogre::String& archType);

    ~ResArchive() noexcept;

    virtual bool isCaseSensitive(void) const override;

    virtual void load() override;

    virtual void unload() override;

    virtual Ogre::DataStreamPtr open(const Ogre::String& filename, bool readOnly = true) const override;

    virtual Ogre::StringVectorPtr list(bool recursive = true, bool dirs = false) const override;

    virtual Ogre::FileInfoListPtr listFileInfo(bool recursive = true, bool dirs = false) const override;

    virtual Ogre::StringVectorPtr find(const Ogre::String& pattern, bool recursive = true, bool dirs = false) const override;

    virtual bool exists(const Ogre::String& filename) const override;

    virtual time_t getModifiedTime(const Ogre::String& filename) const override;

    virtual Ogre::FileInfoListPtr findFileInfo(const Ogre::String& pattern, bool recursive = true, bool dirs = false) const override;

    class Factory : public Ogre::ArchiveFactory
    {
    public:
        virtual const Ogre::String & getType() const override;

        virtual Ogre::Archive * createInstance(const Ogre::String & name, bool bReadOnly) override;

        virtual void destroyInstance(Ogre::Archive * archive) override;
    };

private:
    std::unique_ptr<ResFileInfo> m_archiveInfo;
    Ogre::FileInfoList m_fileInfoList;
    const std::string m_resId;

    bool FindResEntry(const Ogre::String & filename, ResEntry& entry) const;

    static std::string GetInternalFileName(const std::string& filename, bool& isOgreMaterial);
    static std::string GetPublicFilename(const std::string& filename);
};



} // namespace res
} // namespace archive
} // namespace resource
} // namespace d2_hack  

#endif // D2_HACK_RESOURCE_ARCHIVE_INCLUDE_D2_HACK_RESOURCE_ARCHIVE_RES_ARCHIVE_H
