#include <d2_hack/resource/image/txr.h>

#include <OgreDataStream.h>
#include <OgreImage.h>

#include <d2_hack/common/log.h>
#include <d2_hack/common/numeric_conversion.h>
#include <d2_hack/common/resource_mgmt.h>
#include <d2_hack/common/reader.h>

namespace d2_hack
{
namespace resource
{
namespace image
{

namespace
{
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

struct Loffdata
{
    char signature[4] = {};
    std::uint32_t size = 0;
    std::uint32_t offset = 0;
};

static_assert(sizeof(Loffdata) == 12, "Incorrect Loffdata declaration");

struct PfrmData
{
    std::uint32_t size = 0;
    std::uint32_t redMask = 0;
    std::uint32_t greenMask = 0;
    std::uint32_t blueMask = 0;
    std::uint32_t alphaMask = 0;
};

static_assert(sizeof(PfrmData) == 20, "Incorrect PfrmData declaration");


struct MipMapDescriptor
{
    std::uint32_t sectionSize = 0;
    std::uint32_t mipMapCount = 0;
    std::uint32_t width = 0;
    std::uint32_t height = 0;
    std::uint32_t bytePerPixel = 0;
};

static_assert(sizeof(MipMapDescriptor) == 20, "Incorrect MipMapDescriptor declaration");

} // namespace detail

static Ogre::PixelFormat DeducePixelFormatFromPfrm(const detail::PfrmData& pfrm)
{
    if ((pfrm.alphaMask == 0xf000) && (pfrm.redMask == 0x0f00) && (pfrm.greenMask == 0x00f0) && (pfrm.blueMask == 0x000f))
    {
        return Ogre::PixelFormat::PF_A4R4G4B4;
    }
    else if ((pfrm.redMask == 0xf800) && (pfrm.greenMask == 0x07e0) && (pfrm.blueMask == 0x001f) && (pfrm.alphaMask == 0x0000))
    {
        return Ogre::PixelFormat::PF_R5G6B5;
    }


    OGRE_EXCEPT(Ogre::Exception::ERR_INVALID_STATE,
                std::format("Cannot deduce pixel format from Pfrm: {}, {}, {}, {}", pfrm.redMask, pfrm.greenMask, pfrm.blueMask, pfrm.alphaMask));
}


void TxrImageCodec::decode(const Ogre::DataStreamPtr& input, const Ogre::Any& output) const
{
    common::Reader reader{*input};
    detail::TgaHeader tgaHeader;

    reader.TypedRead(tgaHeader);

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

    if (tgaHeader.bitsperpixel != 16)
    {
        OGRE_EXCEPT(Ogre::Exception::ERR_NOT_IMPLEMENTED, "can`t decode bits per pixel other than 16: " + std::to_string(static_cast<int>(tgaHeader.bitsperpixel)));
    }
    if (tgaHeader.imagedescriptor != 32)
    {
        OGRE_EXCEPT(Ogre::Exception::ERR_NOT_IMPLEMENTED, "can`t decode image format descriptor other than 32: " + std::to_string(static_cast<int>(tgaHeader.imagedescriptor)));
    }

    detail::Loffdata loff;
    reader.TypedRead(loff);

    if (memcmp(loff.signature, "LOFF", 4) != 0)
    {
        OGRE_EXCEPT(Ogre::Exception::ERR_INVALID_STATE, ("Invalid LOFF signature: " + std::string{loff.signature, 4}));
    }

    if (loff.size != 4)
    {
        OGRE_EXCEPT(Ogre::Exception::ERR_INVALID_STATE, "LOFF, incorrect size: " + std::to_string(loff.size));
    }

    std::vector<std::uint8_t> imageBuffer;
    imageBuffer.resize(width * height * tgaHeader.bitsperpixel / CHAR_BIT);
    reader.ReadBytes(imageBuffer.data(), imageBuffer.size());

    std::array<char, 4> signature;
    reader.TypedRead(signature);

    detail::PfrmData pfrmData;
    detail::MipMapDescriptor desc;

    if (memcmp(signature.data(), "PFRM", 4) == 0)
    {
        reader.TypedRead(pfrmData);
    }
    else if (memcmp(signature.data(), "LVMP", 4) == 0)
    {
        reader.TypedRead(desc);

        // read mipmaps
        auto mipmaps = desc.mipMapCount;
        while (mipmaps > 0)
        {
            size_t mipmapSize = desc.height * desc.width * desc.bytePerPixel;
            size_t offset = imageBuffer.size();
            imageBuffer.resize(imageBuffer.size() + mipmapSize);
            reader.ReadBytes(imageBuffer.data() + offset, mipmapSize);

            desc.height /= 2;
            desc.width /= 2;
            mipmaps -= 1;
        }

        std::array<std::uint8_t, 2> unknown;
        reader.TypedRead(unknown);

        reader.TypedRead(signature);
        reader.TypedRead(pfrmData);
    }
    else
    {
        OGRE_EXCEPT(Ogre::Exception::ERR_INVALID_STATE, "Unknown Signature");
    }

    std::vector<std::uint8_t> footer;
    size_t pos = 0;
    while (!input->eof())
    {
        if (pos >= footer.size())
        {
            footer.resize(pos + 1);
        }
        input->read(&footer[pos], 1);
        pos += 1;
    }

    Ogre::PixelFormat pixelFormat = DeducePixelFormatFromPfrm(pfrmData);

    Ogre::Image* image = any_cast<Ogre::Image*>(output);
    image->create(pixelFormat, width, height, 1, 1, desc.mipMapCount);
    if (image->getSize() != imageBuffer.size())
    {
        OGRE_EXCEPT(Ogre::Exception::ERR_INVALID_STATE, std::format("Incorrect image size, expected: {}, got: {}", image->getSize(), imageBuffer.size()));
    }
    memcpy(image->getData(), imageBuffer.data(), imageBuffer.size());
}

Ogre::String TxrImageCodec::getType() const
{
    return common::extensions::TextureFileExtNoDot;
}

Ogre::String TxrImageCodec::magicNumberToFileExt(const char *magicNumberPtr, size_t maxbytes) const
{
    if (maxbytes >= (TxrMagicNumberOffset + TxrMagicNumberSize))
    {
        if (memcmp(TxrMagicNumber, magicNumberPtr + TxrMagicNumberOffset, TxrMagicNumberSize) == 0)
        {
            return common::extensions::TextureFileExtNoDot;
        }
    }

    return "";
}

} // namespace image
} // namespace resource
} // namespace d2_hack
