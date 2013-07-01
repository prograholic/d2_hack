#include "OffsetDataStream.h"

#include <algorithm>

#include <OgreException.h>

namespace
{

}

OffsetDataStream::OffsetDataStream(Ogre::DataStreamPtr stream, size_t offset, size_t size)
  : Ogre::DataStream(stream->getName(), stream->getAccessMode())
  , mStream(stream)
  , mOffset(offset)
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

  mStream->seek(mOffset);
}

size_t OffsetDataStream::read(void* buf, size_t count)
{
  count = (std::min)(count, size() - tell());

  return mStream->read(buf, count);
}

void OffsetDataStream::skip(long count)
{
  assert((tell() + count) <= size());

  mStream->skip(count);
}

void OffsetDataStream::seek(size_t pos)
{
  assert(pos <= size());

  mStream->seek(mOffset + pos);
}

size_t OffsetDataStream::tell(void) const
{
  size_t pos = mStream->tell();

  assert(pos >= mOffset);
  assert((pos - mOffset) <= mSize);

  return pos - mOffset;
}

bool OffsetDataStream::eof(void) const
{
  return (tell() == size());
}

void OffsetDataStream::close(void)
{
  mStream->close();
}
