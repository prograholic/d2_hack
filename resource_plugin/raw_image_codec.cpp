#include "raw_image_codec.h"

#include <memory>

#include <boost/numeric/conversion/cast.hpp>

#include <OgreImage.h>
#include <OgreDataStream.h>


namespace
{
const Ogre::String rawCodecType = "raw2";
}

Ogre::DataStreamPtr RawImageCodec::encode(const Ogre::MemoryDataStreamPtr& /* input */, const CodecDataPtr& /* pData */) const
{
    OGRE_EXCEPT(Ogre::Exception::ERR_NOT_IMPLEMENTED,
                "encode is not implemented",
                "RawImageCodec::encode");
}

void RawImageCodec::encodeToFile(const Ogre::MemoryDataStreamPtr& /* input */,
                                 const Ogre::String& /* outFileName */,
                                 const CodecDataPtr& /* pData */) const
{
    OGRE_EXCEPT(Ogre::Exception::ERR_NOT_IMPLEMENTED,
                "encodeToFile is not implemented",
                "RawImageCodec::encodeToFile");
}

Ogre::Codec::DecodeResult RawImageCodec::decode(const Ogre::DataStreamPtr& input) const
{
    size_t dataSize = input->size();

    if ((dataSize % 2)    != 0)
    {
        OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS,
                    "Invalid file size, must be even",
                    "RawImageCodec::decode");
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
            OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS,
                        "Invalid file size, must be equal to 2 * ((2 ^ n) + 1) ^ 2",
                        "RawImageCodec::decode");
        }
    }


    std::unique_ptr<ImageData> imgData(new ImageData());

    imgData->depth = 1;
    imgData->width = boost::numeric_cast<Ogre::uint32>(height);
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
        OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS,
                    "Failed to read image data",
                    "RawImageCodec::decode");
    }

    Ogre::Codec::DecodeResult res(output, Ogre::Codec::CodecDataPtr(imgData.get()));
    imgData.release();

    return res;
}

Ogre::String RawImageCodec::getType() const
{
    return rawCodecType;
}

Ogre::String RawImageCodec::magicNumberToFileExt(const char* /* magicNumberPtr */, size_t /* maxbytes */) const
{
    OGRE_EXCEPT(Ogre::Exception::ERR_NOT_IMPLEMENTED,
                "magicNumberToFileExt is not implemented",
                "RawImageCodec::magicNumberToFileExt");
}
