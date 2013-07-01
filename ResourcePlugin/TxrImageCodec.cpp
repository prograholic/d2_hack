#include "TxrImageCodec.h"

#include <climits>

#include <OgreLogManager.h>
#include <OgreImage.h>

namespace
{
  const Ogre::String txrCodecType = "txr";
  const char txrMagicNumber [] = " LOFF";

  size_t txrMagicNumberOffset = 16;
  size_t txrMagicNumberSize = 5;
}


namespace detail
{
  struct TgaHeader
  {
    boost::uint8_t idlength;
    boost::uint8_t colourmaptype;
    boost::uint8_t datatypecode;
    boost::uint8_t colourmaporigin[2];
    boost::uint8_t colourmaplength[2];
    boost::uint8_t colourmapdepth;
    boost::uint8_t x_origin[2];
    boost::uint8_t y_origin[2];
    boost::uint8_t width[2];
    boost::uint8_t height[2];
    boost::uint8_t bitsperpixel;
    boost::uint8_t imagedescriptor;
  };

  BOOST_STATIC_ASSERT(sizeof(TgaHeader) == 18);


}




TxrImageCodec::TxrImageCodec()
{
}

Ogre::DataStreamPtr TxrImageCodec::code(Ogre::MemoryDataStreamPtr& input, Ogre::Codec::CodecDataPtr& pData) const
{
  OGRE_EXCEPT(Ogre::Exception::ERR_NOT_IMPLEMENTED, "code is not implemented", "TxrImageCodec::code");
}

void TxrImageCodec::codeToFile(Ogre::MemoryDataStreamPtr& input,
                               const Ogre::String& outFileName,
                               Ogre::Codec::CodecDataPtr& pData) const
{
  OGRE_EXCEPT(Ogre::Exception::ERR_NOT_IMPLEMENTED, "codeToFile is not implemented", "TxrImageCodec::codeToFile");
}

Ogre::Codec::DecodeResult TxrImageCodec::decode(Ogre::DataStreamPtr& input) const
{
  detail::TgaHeader tgaHeader;
  if (input->read(&tgaHeader, sizeof(tgaHeader)) != sizeof(tgaHeader))
  {
    OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS,
                "Can`t read tga header",
                "TxrImageCodec::decode");
  }

  Ogre::LogManager::getSingleton().stream() << "length of image id data: " << static_cast<int>(tgaHeader.idlength);

  Ogre::LogManager::getSingleton().stream() << "color map type: " << static_cast<int>(tgaHeader.colourmaptype);
  if (tgaHeader.colourmaptype != 0)
  {
    OGRE_EXCEPT(Ogre::Exception::ERR_NOT_IMPLEMENTED,
                "can`t decode color mapped image",
                "TxrImageCodec::decode");
  }

  Ogre::LogManager::getSingleton().stream() << "data type: " << static_cast<int>(tgaHeader.datatypecode);
  if (tgaHeader.datatypecode != 2)
  {
    OGRE_EXCEPT(Ogre::Exception::ERR_NOT_IMPLEMENTED,
                "can`t decode other then uncompressed RGB",
                "TxrImageCodec::decode");
  }
  Ogre::LogManager::getSingleton().stream() << "data type is uncompressed RGB image";


  /// @todo endian issue
  boost::uint16_t xOrigin = *reinterpret_cast<const boost::uint16_t*>(tgaHeader.x_origin);
  boost::uint16_t yOrigin = *reinterpret_cast<const boost::uint16_t*>(tgaHeader.y_origin);
  boost::uint16_t width = *reinterpret_cast<const boost::uint16_t*>(tgaHeader.width);
  boost::uint16_t height = *reinterpret_cast<const boost::uint16_t*>(tgaHeader.height);

  Ogre::LogManager::getSingleton().stream() << "image size " << width << " x " << height;
  Ogre::LogManager::getSingleton().stream() << "image offset " << xOrigin << " x " << yOrigin;


  Ogre::LogManager::getSingleton().stream() << "bits per pixel: " << static_cast<int>(tgaHeader.bitsperpixel);

  Ogre::LogManager::getSingleton().stream() << "image descriptor: " << static_cast<int>(tgaHeader.imagedescriptor);

  input->skip(tgaHeader.idlength);

  std::auto_ptr<ImageData> imgData(new ImageData());

  imgData->depth = 1;
  imgData->width = width;
  imgData->height = height;
  imgData->num_mipmaps = 0;
  imgData->flags = 0;
  switch (tgaHeader.bitsperpixel)
  {
  case 16:
    imgData->format = Ogre::PF_R5G6B5;
    break;

  default:
    OGRE_EXCEPT(Ogre::Exception::ERR_NOT_IMPLEMENTED,
                "can`t decode bits per pixel",
                "TxrImageCodec::decode");
  }

  imgData->size = Ogre::Image::calculateSize(imgData->num_mipmaps, 1,
                                             imgData->width, imgData->height,
                                             imgData->depth, imgData->format);

  Ogre::MemoryDataStreamPtr output(new Ogre::MemoryDataStream(imgData->size));
  if (input->read(output->getPtr(), imgData->size) != imgData->size)
  {
    OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS,
                "Failed to read image data",
                "TxrImageCodec::decode");
  }

  Ogre::Codec::DecodeResult res(output, Ogre::Codec::CodecDataPtr(imgData.get()));
  imgData.release();

  return res;
}

Ogre::String TxrImageCodec::getType() const
{
  return txrCodecType;
}

Ogre::String TxrImageCodec::magicNumberToFileExt(const char *magicNumberPtr, size_t maxbytes) const
{
  if (maxbytes >= (txrMagicNumberOffset + txrMagicNumberSize))
  {
    if (memcmp(txrMagicNumber, magicNumberPtr + txrMagicNumberOffset, txrMagicNumberSize) == 0)
    {
      return "txr";
    }
  }

  return Ogre::StringUtil::BLANK;
}
