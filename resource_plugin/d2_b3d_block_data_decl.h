#ifndef D2_HACK_RESOURCE_PLUGIN_D2_B3D_BLOCK_DATA_DECL_H
#define D2_HACK_RESOURCE_PLUGIN_D2_B3D_BLOCK_DATA_DECL_H

#include <cstdint>
#include <memory>

#include <boost/variant/variant.hpp>

#include "d2_hack_platform_support.h"

D2_HACK_DISABLE_WARNING_BEGIN(4251)

#include <OgreVector3.h>

D2_HACK_DISABLE_WARNING_END() // 4251

namespace b3d
{

const size_t StringSize = 32;
typedef std::array<char, StringSize> ResourceName;

struct Block;
typedef std::shared_ptr<Block> NestedBlock;
typedef std::vector<NestedBlock> NestedBlockList;

namespace block_data
{



struct Empty0
{
    static const size_t EmptyDataSize0 = 40;
    static const size_t EmptyDataSize1 = 4;

    std::uint8_t emptyData0[EmptyDataSize0];
    Ogre::Real unknown;
    std::uint8_t emptyData1[EmptyDataSize1];
};

struct GroupObjects5
{
    Ogre::Vector3 center;
    Ogre::Real boundingSphereRadius;

    ResourceName name;

    NestedBlockList nestedBlocks;
};

struct GroupVertex7
{

    NestedBlockList nestedBlocks;
};

struct GroupLodParameters10
{
    Ogre::Vector3 center;
    Ogre::Real boundingSphereRadius;

    Ogre::Vector3 unknown;

    Ogre::Real distanceToPlayer;

    NestedBlockList nestedBlocks;
};

struct GroupTransformMatrix24
{
    Ogre::Vector3 x;
    Ogre::Vector3 y;
    Ogre::Vector3 z;

    Ogre::Vector3 position;
    
    std::uint32_t unknown;

    NestedBlockList nestedBlocks;
};

struct MayBeTextureIndex
{
    std::uint32_t unknown0;
    Ogre::Real unknown1;
    std::array<std::uint32_t, 6> mayBeIndices;
};

typedef std::vector<MayBeTextureIndex> MayBeTextureIndexList;

struct GroupIndexAndTextures35
{
    Ogre::Vector3 mayBeCenter;
    Ogre::Real mayBeBoundingSphereRadius;
    std::uint32_t unknown0;
    std::uint32_t unknown1;

    MayBeTextureIndexList mayBeTextureIndexList;
};

struct MayBePositionAndNormal
{
    Ogre::Vector3 mayBePosition;
    Ogre::Vector3 mayBeNormal;
    Ogre::Real unknown0;
    Ogre::Real unknown1;
};

typedef std::vector< MayBePositionAndNormal> MayBePositionAndNormalList;

struct GroupIndexAndTextures37
{
    static const std::uint32_t Unknown2 = 2;

    Ogre::Vector3 mayBeCenter;
    Ogre::Real mayBeBoundingSphereRadius;
    ResourceName mayBeName;
    std::uint32_t unknownIf2ThenUseUnknown0And1;

    MayBePositionAndNormalList mayBePositionAndNormalList;

    NestedBlockList nestedBlocks;
};

typedef boost::variant<
    Empty0,
    GroupObjects5,
    GroupVertex7,
    GroupLodParameters10,
    GroupTransformMatrix24,
    GroupIndexAndTextures35,
    GroupIndexAndTextures37
> Data;


} // namespace block_data

} // namespace b3d



#endif /* D2_HACK_RESOURCE_PLUGIN_D2_B3D_BLOCK_DATA_DECL_H */
