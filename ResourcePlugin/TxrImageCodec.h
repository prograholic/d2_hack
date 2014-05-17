#ifndef D2_HACK_RESOURCE_PLUGIN_TXR_IMAGE_CODEC_H
#define D2_HACK_RESOURCE_PLUGIN_TXR_IMAGE_CODEC_H

#include <OgreImageCodec.h>

class TxrImageCodec : public Ogre::ImageCodec
{
public:
  virtual Ogre::DataStreamPtr encode(Ogre::MemoryDataStreamPtr& input, CodecDataPtr& pData) const;

  virtual void encodeToFile(Ogre::MemoryDataStreamPtr& input, const Ogre::String& outFileName, CodecDataPtr& pData) const;

  virtual Ogre::Codec::DecodeResult decode(Ogre::DataStreamPtr& input) const;

  virtual Ogre::String getType() const;

  virtual Ogre::String magicNumberToFileExt(const char *magicNumberPtr, size_t maxbytes) const;
};

#endif // D2_HACK_RESOURCE_PLUGIN_TXR_IMAGE_CODEC_H
