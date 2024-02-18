#ifndef D2_HACK_RESOURCE_TECH_INCLUDE_D2_HACK_RESOURCE_TECH_TECH_TYPES_H
#define D2_HACK_RESOURCE_TECH_INCLUDE_D2_HACK_RESOURCE_TECH_TECH_TYPES_H

#include <d2_hack/common/platform.h>

#include <vector>
#include <cstdint>

namespace d2_hack
{
namespace resource
{
namespace tech
{

struct TechHeader
{
    std::uint32_t length;
    std::vector<char> filename;

    std::uint32_t timeUnknown;

};

struct Tech
{
    TechHeader header;

};



} // namespace tech
} // namespace resource
} // namespace d2_hack

#endif /* D2_HACK_RESOURCE_TECH_INCLUDE_D2_HACK_RESOURCE_TECH_TECH_TYPES_H */

