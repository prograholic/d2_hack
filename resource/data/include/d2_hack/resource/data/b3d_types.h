#ifndef D2_HACK_RESOURCE_PLUGIN_D2_B3D_TYPES_H
#define D2_HACK_RESOURCE_PLUGIN_D2_B3D_TYPES_H

#include <d2_hack/common/platform.h>

#include <cstdint>
#include <memory>

D2_HACK_DISABLE_WARNING_BEGIN(4251)
#include <OgreVector3.h>
D2_HACK_DISABLE_WARNING_END() // 4251

#include <d2_hack/common/types.h>

namespace d2_hack
{
namespace resource
{
namespace data
{
namespace b3d
{

enum class VisitMode
{
    PreOrder,
    PostOrder
};


namespace block_data
{

static const std::uint32_t EmptyBlock0 = 0;
static const std::uint32_t GroupUnknownBlock2 = 2;
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
static const std::uint32_t SimpleUnknownBlock25 = 25;
static const std::uint32_t SimpleFacesBlock28 = 28;
static const std::uint32_t GroupUnknownBlock29 = 29;
static const std::uint32_t SimplePortalBlock30 = 30;
static const std::uint32_t GroupLightingObjectBlock33 = 33;
static const std::uint32_t SimpleUnknownBlock34 = 34;
static const std::uint32_t SimpleFacesBlock35 = 35;
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
    static constexpr auto Value = EmptyBlock0;
    inline static const common::BoundingSphere boundingSphere = common::InvalidBoundingSphere;

    static const size_t EmptyDataSize0 = 40;
    static const size_t EmptyDataSize1 = 4;

    std::uint8_t emptyData0[EmptyDataSize0];
    Ogre::Real unknown;
    std::uint8_t emptyData1[EmptyDataSize1];
};

struct GroupUnknown2
{
    static constexpr auto Value = GroupUnknownBlock2;

    common::BoundingSphere boundingSphere;
    common::Position unknown0;
    Ogre::Real unknown1;
};

struct GroupRoadInfraObjects4
{
    static constexpr auto Value = GroupRoadInfraObjectsBlock4;

    common::BoundingSphere boundingSphere;
    common::ResourceName name;

    common::ResourceName data;
};

struct GroupObjects5
{
    static constexpr auto Value = GroupObjectsBlock5;

    common::BoundingSphere boundingSphere;
    common::ResourceName name;
};

struct GroupVertex7
{
    static constexpr auto Value = GroupVertexBlock7;

    common::BoundingSphere boundingSphere;
    common::ResourceName name;

    common::SimpleMeshInfo meshInfo;
};

struct Face8
{
    static const std::uint32_t UnknownType0 = 0;
    static const std::uint32_t UnknownType1 = 1;
    static const std::uint32_t FaceIndexType2 = 2;
    static const std::uint32_t FaceIndexType3 = 3;
    static const std::uint32_t UnknownType16 = 16;
    static const std::uint32_t FaceIndexType17 = 17;
    static const std::uint32_t FaceIndexType48 = 48;
    static const std::uint32_t FaceIndexType50 = 50;
    static const std::uint32_t FaceIndexType51 = 51;
    static const std::uint32_t FaceIndexType128 = 128;
    static const std::uint32_t FaceIndexType129 = 129;
    static const std::uint32_t FaceIndexType131 = 131;
    static const std::uint32_t UnknownType144 = 144;
    static const std::uint32_t UnknownType145 = 145;
    static const std::uint32_t FaceIndexType176 = 176;
    static const std::uint32_t FaceIndexType177 = 177;
    static const std::uint32_t FaceIndexType178 = 178;
    static const std::uint32_t FaceIndexType179 = 179;

    std::uint32_t type;
    Ogre::Real unknown0;
    std::uint32_t unknown1;
    std::uint32_t materialIndex;

    struct Unknown177
    {
        Ogre::Real unknown;
    };

    using Unknown177List = std::vector<Unknown177>;

    common::SimpleMeshInfo meshInfo;
    Unknown177List unknown177;
};

typedef std::vector<Face8> Face8List;

struct SimpleFaces8
{
    static constexpr auto Value = SimpleFacesBlock8;

    common::BoundingSphere boundingSphere;

    Face8List faces;
};

struct GroupTrigger9
{
    static constexpr auto Value = GroupTriggerBlock9;

    common::BoundingSphere boundingSphere;

    Ogre::Vector3 unknown;
    Ogre::Real distanceToPlayer;
};

struct GroupLodParameters10
{
    static constexpr auto Value = GroupLodParametersBlock10;

    common::BoundingSphere boundingSphere;

    Ogre::Vector3 unknown;

    Ogre::Real distanceToPlayer;
};

struct GroupUnknown12
{
    static constexpr auto Value = GroupUnknownBlock12;

    common::BoundingSphere boundingSphere;

    Ogre::Real unknown0;
    Ogre::Real unknown1;
    Ogre::Real unknown2;
    Ogre::Real unknown3;

    std::uint32_t unknown4;
    std::uint32_t unknown5;
};

struct SimpleTrigger13
{
    static constexpr auto Value = SimpleTriggerBlock13;

    common::BoundingSphere boundingSphere;

    std::uint32_t unknown0;
    std::uint32_t unknown1;

    std::vector<Ogre::Real> unknown2;
};

struct SimpleUnknown14
{
    static constexpr auto Value = SimpleUnknownBlock14;

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
    static constexpr auto Value = SimpleObjectConnectorBlock18;

    common::BoundingSphere boundingSphere;

    common::ResourceName space;
    common::ResourceName object;
};

struct GroupObjects19
{
    static constexpr auto Value = GroupObjectsBlock19;
    inline static const common::BoundingSphere boundingSphere = common::InvalidBoundingSphere;
};

struct SimpleFlatCollision20
{
    static constexpr auto Value = SimpleFlatCollisionBlock20;

    common::BoundingSphere boundingSphere;

    std::uint32_t unknown0;
    std::uint32_t unknown1;

    std::vector<common::Position> unknown2;
    std::vector<Ogre::Real> unknown3;
};

struct GroupObjects21
{
    static constexpr auto Value = GroupObjectsBlock21;

    common::BoundingSphere boundingSphere;

    std::uint32_t count;
    std::uint32_t unknown;
};

struct SimpleVolumeCollision23
{
    static constexpr auto Value = SimpleVolumeCollisionBlock23;
    inline static const common::BoundingSphere boundingSphere = common::InvalidBoundingSphere;

    std::uint32_t unknown0;
    std::uint32_t surfaceType;

    std::vector<Ogre::Real> unknown1;

    std::vector<common::PositionList> polygons;

};

struct GroupTransformMatrix24
{
    static constexpr auto Value = GroupTransformMatrixBlock24;
    inline static const common::BoundingSphere boundingSphere = common::InvalidBoundingSphere;

    Ogre::Vector3 x;
    Ogre::Vector3 y;
    Ogre::Vector3 z;

    Ogre::Vector3 position;

    std::uint32_t unknown;
};

struct SimpleUnknown25
{
    static constexpr auto Value = SimpleUnknownBlock25;
    inline static const common::BoundingSphere boundingSphere = common::InvalidBoundingSphere;

    Ogre::Real unknown0;
    std::uint32_t unknown1;
    std::uint32_t unknown2;
    common::ResourceName soundName;
    std::array<Ogre::Real, 11> unknown3;
};

struct Face28
{
    static const std::uint32_t Unknown2 = 2;

    struct Unknown
    {
        Ogre::Vector2 unknown;
    };

    using UnknownList = std::vector<Unknown>;

    std::uint32_t type;

    Ogre::Real unknown0;
    std::uint32_t unknown1;
    std::uint32_t materialIndex;

    common::SimpleMeshInfo meshInfo;
    UnknownList unknown;
};

typedef std::vector<Face28> Face28List;

struct SimpleFaces28
{
    static constexpr auto Value = SimpleFacesBlock28;

    common::BoundingSphere boundingSphere;

    Ogre::Vector3 unknown;

    Face28List faces;
};

struct GroupUnknown29
{
    static constexpr auto Value = GroupUnknownBlock29;

    common::BoundingSphere boundingSphere;
    std::uint32_t type;
    std::uint32_t unknown0;

    std::array<Ogre::Real, 8> unknown1;
};

struct SimplePortal30
{
    static constexpr auto Value = SimplePortalBlock30;

    common::BoundingSphere boundingSphere;

    common::ResourceName connectedRoom;

    Ogre::Vector3 leftDown;
    Ogre::Vector3 upRight;
};

struct GroupLightingObjects33
{
    static constexpr auto Value = GroupLightingObjectBlock33;

    common::BoundingSphere boundingSphere;

    std::uint32_t unknown0;
    std::uint32_t unknown1;
    std::uint32_t unknown2;

    Ogre::Vector3 position;

    std::array<Ogre::Real, 12> color;
};

struct SimpleUnknown34
{
    static constexpr auto Value = SimpleUnknownBlock34;

    common::BoundingSphere boundingSphere;
    Ogre::Real unknown0;

    struct Unknown
    {
        Ogre::Real unknown0;
        Ogre::Real unknown1;
        Ogre::Real unknown2;
        Ogre::Real unknown3;
    };

    std::vector<Unknown> data;
};

struct Face35
{
    static const std::uint32_t Indices0 = 0;
    static const std::uint32_t Indices1 = 1;
    static const std::uint32_t Unknown2 = 2;
    static const std::uint32_t Indices3 = 3;
    static const std::uint32_t UnknownType3 = 3;
    static const std::uint32_t Indices16 = 16;
    static const std::uint32_t Indices17 = 17;
    static const std::uint32_t UnknownType48 = 48;
    static const std::uint32_t UnknownType49 = 49;
    static const std::uint32_t UnknownType50 = 50;
    static const std::uint32_t UnknownType51 = 51;

    struct Unknown49
    {
        Ogre::Real unknown;
    };

    using Unknown49List = std::vector<Unknown49>;

    std::uint32_t type;
    Ogre::Real unknown0;
    std::uint32_t unknown1;
    std::uint32_t materialIndex;
    
    common::SimpleMeshInfo meshInfo;
    Unknown49List unknown49;
};

typedef std::vector<Face35> Face35List;

struct SimpleFaces35
{
    static constexpr auto Value = SimpleFacesBlock35;

    static const std::uint32_t Unknown1 = 1;
    static const std::uint32_t Unknown2 = 2;
    static const std::uint32_t IndicesOnly3 = 3;


    common::BoundingSphere boundingSphere;
    std::uint32_t type;
    std::uint32_t materialIndex;

    Face35List faces;
};

struct GroupVertexData37
{
    static constexpr auto Value = GroupIndexAndTexturesBlock37;

    static const std::uint32_t Vertex2 = 2;
    static const std::uint32_t Vertex3 = 3;
    static const std::uint32_t UnknownType258 = 258;
    static const std::uint32_t UnknownType514 = 514;
    static const std::uint32_t UnknownType515 = 515;

    struct Unknown258Or515
    {
        Ogre::Vector2 unknown;
    };

    using Unknown258Or515List = std::vector<Unknown258Or515>;

    struct Unknown514
    {
        Ogre::Vector4 unknown;
    };

    using Unknown514List = std::vector<Unknown514>;

    common::BoundingSphere boundingSphere;
    common::ResourceName name;
    std::uint32_t type;

    common::SimpleMeshInfo meshInfo;
    Unknown258Or515List unknown258Or515;
    Unknown514List unknown514;
};

struct SimpleGeneratedObjects40
{
    static constexpr auto Value = SimpleGeneratedObjectsBlock40;

    common::BoundingSphere boundingSphere;

    common::ResourceName empty;
    common::ResourceName name;

    std::uint32_t type;
    Ogre::Real unknown0;

    std::vector<Ogre::Real> unknown1;
};

} // namespace block_data
} // namespace b3d
} // namespace data
} // namespace resource
} // namespace d2_hack




#endif /* D2_HACK_RESOURCE_PLUGIN_D2_B3D_TYPES_H */
