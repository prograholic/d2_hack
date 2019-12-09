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

struct GroupTransformMatrix24
{
    Ogre::Vector3 x;
    Ogre::Vector3 y;
    Ogre::Vector3 z;

    Ogre::Vector3 position;
    
    std::uint32_t unknown;

    NestedBlockList nestedBlocks;
};

typedef boost::variant<
    Empty0,
    GroupObjects5,
    GroupTransformMatrix24
> Data;


} // namespace block_data

} // namespace b3d



#endif /* D2_HACK_RESOURCE_PLUGIN_D2_B3D_BLOCK_DATA_DECL_H */
