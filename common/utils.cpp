#include <d2_hack/common/utils.h>


namespace d2_hack
{
namespace common
{

std::string ResourceNameToString(const ResourceName& resName)
{
    const char* data = reinterpret_cast<const char*>(resName.data());
    return std::string(data, data + strnlen(data, resName.size()));
}

bool IsValid(const BoundingSphere& boundingSphere)
{
    return (boundingSphere.radius != 0.0f);
}

size_t Hash<common::BoundingSphere>::operator()(const common::BoundingSphere& data) const noexcept
{
    auto x = std::hash<Ogre::Real>{}(data.center.x);
    auto y = std::hash<Ogre::Real>{}(data.center.y);
    auto z = std::hash<Ogre::Real>{}(data.center.z);
    auto r = std::hash<Ogre::Real>{}(data.radius);

    return x ^ (y > 1) ^ (z > 2) ^ (r > 3);
}

bool operator==(const BoundingSphere& left, const BoundingSphere& right)
{
    return (left.center == right.center) && (left.radius == right.radius);
}

} // namespace common
} // namespace d2_hack
