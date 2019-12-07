#include "txr_image_codec.h"

#include <climits>
#include <cstdint>

#include <OgreLogManager.h>
#include <OgreImage.h>
#include <OgreDataStream.h>
#include <OgreString.h>

#include "d2_hack_common.h"

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
        std::uint8_t idlength;
        std::uint8_t colourmaptype;
        std::uint8_t datatypecode;
        std::uint8_t colourmaporigin[2];
        std::uint8_t colourmaplength[2];
        std::uint8_t colourmapdepth;
        std::uint8_t x_origin[2];
        std::uint8_t y_origin[2];
        std::uint8_t width[2];
        std::uint8_t height[2];
        std::uint8_t bitsperpixel;
        std::uint8_t imagedescriptor;
    };

    static_assert(sizeof(TgaHeader) == 18, "Incorrect TGA header declaration");


}




Ogre::DataStreamPtr TxrImageCodec::encode(const Ogre::MemoryDataStreamPtr& /* input */,
                                          const Ogre::Codec::CodecDataPtr& /* pData */) const
{
    OGRE_EXCEPT(Ogre::Exception::ERR_NOT_IMPLEMENTED, "encode is not implemented", "TxrImageCodec::encode");
}

void TxrImageCodec::encodeToFile(const Ogre::MemoryDataStreamPtr& /* input */,
                                 const Ogre::String& /* outFileName */,
                                 const Ogre::Codec::CodecDataPtr& /* pData */) const
{
    OGRE_EXCEPT(Ogre::Exception::ERR_NOT_IMPLEMENTED, "encodeToFile is not implemented", "TxrImageCodec::encodeToFile");
}

Ogre::Codec::DecodeResult TxrImageCodec::decode(const Ogre::DataStreamPtr& input) const
{
    detail::TgaHeader tgaHeader;
    if (input->read(&tgaHeader, sizeof(tgaHeader)) != sizeof(tgaHeader))
    {
        OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS,
                    "Can`t read tga header",
                    "TxrImageCodec::decode");
    }

    OGRE_LOG(TxrImageCodec) << "length of image id data: " << static_cast<int>(tgaHeader.idlength);

    OGRE_LOG(TxrImageCodec) << "color map type: " << static_cast<int>(tgaHeader.colourmaptype);
    if (tgaHeader.colourmaptype != 0)
    {
        OGRE_EXCEPT(Ogre::Exception::ERR_NOT_IMPLEMENTED,
                    "can`t decode color mapped image",
                    "TxrImageCodec::decode");
    }

    OGRE_LOG(TxrImageCodec) << "data type: " << static_cast<int>(tgaHeader.datatypecode);
    if (tgaHeader.datatypecode != 2)
    {
        OGRE_EXCEPT(Ogre::Exception::ERR_NOT_IMPLEMENTED,
                    "can`t decode other then uncompressed RGB",
                    "TxrImageCodec::decode");
    }
    OGRE_LOG(TxrImageCodec) << "data type is uncompressed RGB image";


    /// @todo endian issue
    std::uint16_t xOrigin = *reinterpret_cast<const std::uint16_t*>(tgaHeader.x_origin);
    std::uint16_t yOrigin = *reinterpret_cast<const std::uint16_t*>(tgaHeader.y_origin);
    std::uint16_t width = *reinterpret_cast<const std::uint16_t*>(tgaHeader.width);
    std::uint16_t height = *reinterpret_cast<const std::uint16_t*>(tgaHeader.height);

    OGRE_LOG(TxrImageCodec) << "image size " << width << " x " << height;
    OGRE_LOG(TxrImageCodec) << "image offset " << xOrigin << " x " << yOrigin;


    OGRE_LOG(TxrImageCodec) << "bits per pixel: " << static_cast<int>(tgaHeader.bitsperpixel);

    OGRE_LOG(TxrImageCodec) << "image descriptor: " << static_cast<int>(tgaHeader.imagedescriptor);

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

    return "";
}
