#ifndef D2_HACK_RESOURCE_PLUGIN_D2_B3D_BLOCK_DATA_DECL_H
#define D2_HACK_RESOURCE_PLUGIN_D2_B3D_BLOCK_DATA_DECL_H

#include <d2_hack/common/platform.h>

#include <cstdint>
#include <memory>

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
namespace block_data
{

static const std::uint32_t EmptyBlock0 = 0;
static const std::uint32_t GroupRoadInfraObjectsBlock4 = 4;
static const std::uint32_t GroupObjectsBlock5 = 5;
static const std::uint32_t GroupVertexBlock7 = 7;
static const std::uint32_t SimpleFacesBlock8 = 8;
static const std::uint32_t GroupTriggerBlock9 = 9;
static const std::uint32_t GroupLodParametersBlock10 = 10;
static const std::uint32_t GroupUnknownBlock12 = 12;
static const std::uint32_t SimpleTriggerBlock13 = 13;
static const std::uint32_t SimpleUnknownBlock14 = 14;
static const std::uint32_t SimpleObjectConnectorBlock18 = 18;
static const std::uint32_t GroupObjectsBlock19 = 19;
static const std::uint32_t SimpleFlatCollisionBlock20 = 20;
static const std::uint32_t GroupObjectsBlock21 = 21;
static const std::uint32_t SimpleVolumeCollisionBlock23 = 23;
static const std::uint32_t GroupTransformMatrixBlock24 = 24;
static const std::uint32_t SimpleFacesBlock28 = 28;
static const std::uint32_t SimplePortalBlock30 = 30;
static const std::uint32_t GroupLightingObjectBlock33 = 33;
static const std::uint32_t SimpleFaceDataBlock35 = 35;
static const std::uint32_t GroupIndexAndTexturesBlock37 = 37;
static const std::uint32_t SimpleGeneratedObjectsBlock40 = 40;

static const std::uint32_t MaxBlockId = 40;

struct BlockHeader
{
    common::ResourceName name;
    std::uint32_t type;
};

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

    //NestedBlockList nestedBlocks;
};

struct GroupObjects5
{
    common::BoundingSphere boundingSphere;
    common::ResourceName name;

    //NestedBlockList nestedBlocks;
};

struct GroupVertex7
{
    common::BoundingSphere boundingSphere;
    common::ResourceName name;

    //common::PositionWithTexCoordList vertices;

    //NestedBlockList nestedBlocks;
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

    std::uint32_t type;
    Ogre::Real unknown0;
    std::uint32_t unknown1;
    std::uint32_t unknown2;

#if 0
    typedef boost::variant<
        common::Index,
        common::IndexWithTexCoord,
        common::IndexWithPosition,
        common::IndexWithPositionTexCoord
    > FaceData;
    typedef std::vector<FaceData> FaceDataList;

    FaceDataList faceDataList;
#endif //0
};

//typedef std::vector<Face8> Face8List;

struct SimpleFaces8
{
    common::BoundingSphere boundingSphere;

    //Face8List faces;
};

struct GroupTrigger9
{
    common::BoundingSphere boundingSphere;

    Ogre::Vector3 unknown;
    Ogre::Real distanceToPlayer;

    //NestedBlockList nestedBlocks;
};

struct GroupLodParameters10
{
    common::BoundingSphere boundingSphere;

    Ogre::Vector3 unknown;

    Ogre::Real distanceToPlayer;

    //NestedBlockList nestedBlocks;
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

    //NestedBlockList nestedBlocks;
};

struct SimpleTrigger13
{
    common::BoundingSphere boundingSphere;

    std::uint32_t unknown0;
    std::uint32_t unknown1;

    //std::vector<Ogre::Real> unknown2;
};

struct SimpleUnknown14
{
    common::BoundingSphere boundingSphere;

    std::uint32_t unknown0;
    std::uint32_t unknown1;

    Ogre::Real unknown2;
    Ogre::Real unknown3;
    Ogre::Real unknown4;
    Ogre::Real unknown5;
    Ogre::Real unknown6;
};

struct SimpleObjectConnector18
{
    common::BoundingSphere boundingSphere;

    common::ResourceName space;
    common::ResourceName object;
};

struct GroupObjects19
{
    //NestedBlockList nestedBlocks;
};

struct SimpleFlatCollision20
{
    common::BoundingSphere boundingSphere;

    std::uint32_t unknown0;
    std::uint32_t unknown1;

    //PositionList a;
    //std::vector<Ogre::Real> b;
};

struct GroupObjects21
{
    common::BoundingSphere boundingSphere;

    std::uint32_t count;
    std::uint32_t unknown;

    //NestedBlockList nestedBlocks;
};

struct SimpleVolumeCollision23
{
    static const std::uint32_t UnknownType0 = 0;

#if 0
    struct Unknown0
    {
        common::PositionList unknown;
    };

    typedef boost::variant <
        Unknown0
    > CollisionEntry;

    typedef std::vector<CollisionEntry> CollisionEntries;
#endif //0

    std::uint32_t unknown0;
    std::uint32_t unknown1;

    std::uint32_t type;

    Ogre::Real unknown2;
    Ogre::Real unknown3;
    Ogre::Real unknown4;

    //CollisionEntries collisionEntries;
};

struct GroupTransformMatrix24
{
    Ogre::Vector3 x;
    Ogre::Vector3 y;
    Ogre::Vector3 z;

    Ogre::Vector3 position;

    std::uint32_t unknown;

    //NestedBlockList nestedBlocks;
};

struct Face28Entry
{
    static const std::uint32_t Unknown2 = 2;

    struct Unknown
    {
        Ogre::Real unknown0;
        Ogre::Real unknown1;
        Ogre::Real unknown2;
        Ogre::Real unknown3;
    };

#if 0
    typedef boost::variant<
        Unknown
    > FaceData;

    std::vector<FaceData> faceData;
#endif //0

    std::uint32_t type;

    Ogre::Real unknown0;
    std::uint32_t unknown1;
    std::uint32_t unknown2;
};

//typedef std::vector<Face28Entry> Faces28Entries;

struct SimpleFaces28
{
    common::BoundingSphere boundingSphere;

    Ogre::Vector3 unknown;

    //Faces28Entries facesEntries;
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

    //NestedBlockList nestedBlocks;
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
    static const std::uint32_t UnknownType51 = 51;

    struct Unknown49
    {
        common::Index index;
        Ogre::Real unknown;
    };

#if 0
    typedef boost::variant<
        common::Index,
        common::IndexWithPosition,
        Unknown49,
        common::IndexWithPositionTexCoord
    > MeshData;
    typedef std::vector<MeshData> MeshDataList;
#endif //0

    std::uint32_t type;
    Ogre::Real unknown0;
    std::uint32_t unknown1;
    std::uint32_t materialIndex;
    

    //MeshDataList meshDataList;
};

//typedef std::vector<Mesh35> Mesh35List;

struct SimpleFaceData35
{
    static const std::uint32_t Unknown1 = 1;
    static const std::uint32_t Unknown2 = 2;
    static const std::uint32_t IndicesOnly3 = 3;


    common::BoundingSphere boundingSphere;
    std::uint32_t type;
    std::uint32_t materialIndex;

    //Mesh35List meshList;
};

struct GroupVertexData37
{
    static const std::uint32_t Vertex2 = 2;
    static const std::uint32_t Vertex3 = 3;
    static const std::uint32_t UnknownType258 = 258;
    static const std::uint32_t UnknownType514 = 514;

    struct Unknown258
    {
        Ogre::Vector3 unknown0;
        Ogre::Vector3 unknown1;
        Ogre::Vector3 unknown2;
        Ogre::Real unknown3;
    };

    struct Unknown514
    {
        Ogre::Vector3 unknown0;
        Ogre::Vector3 unknown1;
        Ogre::Vector3 unknown2;
        Ogre::Vector3 unknown3;
    };

#if 0
    typedef boost::variant<
        common::PositionWithTexCoordNormal,
        common::PositionWithNormal,
        Unknown514
    > VertexData;

    typedef std::vector<VertexData> VertexDataList;
#endif //0

    common::BoundingSphere boundingSphere;
    common::ResourceName name;
    std::uint32_t type;

    //VertexDataList vertexDataList;

    //NestedBlockList nestedBlocks;
};

struct SimpleGeneratedObjects40
{
    common::BoundingSphere boundingSphere;

    common::ResourceName empty;
    common::ResourceName name;

    std::uint32_t type;
    Ogre::Real unknown0;

    //std::vector<Ogre::Real> unknown1;
};

} // namespace block_data
} // namespace b3d
} // namespace data
} // namespace codec
} // namespace d2_hack




#endif /* D2_HACK_RESOURCE_PLUGIN_D2_B3D_BLOCK_DATA_DECL_H */
