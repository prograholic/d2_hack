#ifndef D2_HACK_CODEC_IMAGE_INCLUDE_D2_HACK_CODEC_IMAGE_RAW_H
#define D2_HACK_CODEC_IMAGE_INCLUDE_D2_HACK_CODEC_IMAGE_RAW_H

#include <d2_hack/common/platform.h>

#include <OgreImageCodec.h>

namespace d2_hack
{
namespace codec
{
namespace image
{

class RawImageCodec : public Ogre::ImageCodec
{
public:
    virtual Ogre::DataStreamPtr encode(const Ogre::MemoryDataStreamPtr& input, const CodecDataPtr& pData) const override;

    virtual void encodeToFile(const Ogre::MemoryDataStreamPtr& input, const Ogre::String& outFileName, const CodecDataPtr& pData) const override;

    virtual Ogre::Codec::DecodeResult decode(const Ogre::DataStreamPtr& input) const override;

    virtual Ogre::String getType() const override;

    virtual Ogre::String magicNumberToFileExt(const char *magicNumberPtr, size_t maxbytes) const override;
};

} // namespace image
} // namespace codec
} // namespace d2_hack

#endif /* D2_HACK_CODEC_IMAGE_INCLUDE_D2_HACK_CODEC_IMAGE_RAW_H */

