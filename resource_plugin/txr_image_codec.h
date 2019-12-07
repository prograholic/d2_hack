#ifndef D2_HACK_RESOURCE_PLUGIN_TXR_IMAGE_CODEC_H
#define D2_HACK_RESOURCE_PLUGIN_TXR_IMAGE_CODEC_H

#include <OgreImageCodec.h>

class TxrImageCodec : public Ogre::ImageCodec
{
public:
    virtual Ogre::DataStreamPtr encode(const Ogre::MemoryDataStreamPtr& input, const CodecDataPtr& pData) const override;

    virtual void encodeToFile(const Ogre::MemoryDataStreamPtr& input, const Ogre::String& outFileName, const CodecDataPtr& pData) const override;

    virtual Ogre::Codec::DecodeResult decode(const Ogre::DataStreamPtr& input) const override;

    virtual Ogre::String getType() const override;

    virtual Ogre::String magicNumberToFileExt(const char *magicNumberPtr, size_t maxbytes) const override;
};

#endif // D2_HACK_RESOURCE_PLUGIN_TXR_IMAGE_CODEC_H
