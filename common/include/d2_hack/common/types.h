#ifndef D2_HACK_COMMON_INCLUDE_D2_HACK_COMMON_TYPES_H
#define D2_HACK_COMMON_INCLUDE_D2_HACK_COMMON_TYPES_H

#include <d2_hack/common/platform.h>

#include <cstdint>
#include <array>
#include <vector>

D2_HACK_DISABLE_WARNING_BEGIN(4251)
#include <OgreVector3.h>
D2_HACK_DISABLE_WARNING_END() // 4251

namespace d2_hack
{
namespace common
{

const size_t ResourceNameSize = 32;

typedef std::array<std::uint8_t, ResourceNameSize> ResourceName;

typedef std::vector<std::uint8_t> Blob;

struct BoundingSphere
{
    Ogre::Vector3 center;
    Ogre::Real radius;
};


typedef Ogre::Vector3 Position;
typedef Ogre::Vector2 TexCoord;
typedef Ogre::Vector3 Normal;
typedef std::uint32_t Index;

struct PositionWithTexCoord
{
    Position position;
    TexCoord texCoord;
};

struct PositionWithTexCoordNormal
{
    Position position;
    TexCoord texCoord;
    Normal normal;
};

struct PositionWithNormal
{
    Position position;
    Normal normal;
};

struct IndexWithTexCoord
{
    Index index;
    TexCoord texCoord;
};

struct IndexWithPosition
{
    Index index;
    Position position;
};

struct IndexWithPositionTexCoord
{
    Index index;
    PositionWithTexCoord positionWithTexCoord;
};




} // namespace common
} // namespace d2_hack






#endif /* D2_HACK_COMMON_INCLUDE_D2_HACK_COMMON_TYPES_H */
