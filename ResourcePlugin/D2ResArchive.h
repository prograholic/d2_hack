#ifndef D2_HACK_RESOURCE_PLUGIN_D2_RES_ARCHIVE_H
#define D2_HACK_RESOURCE_PLUGIN_D2_RES_ARCHIVE_H

#include <OgreArchive.h>
#include <OgreArchiveFactory.h>

#include "D2ResFileInfo.h"

class D2ResArchive : public Ogre::Archive
{
public:
  D2ResArchive(const Ogre::String& name, const Ogre::String& archType);

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

private:

  D2ResFileInfo mArchiveInfo;
  Ogre::FileInfoList mFileInfoList;

  bool findEntry(const Ogre::String & filename, D2ResEntry & entry) const;

};




class D2ResArchiveFactory : public Ogre::ArchiveFactory
{
public:

  virtual const Ogre::String& getType() const;

  virtual Ogre::Archive* createInstance(const Ogre::String& name, bool bReadOnly);

  virtual void destroyInstance(Ogre::Archive* archive);

};


#endif // D2_HACK_RESOURCE_PLUGIN_D2_RES_ARCHIVE_H
