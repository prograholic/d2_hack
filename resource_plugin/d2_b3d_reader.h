#ifndef D2_HACK_RESOURCE_PLUGIN_D2_B3D_READER_H
#define D2_HACK_RESOURCE_PLUGIN_D2_B3D_READER_H

#include <iosfwd>
#include <cstdint>
#include <vector>
#include <array>

#include "d2_b3d_block_data_decl.h"

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


typedef std::vector<ResourceName> Materials;

struct BlockHeader
{
    ResourceName name;
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
    void Read(std::istream& input, B3d& data);
};

} // namespace b3d

#endif /* D2_HACK_RESOURCE_PLUGIN_D2_B3D_READER_H */
