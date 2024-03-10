#include <d2_hack/resource/image/txr.h>

#include <OgreDataStream.h>
#include <OgreImage.h>

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


void TxrImageCodec::decode(const Ogre::DataStreamPtr& input, const Ogre::Any& output) const
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

    Ogre::PixelFormat pixelFormat;
    switch (tgaHeader.bitsperpixel)
    {
    case 16:
        pixelFormat = Ogre::PF_R5G6B5;
        break;

    default:
        OGRE_EXCEPT(Ogre::Exception::ERR_NOT_IMPLEMENTED, "can`t decode bits per pixel");
    }

    input->skip(tgaHeader.idlength);

    Ogre::Image* image = any_cast<Ogre::Image*>(output);
    image->create(pixelFormat, width, height, 1, 1, 0);
    if (input->read(image->getData(), image->getSize()) != image->getSize())
    {
        OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS, "Failed to read image data");
    }
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
