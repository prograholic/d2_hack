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

enum class BlockAction
{
    Skip,
    Process
};


// TODO: add const common::ResourceName& resourceName,  to OnBlock method
class B3dListenerInterface
{
public:
    virtual ~B3dListenerInterface() = default;

    virtual BlockAction OnBlockBegin(const block_data::BlockHeader& /* blockHeader */) = 0;

    virtual void OnBlockEnd(const block_data::BlockHeader& /* blockHeader */, BlockAction blockAction) = 0;

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

    virtual void OnBlock(const block_data::GroupUnknown29& /* block */) = 0;

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

    virtual void OnData(common::PositionList&& /* data*/) = 0;

    virtual void OnData(block_data::Face28Entry&& /* data */) = 0;

    virtual void OnData(std::vector<block_data::Face28Entry::Unknown>&& /* data */) = 0;

    virtual void OnData(block_data::Mesh35&& /* data */) = 0;

    virtual void OnData(std::vector<block_data::Mesh35::Unknown49>&& /* data */) = 0;

    virtual void OnData(std::vector<block_data::GroupVertexData37::Unknown514>&& /* data */) = 0;

    virtual void OnData(std::vector<block_data::GroupVertexData37::Unknown258>&& /* data */) = 0;

    virtual void OnData(common::PositionWithNormalList&& /* data */) = 0;

    virtual void OnData(common::PositionWithTexCoordNormalList&& /* data */) = 0;

    virtual void OnData(common::CollisionPositionList&& /* data */) = 0;

    virtual void OnData(common::CollisionUnknownList&& /* data */) = 0;

    virtual void OnData(common::TriggerInfoList&& /* data */) = 0;

    virtual void OnData(common::GeneratedObjectInfo&& /* data */) = 0;
};



template <typename SimpleAction>
class SimpleActionB3dListener : public B3dListenerInterface
{
public:
    virtual BlockAction OnBlockBegin(const block_data::BlockHeader& blockHeader) override
    {
        return SimpleAction::OnBlockBegin(blockHeader);
    }

    virtual void OnBlockEnd(const block_data::BlockHeader& blockHeader, BlockAction blockAction) override
    {
        SimpleAction::OnBlockEnd(blockHeader, blockAction);
    }

    virtual void OnNestedBlockBegin(std::uint32_t nestedBlockNumber) override
    {
        SimpleAction::OnNestedBlockBegin(nestedBlockNumber);
    }

    virtual void OnNestedBlockEnd(std::uint32_t nestedBlockNumber) override
    {
        SimpleAction::OnNestedBlockEnd(nestedBlockNumber);
    }

    virtual void OnMaterials(Materials&& materials) override
    {
        SimpleAction::OnMaterials(std::forward<Materials>(materials));
    }

    virtual void OnBlock(const block_data::Empty0& block) override
    {
        SimpleAction::OnBlock(block);
    }

    virtual void OnBlock(const block_data::GroupRoadInfraObjects4& block) override
    {
        SimpleAction::OnBlock(block);
    }

    virtual void OnBlock(const block_data::GroupObjects5& block) override
    {
        SimpleAction::OnBlock(block);
    }

    virtual void OnBlock(const block_data::GroupVertex7& block) override
    {
        SimpleAction::OnBlock(block);
    }

    virtual void OnBlock(const block_data::SimpleFaces8& block) override
    {
        SimpleAction::OnBlock(block);
    }

    virtual void OnBlock(const block_data::GroupTrigger9& block) override
    {
        SimpleAction::OnBlock(block);
    }

    virtual void OnBlock(const block_data::GroupLodParameters10& block) override
    {
        SimpleAction::OnBlock(block);
    }

    virtual void OnBlock(const block_data::GroupUnknown12& block) override
    {
        SimpleAction::OnBlock(block);
    }

    virtual void OnBlock(const block_data::SimpleTrigger13& block) override
    {
        SimpleAction::OnBlock(block);
    }

    virtual void OnBlock(const block_data::SimpleUnknown14& block) override
    {
        SimpleAction::OnBlock(block);
    }

    virtual void OnBlock(const block_data::SimpleObjectConnector18& block) override
    {
        SimpleAction::OnBlock(block);
    }

    virtual void OnBlock(const block_data::GroupObjects19& block) override
    {
        SimpleAction::OnBlock(block);
    }

    virtual void OnBlock(const block_data::SimpleFlatCollision20& block) override
    {
        SimpleAction::OnBlock(block);
    }

    virtual void OnBlock(const block_data::GroupObjects21& block) override
    {
        SimpleAction::OnBlock(block);
    }

    virtual void OnBlock(const block_data::SimpleVolumeCollision23& block) override
    {
        SimpleAction::OnBlock(block);
    }

    virtual void OnBlock(const block_data::GroupTransformMatrix24& block) override
    {
        SimpleAction::OnBlock(block);
    }

    virtual void OnBlock(const block_data::SimpleFaces28& block) override
    {
        SimpleAction::OnBlock(block);
    }

    virtual void OnBlock(const block_data::GroupUnknown29& block) override
    {
        SimpleAction::OnBlock(block);
    }

    virtual void OnBlock(const block_data::SimplePortal30& block) override
    {
        SimpleAction::OnBlock(block);
    }

    virtual void OnBlock(const block_data::GroupLightingObjects33& block) override
    {
        SimpleAction::OnBlock(block);
    }

    virtual void OnBlock(const block_data::SimpleFaceData35& block) override
    {
        SimpleAction::OnBlock(block);
    }

    virtual void OnBlock(const block_data::GroupVertexData37& block) override
    {
        SimpleAction::OnBlock(block);
    }

    virtual void OnBlock(const block_data::SimpleGeneratedObjects40& block) override
    {
        SimpleAction::OnBlock(block);
    }

    virtual void OnData(common::PositionWithTexCoordList&& data) override
    {
        SimpleAction::OnData(std::forward<common::PositionWithTexCoordList>(data));
    }

    virtual void OnData(block_data::Face8&& data) override
    {
        SimpleAction::OnData(std::forward<block_data::Face8>(data));
    }

    virtual void OnData(common::IndexList&& data) override
    {
        SimpleAction::OnData(std::forward<common::IndexList>(data));
    }

    virtual void OnData(common::IndexWithPositionList&& data) override
    {
        SimpleAction::OnData(std::forward<common::IndexWithPositionList>(data));
    }

    virtual void OnData(common::IndexWithPositionTexCoordList&& data) override
    {
        SimpleAction::OnData(std::forward<common::IndexWithPositionTexCoordList>(data));
    }

    virtual void OnData(common::IndexWithTexCoordList&& data) override
    {
        SimpleAction::OnData(std::forward<common::IndexWithTexCoordList>(data));
    }

    virtual void OnData(common::PositionList&& data) override
    {
        SimpleAction::OnData(std::forward<common::PositionList>(data));
    }

    virtual void OnData(block_data::Face28Entry&& data) override
    {
        SimpleAction::OnData(std::forward<block_data::Face28Entry>(data));
    }

    virtual void OnData(std::vector<block_data::Face28Entry::Unknown>&& data) override
    {
        SimpleAction::OnData(std::forward<std::vector<block_data::Face28Entry::Unknown>>(data));
    }

    virtual void OnData(block_data::Mesh35&& data) override
    {
        SimpleAction::OnData(std::forward<block_data::Mesh35>(data));
    }

    virtual void OnData(std::vector<block_data::Mesh35::Unknown49>&& data) override
    {
        SimpleAction::OnData(std::forward<std::vector<block_data::Mesh35::Unknown49>>(data));
    }

    virtual void OnData(std::vector<block_data::GroupVertexData37::Unknown514>&& data) override
    {
        SimpleAction::OnData(std::forward<std::vector<block_data::GroupVertexData37::Unknown514>>(data));
    }

    virtual void OnData(std::vector<block_data::GroupVertexData37::Unknown258>&& data) override
    {
        SimpleAction::OnData(std::forward<std::vector<block_data::GroupVertexData37::Unknown258>>(data));
    }

    virtual void OnData(common::PositionWithNormalList&& data) override
    {
        SimpleAction::OnData(std::forward<common::PositionWithNormalList>(data));
    }

    virtual void OnData(common::PositionWithTexCoordNormalList&& data) override
    {
        SimpleAction::OnData(std::forward<common::PositionWithTexCoordNormalList>(data));
    }

    virtual void OnData(common::CollisionPositionList&& data) override
    {
        SimpleAction::OnData(std::forward<common::CollisionPositionList>(data));
    }

    virtual void OnData(common::CollisionUnknownList&& data) override
    {
        SimpleAction::OnData(std::forward<common::CollisionUnknownList>(data));
    }

    virtual void OnData(common::TriggerInfoList&& data) override
    {
        SimpleAction::OnData(std::forward<common::TriggerInfoList>(data));
    }

    virtual void OnData(common::GeneratedObjectInfo&& data) override
    {
        SimpleAction::OnData(std::forward<common::GeneratedObjectInfo>(data));
    }
};

struct VoidAction
{
    static BlockAction OnBlockBegin(const block_data::BlockHeader& /* blockHeader */)
    {
        return BlockAction::Process;
    }

    static void OnBlockEnd(const block_data::BlockHeader& /* blockHeader */, BlockAction /* blockAction */)
    {
    }

    static void OnNestedBlockBegin(std::uint32_t /* nestedBlockNumber */)
    {
    }

    static void OnNestedBlockEnd(std::uint32_t /* nestedBlockNumber */)
    {
    }

    static void OnMaterials(Materials&& /* materials */)
    {
    }

    template <typename T>
    static void OnBlock(const T& /* block */)
    {
    }

    template <typename T>
    static void OnData(T&& /* data */)
    {
    }
};

typedef SimpleActionB3dListener<VoidAction> VoidB3dListener;


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
