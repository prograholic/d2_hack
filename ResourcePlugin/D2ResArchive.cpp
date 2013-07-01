#include "D2ResArchive.h"

#include <memory>
#include <fstream>

#include "OffsetDataStream.h"

D2ResArchive::D2ResArchive(const Ogre::String& name, const Ogre::String& archType)
  : Ogre::Archive(name, archType)
{
}


bool D2ResArchive::isCaseSensitive(void) const
{
  return true;
}

void D2ResArchive::load()
{

}

void D2ResArchive::unload()
{

}


Ogre::DataStreamPtr D2ResArchive::open(const Ogre::String& filename, bool /* readOnly */) const
{
  std::auto_ptr<std::ifstream> stdStream(new std::ifstream(filename.c_str(), std::ios_base::binary));

  if (*stdStream)
  {
    Ogre::DataStreamPtr fileStream(new Ogre::FileStreamDataStream(stdStream.get()));
    stdStream.release();

    return Ogre::DataStreamPtr(new OffsetDataStream(fileStream, 0, 0));
  }
  return Ogre::DataStreamPtr();
}

Ogre::StringVectorPtr D2ResArchive::list(bool recursive, bool dirs)
{

}

Ogre::FileInfoListPtr D2ResArchive::listFileInfo(bool recursive, bool dirs)
{

}

Ogre::StringVectorPtr D2ResArchive::find(const Ogre::String& pattern, bool recursive, bool dirs)
{

}

bool D2ResArchive::exists(const Ogre::String& filename)
{

}

time_t D2ResArchive::getModifiedTime(const Ogre::String& filename)
{

}

Ogre::FileInfoListPtr D2ResArchive::findFileInfo(const Ogre::String& pattern, bool recursive, bool dirs) const
{

}


/////////////////////////////////////////////////////////////////////////////////////////

namespace
{
  Ogre::String d2ResFactoryType = "res";
}

const Ogre::String& D2ResArchiveFactory::getType() const
{
  return d2ResFactoryType;
}

Ogre::Archive* D2ResArchiveFactory::createInstance(const Ogre::String& name)
{
  return new D2ResArchive(name, "D2Res");
}

void D2ResArchiveFactory::destroyInstance(Ogre::Archive* archive)
{
  delete archive;
}
