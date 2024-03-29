#ifndef D2_HACK_RESOURCE_IMAGE_INCLUDE_D2_HACK_RESOURCE_IMAGE_RAW_H
#define D2_HACK_RESOURCE_IMAGE_INCLUDE_D2_HACK_RESOURCE_IMAGE_RAW_H

#include <d2_hack/common/platform.h>

#include <OgreImageCodec.h>

namespace d2_hack
{
namespace resource
{
namespace image
{

class RawImageCodec : public Ogre::ImageCodec
{
public:
    virtual void decode(const Ogre::DataStreamPtr& input, const Ogre::Any& output) const override;

    virtual Ogre::String getType() const override;

    virtual Ogre::String magicNumberToFileExt(const char *magicNumberPtr, size_t maxbytes) const override;
};

} // namespace image
} // namespace resource
} // namespace d2_hack

#endif /* D2_HACK_RESOURCE_IMAGE_INCLUDE_D2_HACK_RESOURCE_IMAGE_RAW_H */

