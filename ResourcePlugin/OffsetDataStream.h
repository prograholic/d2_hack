#ifndef D2_HACK_RESOURCE_PLUGIN_D2_OFFSET_DATA_STREAM_H
#define D2_HACK_RESOURCE_PLUGIN_D2_OFFSET_DATA_STREAM_H


#include <OgreDataStream.h>

class OffsetDataStream : public Ogre::DataStream
{
public:
  OffsetDataStream(Ogre::DataStreamPtr stream, size_t offset, size_t size);

  virtual size_t read(void* buf, size_t count);

  virtual void skip(long count);

  virtual void seek(size_t pos);

  virtual size_t tell(void) const;

  virtual bool eof(void) const;

  virtual void close(void);

private:
  Ogre::DataStreamPtr mStream;
  size_t mOffset;
};

#endif // D2_HACK_RESOURCE_PLUGIN_D2_OFFSET_DATA_STREAM_H
