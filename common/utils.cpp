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

ResourceName StringToResourceName(const std::string& name)
{
    ResourceName res{};
    if (name.size() > res.size())
    {
        OGRE_EXCEPT(Ogre::Exception::ERR_INVALID_STATE, "Incorrect string: \"" + name + "\"", "StringToResourceName");
    }

    std::copy(name.begin(), name.end(), res.begin());

    return res;
}

bool IsValid(const BoundingSphere& boundingSphere)
{
    return (boundingSphere.radius != 0.0f);
}

size_t Hash<common::BoundingSphere>::operator()(const common::BoundingSphere& data) const noexcept
{
    auto c = Hash<Ogre::Vector3>{}(data.center);
    auto r = std::hash<Ogre::Real>{}(data.radius);

    return c ^ (r >> 3);
}

bool operator==(const BoundingSphere& left, const BoundingSphere& right)
{
    return (left.center == right.center) && (left.radius == right.radius);
}

size_t Hash<Ogre::Vector3>::operator()(const Ogre::Vector3& data) const noexcept
{
    auto x = std::hash<Ogre::Real>{}(data.x);
    auto y = std::hash<Ogre::Real>{}(data.y);
    auto z = std::hash<Ogre::Real>{}(data.z);

    return x ^ (y >> 1) ^ (z >> 2);
}

} // namespace common
} // namespace d2_hack
