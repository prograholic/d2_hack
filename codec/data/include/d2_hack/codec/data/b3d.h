#ifndef D2_HACK_CODEC_DATA_INCLUDE_D2_HACK_CODEC_DATA_B3D_H
#define D2_HACK_CODEC_DATA_INCLUDE_D2_HACK_CODEC_DATA_B3D_H

#include <d2_hack/common/platform.h>

#include <ostream>

#include <d2_hack/common/types.h>
#include <d2_hack/common/reader.h>

namespace d2_hack
{
namespace codec
{
namespace data
{


class B3dVisitorInterface
{
public:
    virtual ~B3dVisitorInterface() = default;

    virtual void VisitVector3(const Ogre::Vector3& /* vector */)
    {
    }

    virtual void VisitBoundingSphere(const common::BoundingSphere& /* boundingSphere */)
    {
    }
    
    virtual void VisitBlockSeparator(std::uint32_t /* blockSeparator */)
    {
    }

    virtual void VisitBlockHeader(const common::ResourceName& /* name */, std::uint32_t /* type */)
    {
    }

    virtual void VisitBlock(const common::ResourceName& /* name */, std::uint32_t /* type */)
    {
    }

    virtual void VisitNestedBlockHeader(std::uint32_t /* nestedBlockCount */)
    {
    }

    virtual std::ostream* GetDebugOutStream()
    {
        return nullptr;
    }
};









} // namespace image
} // namespace codec
} // namespace d2_hack

#endif /* D2_HACK_CODEC_DATA_INCLUDE_D2_HACK_CODEC_DATA_B3D_H */
