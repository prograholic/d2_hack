#ifndef D2_HACK_CODEC_DATA_B3D_READER_H
#define D2_HACK_CODEC_DATA_B3D_READER_H

#include <d2_hack/common/platform.h>

#include <iosfwd>
#include <cstdint>
#include <vector>
#include <array>

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
