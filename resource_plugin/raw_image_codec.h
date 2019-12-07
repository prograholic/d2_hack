#ifndef D2_HACK_RESOURCE_PLUGIN_D2_RAW_LOADER_H
#define D2_HACK_RESOURCE_PLUGIN_D2_RAW_LOADER_H

#include <OgreImageCodec.h>

class RawImageCodec : public Ogre::ImageCodec
{
public:
    virtual Ogre::DataStreamPtr encode(const Ogre::MemoryDataStreamPtr& input, const CodecDataPtr& pData) const override;

    virtual void encodeToFile(const Ogre::MemoryDataStreamPtr& input, const Ogre::String& outFileName, const CodecDataPtr& pData) const override;

    virtual Ogre::Codec::DecodeResult decode(const Ogre::DataStreamPtr& input) const override;

    virtual Ogre::String getType() const override;

    virtual Ogre::String magicNumberToFileExt(const char *magicNumberPtr, size_t maxbytes) const override;
};

#endif /* D2_HACK_RESOURCE_PLUGIN_D2_RAW_LOADER_H */

