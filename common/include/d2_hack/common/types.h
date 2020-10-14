#ifndef D2_HACK_COMMON_INCLUDE_D2_HACK_COMMON_TYPES_H
#define D2_HACK_COMMON_INCLUDE_D2_HACK_COMMON_TYPES_H

#include <d2_hack/common/platform.h>

#include <cstdint>
#include <array>
#include <vector>

D2_HACK_DISABLE_WARNING_BEGIN(4251)
#include <OgreVector3.h>
#include <OgreRenderOperation.h>
D2_HACK_DISABLE_WARNING_END() // 4251

namespace d2_hack
{
namespace common
{

const size_t ResourceNameSize = 32;

typedef std::array<std::uint8_t, ResourceNameSize> ResourceName;

typedef std::vector<common::ResourceName> Materials;

typedef std::vector<std::uint8_t> Blob;

struct BoundingSphere
{
    Ogre::Vector3 center;
    Ogre::Real radius;
};


typedef Ogre::Vector3 Position;
typedef std::vector<Position> PositionList;

typedef Ogre::Vector2 TexCoord;
typedef std::vector<TexCoord> TexCoordList;
typedef Ogre::Vector3 Normal;



typedef std::uint32_t Index;
typedef std::vector<Index> IndexList;



struct PositionWithTexCoord
{
    Position position;
    TexCoord texCoord;
};

typedef std::vector<PositionWithTexCoord> PositionWithTexCoordList;



struct PositionWithTexCoordNormal
{
    Position position;
    TexCoord texCoord;
    Normal normal;
};

typedef std::vector<PositionWithTexCoordNormal> PositionWithTexCoordNormalList;



struct PositionWithNormal
{
    Position position;
    Normal normal;
};

typedef std::vector<PositionWithNormal> PositionWithNormalList;



struct IndexWithTexCoord
{
    Index index;
    TexCoord texCoord;
};

typedef std::vector<IndexWithTexCoord> IndexWithTexCoordList;



struct IndexWithPosition
{
    Index index;
    Position position;
};

typedef std::vector<IndexWithPosition> IndexWithPositionList;



struct IndexWithPositionTexCoord
{
    Index index;
    Position position;
    TexCoord texCoord;
};

typedef std::vector<IndexWithPositionTexCoord> IndexWithPositionTexCoordList;

} // namespace common
} // namespace d2_hack






#endif /* D2_HACK_COMMON_INCLUDE_D2_HACK_COMMON_TYPES_H */
