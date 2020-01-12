#ifndef D2_HACK_RESOURCE_PLUGIN_D2_B3D_BLOCK_DATA_DECL_H
#define D2_HACK_RESOURCE_PLUGIN_D2_B3D_BLOCK_DATA_DECL_H

#include <d2_hack/common/platform.h>

#include <cstdint>
#include <memory>

#include <boost/variant/variant.hpp>


D2_HACK_DISABLE_WARNING_BEGIN(4251)
#include <OgreVector3.h>
D2_HACK_DISABLE_WARNING_END() // 4251

#include <d2_hack/common/types.h>

namespace d2_hack
{
namespace codec
{
namespace data
{
namespace b3d
{

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

struct GroupRoadInfraObjects4
{
    common::BoundingSphere boundingSphere;
    common::ResourceName name;

    common::ResourceName data;

    NestedBlockList nestedBlocks;
};

struct GroupObjects5
{
    common::BoundingSphere boundingSphere;
    common::ResourceName name;

    NestedBlockList nestedBlocks;
};


typedef std::vector<common::PositionWithTexCoord> PositionWithTexCoordList;

struct GroupVertex7
{
    common::BoundingSphere boundingSphere;
    common::ResourceName name;

    PositionWithTexCoordList vertices;

    NestedBlockList nestedBlocks;
};

struct Face8
{
    static const std::uint32_t UnknownType0 = 0;
    static const std::uint32_t UnknownType1 = 1;
    static const std::uint32_t FaceIndexType3 = 3;
    static const std::uint32_t UnknownType16 = 16;
    static const std::uint32_t FaceIndexType48 = 48;
    static const std::uint32_t FaceIndexType50 = 50;
    static const std::uint32_t FaceIndexType129 = 129;
    static const std::uint32_t UnknownType144 = 144;
    static const std::uint32_t FaceIndexType176 = 176;
    static const std::uint32_t FaceIndexType178 = 178;

    typedef boost::variant<
        common::Index,
        common::IndexWithTexCoord,
        common::IndexWithPosition,
        common::IndexWithPositionTexCoord
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
    common::BoundingSphere boundingSphere;

    Face8List faces;
};

struct GroupTrigger9
{
    common::BoundingSphere boundingSphere;

    Ogre::Vector3 unknown;
    Ogre::Real distanceToPlayer;

    NestedBlockList nestedBlocks;
};

struct GroupLodParameters10
{
    common::BoundingSphere boundingSphere;

    Ogre::Vector3 unknown;

    Ogre::Real distanceToPlayer;

    NestedBlockList nestedBlocks;
};

struct GroupUnknown12
{
    common::BoundingSphere boundingSphere;

    Ogre::Real unknown0;
    Ogre::Real unknown1;
    Ogre::Real unknown2;
    Ogre::Real unknown3;

    std::uint32_t unknown4;
    std::uint32_t unknown5;

    NestedBlockList nestedBlocks;
};

struct SimpleTrigger13
{
    common::BoundingSphere boundingSphere;

    std::uint32_t unknown0;
    std::uint32_t unknown1;

    std::vector<Ogre::Real> unknown2;
};

struct SimpleObjectConnector18
{
    common::BoundingSphere boundingSphere;

    common::ResourceName space;
    common::ResourceName object;
};

struct GroupObjects19
{
    NestedBlockList nestedBlocks;
};

struct SimpleFlatCollision20
{
    common::BoundingSphere boundingSphere;

    std::uint32_t unknown0;
    std::uint32_t unknown1;

    std::vector<Ogre::Vector3> a;
    std::vector<Ogre::Real> b;
};

struct GroupObjects21
{
    common::BoundingSphere boundingSphere;

    std::uint32_t count;
    std::uint32_t unknown;

    NestedBlockList nestedBlocks;
};

struct SimpleVolumeCollision23
{
    static const std::uint32_t UnknownType0 = 0;


    struct Unknown0
    {
        std::vector<Ogre::Vector3> unknown;
    };

    typedef boost::variant <
        Unknown0
    > CollisionEntry;

    typedef std::vector<CollisionEntry> CollisionEntries;

    std::uint32_t unknown0;
    std::uint32_t unknown1;

    std::uint32_t type;

    Ogre::Real unknown2;
    Ogre::Real unknown3;
    Ogre::Real unknown4;
    
    CollisionEntries collisionEntries;
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

struct Face28Entry
{
    static const std::uint32_t Unknown2 = 2;

    std::uint32_t type;

    Ogre::Real unknown0;
    std::uint32_t unknown1;
    std::uint32_t unknown2;

    struct Unknown
    {
        Ogre::Real unknown0;
        Ogre::Real unknown1;
        Ogre::Real unknown2;
        Ogre::Real unknown3;
    };

    typedef boost::variant<
        Unknown
    > FaceData;

    std::vector<FaceData> faceData;
};

typedef std::vector<Face28Entry> Faces28Entries;

struct SimpleFaces28
{
    common::BoundingSphere boundingSphere;

    Ogre::Vector3 unknown;

    Faces28Entries facesEntries;
};

struct SimplePortal30
{
    common::BoundingSphere boundingSphere;

    common::ResourceName connectedRoom;

    Ogre::Vector3 leftDown;
    Ogre::Vector3 upRight;
};

struct GroupLightingObjects33
{
    common::BoundingSphere boundingSphere;

    std::uint32_t unknown0;
    std::uint32_t unknown1;
    std::uint32_t unknown2;

    Ogre::Vector3 position;

    std::array<Ogre::Real, 12> color;

    NestedBlockList nestedBlocks;
};

struct Mesh35
{
    static const std::uint32_t Indices0 = 0;
    static const std::uint32_t Indices1 = 1;
    static const std::uint32_t Indices3 = 3;
    static const std::uint32_t Indices16 = 16;
    static const std::uint32_t UnknownType48 = 48;
    static const std::uint32_t UnknownType49 = 49;
    static const std::uint32_t UnknownType50 = 50;

    struct Unknown49
    {
        std::uint32_t unknown0;
        Ogre::Real unknown1;
    };

    typedef boost::variant<
        common::Index,
        common::IndexWithPosition,
        Unknown49,
        common::IndexWithPositionTexCoord
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
    

    common::BoundingSphere boundingSphere;
    std::uint32_t type;
    std::uint32_t meshIndex;

    Mesh35List meshList;
};

struct GroupVertexData37
{
    static const std::uint32_t Vertex2 = 2;
    static const std::uint32_t Vertex3 = 3;
    static const std::uint32_t UnknownType514 = 514;

    struct Unknown514
    {
        Ogre::Vector3 unknown0;
        Ogre::Vector3 unknown1;
        Ogre::Vector3 unknown2;
        Ogre::Vector3 unknown3;
    };

    typedef boost::variant<
        common::PositionWithTexCoordNormal,
        common::PositionWithNormal,
        Unknown514
    > VertexData;

    typedef std::vector<VertexData> VertexDataList;

    common::BoundingSphere boundingSphere;
    common::ResourceName name;
    std::uint32_t type;

    VertexDataList vertexDataList;

    NestedBlockList nestedBlocks;
};

struct SimpleGeneratedObjects40
{
    common::BoundingSphere boundingSphere;

    common::ResourceName empty;
    common::ResourceName name;

    std::uint32_t type;
    Ogre::Real unknown0;

    std::vector<Ogre::Real> unknown1;
};


typedef boost::variant<
    Empty0,
    //GroupRoadInfraObjects4,
    GroupObjects5,
    GroupVertex7,
    SimpleFaces8,
    GroupTrigger9,
    GroupLodParameters10,
    GroupUnknown12,
    SimpleTrigger13,
    SimpleObjectConnector18,
    GroupObjects19,
    SimpleFlatCollision20,
    GroupObjects21,
    SimpleVolumeCollision23,
    GroupTransformMatrix24,
    SimpleFaces28,
    SimplePortal30,
    GroupLightingObjects33,
    SimpleFaceData35,
    GroupVertexData37,
    SimpleGeneratedObjects40
> Data;


} // namespace block_data

} // namespace b3d
} // namespace data
} // namespace codec
} // namespace d2_hack




#endif /* D2_HACK_RESOURCE_PLUGIN_D2_B3D_BLOCK_DATA_DECL_H */
