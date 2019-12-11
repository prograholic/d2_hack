#ifndef D2_HACK_COMMON_INCLUDE_D2_HACK_COMMON_TYPES_H
#define D2_HACK_COMMON_INCLUDE_D2_HACK_COMMON_TYPES_H

#include <d2_hack/common/platform.h>

#include <cstdint>
#include <array>
#include <vector>

namespace d2_hack
{
namespace common
{

const size_t ResourceNameSize = 32;

typedef std::array<std::uint8_t, ResourceNameSize> ResourceName;

typedef std::vector<std::uint8_t> Blob;

} // namespace common
} // namespace d2_hack






#endif /* D2_HACK_COMMON_INCLUDE_D2_HACK_COMMON_TYPES_H */
