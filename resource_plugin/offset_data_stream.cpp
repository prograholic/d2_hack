#include "offset_data_stream.h"

#include <algorithm>

#include <OgreException.h>

namespace
{

}

OffsetDataStream::OffsetDataStream(Ogre::DataStreamPtr stream, size_t offset, size_t size)
    : Ogre::DataStream(stream->getName(), stream->getAccessMode())
    , m_stream(stream)
    , m_offset(offset)
{
    size_t underlyingStreamSize = stream->size();

    if (underlyingStreamSize < offset)
    {
        OGRE_EXCEPT(Ogre::Exception::ERR_INVALID_STATE,
            "Stream size cannot be lower than offset",
            "OffsetDataStream::OffsetDataStream");
    }

    if ((underlyingStreamSize - offset) < size)
    {
        OGRE_EXCEPT(Ogre::Exception::ERR_INVALID_STATE,
            "Stream size cannot be lower than offset + size",
            "OffsetDataStream::OffsetDataStream");
    }

    mSize = size;

    m_stream->seek(m_offset);
}

size_t OffsetDataStream::read(void* buf, size_t count)
{
    count = (std::min)(count, size() - tell());

    return m_stream->read(buf, count);
}

void OffsetDataStream::skip(long count)
{
    assert((tell() + count) <= size());

    m_stream->skip(count);
}

void OffsetDataStream::seek(size_t pos)
{
    assert(pos <= size());

    m_stream->seek(m_offset + pos);
}

size_t OffsetDataStream::tell() const
{
    size_t pos = m_stream->tell();

    assert(pos >= m_offset);
    assert((pos - m_offset) <= mSize);

    return pos - m_offset;
}

bool OffsetDataStream::eof() const
{
    return (tell() == size());
}

void OffsetDataStream::close()
{
    m_stream->close();
}
