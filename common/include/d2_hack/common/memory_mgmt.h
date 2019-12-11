#ifndef D2_HACK_COMMON_INCLUDE_D2_HACK_COMMON_MEMORY_MGMT_H
#define D2_HACK_COMMON_INCLUDE_D2_HACK_COMMON_MEMORY_MGMT_H

#include <d2_hack/common/platform.h>

#include <OgreMemoryAllocatorConfig.h>

namespace d2_hack
{
namespace common
{

struct Deleter
{
    template <typename T>
    void operator()(T* ptr)
    {
        OGRE_DELETE ptr;
    }
};

} // namespace common
} // namespace d2_hack

#endif /* D2_HACK_COMMON_INCLUDE_D2_HACK_COMMON_MEMORY_MGMT_H */
