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

    virtual size_t tell() const;

    virtual bool eof() const;

    virtual void close();

private:
    Ogre::DataStreamPtr m_stream;
    size_t m_offset;
};

#endif // D2_HACK_RESOURCE_PLUGIN_D2_OFFSET_DATA_STREAM_H
