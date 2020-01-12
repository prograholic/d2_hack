#ifndef D2_HACK_CODEC_DATA_B3D_READER_H
#define D2_HACK_CODEC_DATA_B3D_READER_H

#include <d2_hack/common/platform.h>

#include <iosfwd>
#include <cstdint>
#include <vector>
#include <array>

#include <OgreDataStream.h>

#include <d2_hack/common/types.h>
#include <d2_hack/codec/data/b3d.h>


#include "b3d_block_data_decl.h"

namespace d2_hack
{
namespace codec
{
namespace data
{
namespace b3d
{

struct FileHeader
{
    static const size_t SignatureSize = 4;

    std::uint8_t signature[SignatureSize];
    std::uint32_t fileSize;

    struct Section
    {
        std::uint32_t offset;
        std::uint32_t size;
    };

    Section materials;

    Section data;
};


typedef std::vector<common::ResourceName> Materials;

struct BlockHeader
{
    common::ResourceName name;
    std::uint32_t type;
};

struct Block
{
    BlockHeader header;

    block_data::Data data;
};

typedef std::vector<Block> Data;

struct B3d
{
    FileHeader header;

    Materials materials;
    Data data;
};



class B3dVisitorInterface
{
public:
    virtual ~B3dVisitorInterface() = default;

    virtual void VisitPosition(const common::Position& /* position */)
    {
    }

    virtual void VisitBoundingSphere(const common::BoundingSphere& /* boundingSphere */)
    {
    }

    virtual void VisitBlockSeparator(std::uint32_t /* blockSeparator */)
    {
    }

    virtual void VisitBlockHeader(const common::ResourceName& /* name */, std::uint32_t /* type */)
    {
    }

    virtual void VisitBlock(const common::ResourceName& /* name */, std::uint32_t /* type */)
    {
    }

    virtual void VisitNestedBlockHeader(std::uint32_t /* nestedBlockCount */)
    {
    }

    virtual std::ostream* GetDebugOutStream()
    {
        return nullptr;
    }

    virtual void VisitMaterials(const b3d::Materials& /* materials */)
    {
    }

    virtual void VisitBlockData(const b3d::block_data::Empty0& /* blockData */)
    {
    }

    virtual void VisitBlockData(const b3d::block_data::GroupRoadInfraObjects4& /* blockData */)
    {
    }

    virtual void VisitBlockData(const b3d::block_data::GroupObjects5& /* blockData */)
    {
    }

    virtual void VisitBlockData(const b3d::block_data::GroupVertex7& /* blockData */)
    {
    }

    virtual void VisitBlockData(const b3d::block_data::SimpleFaces8& /* blockData */)
    {
    }

    virtual void VisitBlockData(const b3d::block_data::GroupTrigger9& /* blockData */)
    {
    }

    virtual void VisitBlockData(const b3d::block_data::GroupLodParameters10& /* blockData */)
    {
    }

    virtual void VisitBlockData(const b3d::block_data::GroupUnknown12& /* blockData */)
    {
    }

    virtual void VisitBlockData(const b3d::block_data::SimpleTrigger13& /* blockData */)
    {
    }

    virtual void VisitBlockData(const b3d::block_data::SimpleObjectConnector18& /* blockData */)
    {
    }

    virtual void VisitBlockData(const b3d::block_data::GroupObjects19& /* blockData */)
    {
    }

    virtual void VisitBlockData(const b3d::block_data::SimpleFlatCollision20& /* blockData */)
    {
    }

    virtual void VisitBlockData(const b3d::block_data::GroupObjects21& /* blockData */)
    {
    }

    virtual void VisitBlockData(const b3d::block_data::SimpleVolumeCollision23& /* blockData */)
    {
    }

    virtual void VisitBlockData(const b3d::block_data::GroupTransformMatrix24& /* blockData */)
    {
    }

    virtual void VisitBlockData(const b3d::block_data::SimpleFaces28& /* blockData */)
    {
    }

    virtual void VisitBlockData(const b3d::block_data::SimplePortal30& /* blockData */)
    {
    }

    virtual void VisitBlockData(const b3d::block_data::GroupLightingObjects33& /* blockData */)
    {
    }

    virtual void VisitBlockData(const b3d::block_data::SimpleFaceData35& /* blockData */)
    {
    }

    virtual void VisitBlockData(const b3d::block_data::GroupVertexData37& /* blockData */)
    {
    }

    virtual void VisitBlockData(const b3d::block_data::SimpleGeneratedObjects40& /* blockData */)
    {
    }
};



class B3dReader
{
public:
    void Read(std::istream& input, B3d& data, B3dVisitorInterface* visior = nullptr);
};


} // namespace b3d
} // namespace data
} // namespace codec
} // namespace d2_hack


#endif /* D2_HACK_CODEC_DATA_B3D_READER_H */
