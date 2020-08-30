#ifndef D2_HACK_RESOURCE_DATA_B3D_READER_H
#define D2_HACK_RESOURCE_DATA_B3D_READER_H

#include <d2_hack/common/platform.h>

#include <iosfwd>
#include <cstdint>
#include <vector>
#include <array>

#include <OgreDataStream.h>

#include <d2_hack/common/types.h>

#include <d2_hack/resource/data/b3d_block_data_decl.h>

namespace d2_hack
{
namespace resource
{
namespace data
{
namespace b3d
{

typedef std::vector<common::ResourceName> Materials;


class B3dListenerInterface
{
public:
    virtual ~B3dListenerInterface() = default;

    virtual void OnBlockBegin(const block_data::BlockHeader& /* blockHeader */) = 0;

    virtual void OnBlockEnd(const block_data::BlockHeader& /* blockHeader */) = 0;

    virtual void OnNestedBlockBegin(std::uint32_t /* nestedBlockNumber */) = 0;

    virtual void OnNestedBlockEnd(std::uint32_t /* nestedBlockNumber */) = 0;

    virtual void OnMaterials(Materials&& /* materials */) = 0;

    virtual void OnBlock(const block_data::Empty0& /* block */) = 0;

    virtual void OnBlock(const block_data::GroupRoadInfraObjects4& /* block */) = 0;

    virtual void OnBlock(const block_data::GroupObjects5& /* block */) = 0;

    virtual void OnBlock(const block_data::GroupVertex7& /* block */) = 0;

    virtual void OnBlock(const block_data::SimpleFaces8& /* block */) = 0;

    virtual void OnBlock(const block_data::GroupTrigger9& /* block */) = 0;

    virtual void OnBlock(const block_data::GroupLodParameters10& /* block */) = 0;

    virtual void OnBlock(const block_data::GroupUnknown12& /* block */) = 0;

    virtual void OnBlock(const block_data::SimpleTrigger13& /* block */) = 0;

    virtual void OnBlock(const block_data::SimpleUnknown14& /* block */) = 0;

    virtual void OnBlock(const block_data::SimpleObjectConnector18& /* block */) = 0;

    virtual void OnBlock(const block_data::GroupObjects19& /* block */) = 0;

    virtual void OnBlock(const block_data::SimpleFlatCollision20& /* block */) = 0;

    virtual void OnBlock(const block_data::GroupObjects21& /* block */) = 0;

    virtual void OnBlock(const block_data::SimpleVolumeCollision23& /* block */) = 0;

    virtual void OnBlock(const block_data::GroupTransformMatrix24& /* block */) = 0;

    virtual void OnBlock(const block_data::SimpleFaces28& /* block */) = 0;

    virtual void OnBlock(const block_data::SimplePortal30& /* block */) = 0;

    virtual void OnBlock(const block_data::GroupLightingObjects33& /* block */) = 0;

    virtual void OnBlock(const block_data::SimpleFaceData35& /* block */) = 0;

    virtual void OnBlock(const block_data::GroupVertexData37& /* block */) = 0;

    virtual void OnBlock(const block_data::SimpleGeneratedObjects40& /* block */) = 0;

    virtual void OnData(common::PositionWithTexCoordList&& /* data */) = 0;

    virtual void OnData(block_data::Face8&& /* data */) = 0;

    virtual void OnData(common::IndexList&& /* data */) = 0;

    virtual void OnData(common::IndexWithPositionList&& /* data */) = 0;

    virtual void OnData(common::IndexWithPositionTexCoordList&& /* data */) = 0;

    virtual void OnData(common::IndexWithTexCoordList&& /* data */) = 0;

    virtual void OnData(std::vector<Ogre::Real>&& /* data*/) = 0;

    virtual void OnData(common::PositionList&& /* data*/) = 0;

    virtual void OnData(block_data::Face28Entry&& /* data */) = 0;

    virtual void OnData(std::vector<block_data::Face28Entry::Unknown>&& /* data */) = 0;

    virtual void OnData(block_data::Mesh35&& /* data */) = 0;

    virtual void OnData(std::vector<block_data::Mesh35::Unknown49>&& /* data */) = 0;

    virtual void OnData(std::vector<block_data::GroupVertexData37::Unknown514>&& /* data */) = 0;

    virtual void OnData(std::vector<block_data::GroupVertexData37::Unknown258>&& /* data */) = 0;

    virtual void OnData(common::PositionWithNormalList&& /* data */) = 0;

    virtual void OnData(common::PositionWithTexCoordNormalList&& /* data */) = 0;
};


class VoidB3dListener : public B3dListenerInterface
{
public:
    virtual void OnBlockBegin(const block_data::BlockHeader& /* blockHeader */) override
    {
    }

    virtual void OnBlockEnd(const block_data::BlockHeader& /* blockHeader */) override
    {
    }

    virtual void OnNestedBlockBegin(std::uint32_t /* nestedBlockNumber */) override
    {
    }

    virtual void OnNestedBlockEnd(std::uint32_t /* nestedBlockNumber */) override
    {
    }

    virtual void OnMaterials(Materials&& /* materials */) override
    {
    }

    virtual void OnBlock(const block_data::Empty0& /* block */) override
    {
    }

    virtual void OnBlock(const block_data::GroupRoadInfraObjects4& /* block */) override
    {
    }

    virtual void OnBlock(const block_data::GroupObjects5& /* block */) override
    {
    }

    virtual void OnBlock(const block_data::GroupVertex7& /* block */) override
    {
    }

    virtual void OnBlock(const block_data::SimpleFaces8& /* block */) override
    {
    }

    virtual void OnBlock(const block_data::GroupTrigger9& /* block */) override
    {
    }

    virtual void OnBlock(const block_data::GroupLodParameters10& /* block */) override
    {
    }

    virtual void OnBlock(const block_data::GroupUnknown12& /* block */) override
    {
    }

    virtual void OnBlock(const block_data::SimpleTrigger13& /* block */) override
    {
    }

    virtual void OnBlock(const block_data::SimpleUnknown14& /* block */) override
    {
    }

    virtual void OnBlock(const block_data::SimpleObjectConnector18& /* block */) override
    {
    }

    virtual void OnBlock(const block_data::GroupObjects19& /* block */) override
    {
    }

    virtual void OnBlock(const block_data::SimpleFlatCollision20& /* block */) override
    {
    }

    virtual void OnBlock(const block_data::GroupObjects21& /* block */) override
    {
    }

    virtual void OnBlock(const block_data::SimpleVolumeCollision23& /* block */) override
    {
    }

    virtual void OnBlock(const block_data::GroupTransformMatrix24& /* block */) override
    {
    }

    virtual void OnBlock(const block_data::SimpleFaces28& /* block */) override
    {
    }

    virtual void OnBlock(const block_data::SimplePortal30& /* block */) override
    {
    }

    virtual void OnBlock(const block_data::GroupLightingObjects33& /* block */) override
    {
    }

    virtual void OnBlock(const block_data::SimpleFaceData35& /* block */) override
    {
    }

    virtual void OnBlock(const block_data::GroupVertexData37& /* block */) override
    {
    }

    virtual void OnBlock(const block_data::SimpleGeneratedObjects40& /* block */) override
    {
    }

    virtual void OnData(common::PositionWithTexCoordList&& /* data */) override
    {
    }

    virtual void OnData(block_data::Face8&& /* data */) override
    {
    }

    virtual void OnData(common::IndexList&& /* data */) override
    {
    }

    virtual void OnData(common::IndexWithPositionList&& /* data */) override
    {
    }

    virtual void OnData(common::IndexWithPositionTexCoordList&& /* data */) override
    {
    }

    virtual void OnData(common::IndexWithTexCoordList&& /* data */) override
    {
    }

    virtual void OnData(std::vector<Ogre::Real>&& /* data*/) override
    {
    }

    virtual void OnData(common::PositionList&& /* data*/) override
    {
    }

    virtual void OnData(block_data::Face28Entry&& /* data */) override
    {
    }

    virtual void OnData(std::vector<block_data::Face28Entry::Unknown>&& /* data */) override
    {
    }

    virtual void OnData(block_data::Mesh35&& /* data */) override
    {
    }

    virtual void OnData(std::vector<block_data::Mesh35::Unknown49>&& /* data */) override
    {
    }

    virtual void OnData(std::vector<block_data::GroupVertexData37::Unknown514>&& /* data */) override
    {
    }

    virtual void OnData(std::vector<block_data::GroupVertexData37::Unknown258>&& /* data */) override
    {
    }

    virtual void OnData(common::PositionWithNormalList&& /* data */) override
    {
    }

    virtual void OnData(common::PositionWithTexCoordNormalList&& /* data */) override
    {
    }
};


class B3dReader
{
public:
    void Read(Ogre::DataStream& input, B3dListenerInterface& listener);
};

} // namespace b3d
} // namespace data
} // namespace resource
} // namespace d2_hack


#endif /* D2_HACK_RESOURCE_DATA_B3D_READER_H */
