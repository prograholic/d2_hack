#include <d2_hack/resource/image/raw.h>

#include <memory>

#include <OgreDataStream.h>
#include <OgreImage.h>

#include <d2_hack/common/numeric_conversion.h>

namespace d2_hack
{
namespace resource
{
namespace image
{

namespace
{
const Ogre::String CodecType = "raw2";
} // namespace

Ogre::DataStreamPtr RawImageCodec::encode(const Ogre::MemoryDataStreamPtr& /* input */, const CodecDataPtr& /* pData */) const
{
    OGRE_EXCEPT(Ogre::Exception::ERR_NOT_IMPLEMENTED, "encode is not implemented");
}

void RawImageCodec::encodeToFile(const Ogre::MemoryDataStreamPtr& /* input */,
                                 const Ogre::String& /* outFileName */,
                                 const CodecDataPtr& /* pData */) const
{
    OGRE_EXCEPT(Ogre::Exception::ERR_NOT_IMPLEMENTED, "encodeToFile is not implemented");
}

Ogre::Codec::DecodeResult RawImageCodec::decode(const Ogre::DataStreamPtr& input) const
{
    size_t dataSize = input->size();

    if ((dataSize % 2) != 0)
    {
        OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS, "Invalid file size, must be even");
    }

    size_t dataSize2 = dataSize / 2;

    size_t heightLog2 = 0;
    size_t height = (1 << heightLog2) + 1;
    for ( ; ; )
    {
        height = (1 << heightLog2) + 1;
        size_t heightSquared = height * height;

        if (heightSquared < dataSize2)
        {
            ++heightLog2;
        }
        else if (heightSquared == dataSize2)
        {
            break;
        }
        else if (heightSquared > dataSize2)
        {
            OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS, "Invalid file size, must be equal to 2 * ((2 ^ n) + 1) ^ 2");
        }
    }


    std::unique_ptr<ImageData> imgData(new ImageData());

    imgData->depth = 1;
    imgData->width = common::NumericCast<Ogre::uint32>(height);
    imgData->height = imgData->width;
    imgData->num_mipmaps = 0;
    imgData->flags = 0;
    imgData->format = Ogre::PF_L16;

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

Ogre::String RawImageCodec::getType() const
{
    return CodecType;
}

Ogre::String RawImageCodec::magicNumberToFileExt(const char* /* magicNumberPtr */, size_t /* maxbytes */) const
{
    OGRE_EXCEPT(Ogre::Exception::ERR_NOT_IMPLEMENTED, "magicNumberToFileExt is not implemented");
}

} // namespace image
} // namespace resource
} // namespace d2_hack
