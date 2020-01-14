#include <d2_hack/resource/image/txr.h>

#include <cstdint>

#include <OgreImage.h>
#include <OgreDataStream.h>

#include <d2_hack/common/log.h>
#include <d2_hack/common/numeric_conversion.h>

namespace d2_hack
{
namespace resource
{
namespace image
{

namespace
{
const Ogre::String TxrCodecType = "txr";
const char TxrMagicNumber [] = " LOFF";

size_t TxrMagicNumberOffset = 16;
size_t TxrMagicNumberSize = 5;
} // namespace

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

} // namespace detail




Ogre::DataStreamPtr TxrImageCodec::encode(const Ogre::MemoryDataStreamPtr& /* input */,
                                          const Ogre::Codec::CodecDataPtr& /* pData */) const
{
    OGRE_EXCEPT(Ogre::Exception::ERR_NOT_IMPLEMENTED, "encode is not implemented");
}

void TxrImageCodec::encodeToFile(const Ogre::MemoryDataStreamPtr& /* input */,
                                 const Ogre::String& /* outFileName */,
                                 const Ogre::Codec::CodecDataPtr& /* pData */) const
{
    OGRE_EXCEPT(Ogre::Exception::ERR_NOT_IMPLEMENTED, "encodeToFile is not implemented");
}

Ogre::Codec::DecodeResult TxrImageCodec::decode(const Ogre::DataStreamPtr& input) const
{
    detail::TgaHeader tgaHeader;
    if (input->read(&tgaHeader, sizeof(tgaHeader)) != sizeof(tgaHeader))
    {
        OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS, "Can`t read tga header");
    }

    D2_HACK_LOG(TxrImageCodec) << "length of image id data: " << static_cast<int>(tgaHeader.idlength);

    D2_HACK_LOG(TxrImageCodec) << "color map type: " << static_cast<int>(tgaHeader.colourmaptype);
    if (tgaHeader.colourmaptype != 0)
    {
        OGRE_EXCEPT(Ogre::Exception::ERR_NOT_IMPLEMENTED, "can`t decode color mapped image");
    }

    D2_HACK_LOG(TxrImageCodec) << "data type: " << static_cast<int>(tgaHeader.datatypecode);
    if (tgaHeader.datatypecode != 2)
    {
        OGRE_EXCEPT(Ogre::Exception::ERR_NOT_IMPLEMENTED, "can`t decode other then uncompressed RGB");
    }
    D2_HACK_LOG(TxrImageCodec) << "data type is uncompressed RGB image";

    std::uint16_t xOrigin = common::ToNumeric<std::uint16_t>(tgaHeader.x_origin);
    std::uint16_t yOrigin = common::ToNumeric<std::uint16_t>(tgaHeader.y_origin);
    std::uint16_t width = common::ToNumeric<std::uint16_t>(tgaHeader.width);
    std::uint16_t height = common::ToNumeric<std::uint16_t>(tgaHeader.height);

    D2_HACK_LOG(TxrImageCodec) << "image size " << width << " x " << height;
    D2_HACK_LOG(TxrImageCodec) << "image offset " << xOrigin << " x " << yOrigin;


    D2_HACK_LOG(TxrImageCodec) << "bits per pixel: " << static_cast<int>(tgaHeader.bitsperpixel);

    D2_HACK_LOG(TxrImageCodec) << "image descriptor: " << static_cast<int>(tgaHeader.imagedescriptor);

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
        OGRE_EXCEPT(Ogre::Exception::ERR_NOT_IMPLEMENTED, "can`t decode bits per pixel");
    }

    imgData->size = Ogre::Image::calculateSize(imgData->num_mipmaps, 1,
                                               imgData->width, imgData->height,
                                               imgData->depth, imgData->format);

    Ogre::MemoryDataStreamPtr output(new Ogre::MemoryDataStream(imgData->size));
    if (input->read(output->getPtr(), imgData->size) != imgData->size)
    {
        OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS, "Failed to read image data");
    }

    Ogre::Codec::DecodeResult res(output, Ogre::Codec::CodecDataPtr(imgData.get()));
    imgData.release();

    return res;
}

Ogre::String TxrImageCodec::getType() const
{
    return TxrCodecType;
}

Ogre::String TxrImageCodec::magicNumberToFileExt(const char *magicNumberPtr, size_t maxbytes) const
{
    if (maxbytes >= (TxrMagicNumberOffset + TxrMagicNumberSize))
    {
        if (memcmp(TxrMagicNumber, magicNumberPtr + TxrMagicNumberOffset, TxrMagicNumberSize) == 0)
        {
            return "txr";
        }
    }

    return "";
}

} // namespace image
} // namespace resource
} // namespace d2_hack
