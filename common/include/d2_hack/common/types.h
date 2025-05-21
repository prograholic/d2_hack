#ifndef D2_HACK_COMMON_INCLUDE_D2_HACK_COMMON_TYPES_H
#define D2_HACK_COMMON_INCLUDE_D2_HACK_COMMON_TYPES_H

#include <d2_hack/common/platform.h>

#include <cstdint>
#include <array>
#include <vector>

D2_HACK_DISABLE_WARNING_BEGIN(4251)
#include <Ogre.h>
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

const BoundingSphere InvalidBoundingSphere{Ogre::Vector3::ZERO, 0.0f};


typedef Ogre::Vector3 Position;
typedef std::vector<Position> PositionList;

typedef Ogre::Vector2 TexCoord;
typedef std::vector<TexCoord> TexCoordList;

typedef Ogre::Vector3 Normal;
typedef std::vector<Normal> NormalList;


typedef std::uint32_t Index;
typedef std::vector<Index> IndexList;


struct SimpleMeshInfo
{
    PositionList positions;
    TexCoordList texCoords;
    NormalList normals;
    IndexList indices;
};


// NOTE: Should be in sync with resource group in data/config/resources(_d).cfg.in
const std::string DefaultResourceGroup = "D2";


} // namespace common
} // namespace d2_hack






#endif /* D2_HACK_COMMON_INCLUDE_D2_HACK_COMMON_TYPES_H */
