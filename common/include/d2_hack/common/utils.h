#ifndef D2_HACK_COMMON_INCLUDE_D2_HACK_COMMON_UTILS_H
#define D2_HACK_COMMON_INCLUDE_D2_HACK_COMMON_UTILS_H

#include <d2_hack/common/types.h>

namespace d2_hack
{
namespace common
{
    
std::string ResourceNameToString(const ResourceName& resName);


bool IsValid(const BoundingSphere& boundingSphere);

template <typename T>
struct Hash;

template <>
struct Hash<common::BoundingSphere>
{
    size_t operator()(const common::BoundingSphere& data) const noexcept;
};

bool operator==(const BoundingSphere& left, const BoundingSphere& right);



template <>
struct Hash<Ogre::Vector3>
{
    size_t operator()(const Ogre::Vector3& data) const noexcept;
};


} // namespace common
} // namespace d2_hack






#endif /* D2_HACK_COMMON_INCLUDE_D2_HACK_COMMON_UTILS_H */
