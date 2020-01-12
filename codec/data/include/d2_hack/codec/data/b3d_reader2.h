#ifndef D2_HACK_CODEC_DATA_B3D_READER2_H
#define D2_HACK_CODEC_DATA_B3D_READER2_H

#include <d2_hack/common/platform.h>

#include <iosfwd>
#include <cstdint>
#include <vector>
#include <array>

#include <OgreDataStream.h>

#include <d2_hack/common/types.h>

#include <d2_hack/codec/data/b3d_block_data_decl2.h>

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

    virtual void OnBlockBegin(const block_data2::BlockHeader& /* blockHeader */)
    {
    }

    virtual void OnBlockEnd(const block_data2::BlockHeader& /* blockHeader */)
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

    virtual void OnBlock(block_data2::Empty0&& /* block */)
    {
    }

    virtual void OnBlock(block_data2::GroupRoadInfraObjects4&& /* block */)
    {
    }

    virtual void OnBlock(block_data2::GroupObjects5&& /* block */)
    {
    }

    virtual void OnBlock(block_data2::GroupVertex7&& /* block */)
    {
    }

    virtual void OnBlock(block_data2::SimpleFaces8&& /* block */)
    {
    }

    virtual void OnBlock(block_data2::GroupTrigger9&& /* block */)
    {
    }

    virtual void OnBlock(block_data2::GroupLodParameters10&& /* block */)
    {
    }

    virtual void OnBlock(block_data2::GroupUnknown12&& /* block */)
    {
    }

    virtual void OnBlock(block_data2::SimpleTrigger13&& /* block */)
    {
    }

    virtual void OnBlock(block_data2::SimpleObjectConnector18&& /* block */)
    {
    }

    virtual void OnBlock(block_data2::GroupObjects19&& /* block */)
    {
    }

    virtual void OnBlock(block_data2::SimpleFlatCollision20&& /* block */)
    {
    }

    virtual void OnBlock(block_data2::GroupObjects21&& /* block */)
    {
    }

    virtual void OnBlock(const block_data2::SimpleVolumeCollision23& /* block */)
    {
    }

    virtual void OnBlock(block_data2::GroupTransformMatrix24&& /* block */)
    {
    }

    virtual void OnBlock(block_data2::SimpleFaces28&& /* block */)
    {
    }

    virtual void OnBlock(block_data2::SimplePortal30&& /* block */)
    {
    }

    virtual void OnBlock(block_data2::GroupLightingObjects33&& /* block */)
    {
    }

    virtual void OnBlock(block_data2::SimpleFaceData35&& /* block */)
    {
    }

    virtual void OnBlock(block_data2::GroupVertexData37&& /* block */)
    {
    }

    virtual void OnBlock(block_data2::SimpleGeneratedObjects40&& /* block */)
    {
    }

    virtual void OnData(common::PositionWithTexCoordList& /* data */)
    {
    }

    virtual void OnData(const block_data2::Face8& /* data */)
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

    virtual void OnData(const block_data2::Face28Entry& /* data */)
    {
    }

    virtual void OnData(std::vector<block_data2::Face28Entry::Unknown>&& /* data */)
    {
    }
};


class B3dReader2
{
public:
    void Read(Ogre::DataStream& input, B3dListenerInterface& listener);
};

} // namespace b3d
} // namespace data
} // namespace codec
} // namespace d2_hack


#endif /* D2_HACK_CODEC_DATA_B3D_READER2_H */
