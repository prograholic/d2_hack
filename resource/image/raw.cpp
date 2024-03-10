#include <d2_hack/resource/image/raw.h>


#include <OgreDataStream.h>
#include <OgreImage.h>

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

void RawImageCodec::decode(const Ogre::DataStreamPtr& input, const Ogre::Any& output) const
{
    size_t dataSize = input->size();

    if ((dataSize % 2) != 0)
    {
        OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS, "Invalid file size, must be even");
    }

    size_t dataSize2 = dataSize / 2;

    std::uint32_t heightLog2 = 0;
    std::uint32_t height = (1 << heightLog2) + 1;
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

    Ogre::Image* image = any_cast<Ogre::Image*>(output);
    image->create(Ogre::PF_L16, height, height, 1, 1, 0);
    if (input->read(image->getData(), image->getSize()) != image->getSize())
    {
        OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS, "Failed to read image data");
    }
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
