#ifndef D2_HACK_CODEC_DATA_B3D_READER_H
#define D2_HACK_CODEC_DATA_B3D_READER_H

#include <d2_hack/common/platform.h>

#include <iosfwd>
#include <cstdint>
#include <vector>
#include <array>

#include <OgreDataStream.h>

#include <d2_hack/common/types.h>

#include <d2_hack/codec/data/b3d_block_data_decl.h>

namespace d2_hack
{
namespace codec
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

    virtual void OnBlockBegin(const block_data::BlockHeader& /* blockHeader */)
    {
    }

    virtual void OnBlockEnd(const block_data::BlockHeader& /* blockHeader */)
    {
    }

    virtual void OnNestedBlockBegin(std::uint32_t /* nestedBlockNumber */)
    {
    }

    virtual void OnNestedBlockEnd(std::uint32_t /* nestedBlockNumber */)
    {
    }

    virtual void OnMaterials(Materials&& /* materials */)
    {
    }

    virtual void OnBlock(block_data::Empty0&& /* block */)
    {
    }

    virtual void OnBlock(block_data::GroupRoadInfraObjects4&& /* block */)
    {
    }

    virtual void OnBlock(block_data::GroupObjects5&& /* block */)
    {
    }

    virtual void OnBlock(block_data::GroupVertex7&& /* block */)
    {
    }

    virtual void OnBlock(block_data::SimpleFaces8&& /* block */)
    {
    }

    virtual void OnBlock(block_data::GroupTrigger9&& /* block */)
    {
    }

    virtual void OnBlock(block_data::GroupLodParameters10&& /* block */)
    {
    }

    virtual void OnBlock(block_data::GroupUnknown12&& /* block */)
    {
    }

    virtual void OnBlock(block_data::SimpleTrigger13&& /* block */)
    {
    }

    virtual void OnBlock(block_data::SimpleObjectConnector18&& /* block */)
    {
    }

    virtual void OnBlock(block_data::GroupObjects19&& /* block */)
    {
    }

    virtual void OnBlock(block_data::SimpleFlatCollision20&& /* block */)
    {
    }

    virtual void OnBlock(block_data::GroupObjects21&& /* block */)
    {
    }

    virtual void OnBlock(const block_data::SimpleVolumeCollision23& /* block */)
    {
    }

    virtual void OnBlock(block_data::GroupTransformMatrix24&& /* block */)
    {
    }

    virtual void OnBlock(block_data::SimpleFaces28&& /* block */)
    {
    }

    virtual void OnBlock(block_data::SimplePortal30&& /* block */)
    {
    }

    virtual void OnBlock(block_data::GroupLightingObjects33&& /* block */)
    {
    }

    virtual void OnBlock(const block_data::SimpleFaceData35& /* block */)
    {
    }

    virtual void OnBlock(const block_data::GroupVertexData37& /* block */)
    {
    }

    virtual void OnBlock(block_data::SimpleGeneratedObjects40&& /* block */)
    {
    }

    virtual void OnData(common::PositionWithTexCoordList&& /* data */)
    {
    }

    virtual void OnData(const block_data::Face8& /* data */)
    {
    }

    virtual void OnData(common::IndexList&& /* data */)
    {
    }

    virtual void OnData(common::IndexWithPositionList&& /* data */)
    {
    }

    virtual void OnData(common::IndexWithPositionTexCoordList&& /* data */)
    {
    }

    virtual void OnData(common::IndexWithTexCoordList&& /* data */)
    {
    }

    virtual void OnData(std::vector<Ogre::Real>&& /* data*/)
    {
    }

    virtual void OnData(common::PositionList&& /* data*/)
    {
    }

    virtual void OnData(const block_data::Face28Entry& /* data */)
    {
    }

    virtual void OnData(std::vector<block_data::Face28Entry::Unknown>&& /* data */)
    {
    }

    virtual void OnData(const block_data::Mesh35& /* data */)
    {
    }

    virtual void OnData(std::vector<block_data::Mesh35::Unknown49>&& /* data */)
    {
    }

    virtual void OnData(std::vector<block_data::GroupVertexData37::Unknown514>&& /* data */)
    {
    }

    virtual void OnData(common::PositionWithNormalList&& /* data */)
    {
    }

    virtual void OnData(common::PositionWithTexCoordNormalList&& /* data */)
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
} // namespace codec
} // namespace d2_hack


#endif /* D2_HACK_CODEC_DATA_B3D_READER_H */
