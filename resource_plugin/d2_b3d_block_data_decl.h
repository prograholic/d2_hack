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

struct VertexEntry
{
    Ogre::Vector3 point;
    Ogre::Vector2 textureCoord;
};

typedef std::vector<VertexEntry> Vertices;

struct GroupVertex7
{
    Ogre::Vector3 center;
    Ogre::Real boundingSphereRadius;
    ResourceName name;

    Vertices vertices;

    NestedBlockList nestedBlocks;
};

struct Face8
{
    static const std::uint32_t FaceIndexType129 = 129;
    static const std::uint32_t UnknownType144 = 144;

    struct FaceIndex
    {
        std::uint32_t faceIndex;
    };

    struct Unknown144
    {
        std::uint32_t unknown;
    };

    typedef boost::variant<
        FaceIndex,
        Unknown144
    > FaceData;
    typedef std::vector<FaceData> FaceDataList;


    std::uint32_t type;
    Ogre::Real unknown0;
    std::uint32_t unknown1;
    std::uint32_t unknown2;

    FaceDataList faceDataList;
};

typedef std::vector<Face8> Face8List;

struct SimpleFaces8
{
    Ogre::Vector3 center;
    Ogre::Real boundingSphereRadius;

    Face8List faces;
};

struct GroupTrigger9
{
    Ogre::Vector3 center;
    Ogre::Real boundingSphereRadius;

    Ogre::Vector3 unknown;
    Ogre::Real distanceToPlayer;

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

struct SimpleTrigger13
{
    Ogre::Vector3 center;
    Ogre::Real boundingSphereRadius;

    std::uint32_t unknown0;
    std::uint32_t unknown1;

    std::vector<Ogre::Real> unknown2;
};

struct SimpleFlatCollision20
{
    Ogre::Vector3 center;
    Ogre::Real boundingSphereRadius;

    std::uint32_t unknown0;
    std::uint32_t unknown1;

    std::vector<Ogre::Vector3> a;
    std::vector<Ogre::Real> b;
};

struct GroupObjects21
{
    Ogre::Vector3 center;
    Ogre::Real boundingSphereRadius;

    std::uint32_t count;
    std::uint32_t unknown;

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

struct GroupLightingObjects33
{
    Ogre::Vector3 center;
    Ogre::Real boundingSphereRadius;

    std::uint32_t unknown0;
    std::uint32_t unknown1;
    std::uint32_t unknown2;

    Ogre::Vector3 position;

    std::array<Ogre::Real, 12> color;

    NestedBlockList nestedBlocks;
};

struct Mesh35
{
    static const std::uint32_t Indices1 = 1;
    static const std::uint32_t Indices16 = 16;
    static const std::uint32_t UnknownType48 = 48;
    static const std::uint32_t UnknownType49 = 49;
    static const std::uint32_t UnknownType50 = 50;

    struct Indices
    {
        std::uint32_t index;
    };

    struct Unknown48
    {
        std::uint32_t faceIndex;
        Ogre::Vector3 position;
    };

    struct Unknown49
    {
        std::uint32_t unknown0;
        Ogre::Real unknown1;
    };

    struct Unknown50
    {
        std::uint32_t faceIndex;
        Ogre::Vector3 position;
        Ogre::Vector2 texCoord;
    };

    typedef boost::variant<
        Indices,
        Unknown48,
        Unknown49,
        Unknown50
    > MeshData;
    typedef std::vector<MeshData> MeshDataList;

    std::uint32_t type;
    Ogre::Real unknown0;
    std::uint32_t unknown1;
    std::uint32_t materialIndex;

    MeshDataList meshDataList;
};

typedef std::vector<Mesh35> Mesh35List;

struct SimpleFaceData35
{
    static const std::uint32_t Unknown1 = 1;
    static const std::uint32_t Unknown2 = 2;
    static const std::uint32_t IndicesOnly3 = 3;
    

    Ogre::Vector3 center;
    Ogre::Real boundingSphereRadius;
    std::uint32_t type;
    std::uint32_t meshIndex;

    Mesh35List meshList;
};

struct GroupVertexData37
{
    static const std::uint32_t Vertex2 = 2;
    static const std::uint32_t UnknownType3 = 3;


    struct Vertex
    {
        Ogre::Vector3 position;
        Ogre::Vector2 texCoord;
        Ogre::Vector3 normal;
    };

    struct Unknown3
    {
        Ogre::Vector3 unknown0;
        Ogre::Vector3 unknown1;
    };

    typedef boost::variant<
        Vertex,
        Unknown3
    > VertexData;

    typedef std::vector<VertexData> VertexDataList;



    Ogre::Vector3 center;
    Ogre::Real boundingSphereRadius;
    ResourceName name;
    std::uint32_t type;

    VertexDataList vertexDataList;

    NestedBlockList nestedBlocks;
};

typedef boost::variant<
    Empty0,
    GroupObjects5,
    GroupVertex7,
    SimpleFaces8,
    GroupTrigger9,
    GroupLodParameters10,
    SimpleTrigger13,
    SimpleFlatCollision20,
    GroupObjects21,
    GroupTransformMatrix24,
    GroupLightingObjects33,
    SimpleFaceData35,
    GroupVertexData37
> Data;


} // namespace block_data

} // namespace b3d



#endif /* D2_HACK_RESOURCE_PLUGIN_D2_B3D_BLOCK_DATA_DECL_H */
