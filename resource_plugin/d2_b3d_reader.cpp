// Implementation based on https://github.com/AlexKimov/HardTruck-RignRoll-file-formats

#include "d2_b3d_reader.h"

#include <cstring>
#include <cctype>

#include "d2_hack_common.h"


#define ENABLE_TRACE_B3D

#ifdef ENABLE_TRACE_B3D
#include <iostream>

#define B3D_LOG(data) {std::string off; off.resize(m_trace_offset * 4, ' '); std::cout << off << data << std::endl;}

#endif // ENABLE_TRACE_B3D


namespace b3d
{
static const std::uint8_t Signature[FileHeader::SignatureSize] = { 'b', '3', 'd', '\0' };

static const std::uint32_t DataBeginMagic = 111;
static const std::uint32_t DataEndMagic = 222;

static const std::uint32_t BlockBeginMagic = 333;
static const std::uint32_t BlockNextMagic = 444;
static const std::uint32_t BlockEndMagic = 555;

static const std::uint32_t EmptyBlock0 = 0;
static const std::uint32_t GroupObjectsBlock5 = 5;
static const std::uint32_t GroupLodParametersBlock10 = 10;
static const std::uint32_t GroupTransformMatrixBlock24 = 24;
static const std::uint32_t SimpleIndexAndTexturesBlock35 = 35;
static const std::uint32_t GroupIndexAndTexturesBlock37 = 37;

static const std::uint32_t MaxBlockId = 40;


namespace helpers
{

size_t ReadVector(file_io::Reader& reader, Ogre::Vector3& vector)
{
    size_t count = file_io::helpers::ReadFloat(reader, vector.x);
    count += file_io::helpers::ReadFloat(reader, vector.y);
    count += file_io::helpers::ReadFloat(reader, vector.z);

    return count;
}

} // namespace helpers


class B3dReaderImpl : public file_io::Reader
{
public:
    explicit B3dReaderImpl(std::istream& input)
        : file_io::Reader(input)
#ifdef ENABLE_TRACE_B3D
        , m_trace_offset(0)
#endif // ENABLE_TRACE_B3D
    {
    }

    void Read(B3d& data)
    {
        ReadFileHeader(data.header);

        ReadMaterials(data.header.materials, data.materials);

        ReadData(data.header.data, data.data);
    }

private:

    size_t m_trace_offset;

    void ReadFileHeader(FileHeader& fileHeader)
    {
        file_io::helpers::ReadUntil(*this, fileHeader.signature, FileHeader::SignatureSize);
        if (std::memcmp(fileHeader.signature, Signature, FileHeader::SignatureSize) != 0)
        {
            ThrowError("Incorrect b3d signature", "B3dReaderImpl::ReadHeader");
        }

        // file size
        file_io::helpers::ReadInt(*this, fileHeader.fileSize);
        fileHeader.fileSize *= 4;

        // material section
        file_io::helpers::ReadInt(*this, fileHeader.materials.offset);
        fileHeader.materials.offset *= 4;

        file_io::helpers::ReadInt(*this, fileHeader.materials.size);
        fileHeader.materials.size *= 4;

        // data section
        file_io::helpers::ReadInt(*this, fileHeader.data.offset);
        fileHeader.data.offset *= 4;

        file_io::helpers::ReadInt(*this, fileHeader.data.size);
        fileHeader.data.size *= 4;
    }

    void ReadMaterials(const FileHeader::Section& sectionInfo, Materials& materials)
    {
        if (GetOffset() != sectionInfo.offset)
        {
            ThrowError("Incorrect materials offset", "B3dReaderImpl::ReadMaterials");
        }

        Materials res;

        std::uint32_t materialsCount = 0;
        size_t count = file_io::helpers::ReadInt(*this, materialsCount);

        res.resize(materialsCount);
        for (auto& material : res)
        {
            count += file_io::helpers::ReadUntil(*this, material.begin(), material.size());
        }

        if (count != sectionInfo.size)
        {
            ThrowError("Incorrect materials size", "B3dReaderImpl::ReadMaterials");
        }

        materials = std::move(res);
    }

    void ReadData(const FileHeader::Section& sectionInfo, Data& data)
    {
        if (GetOffset() != sectionInfo.offset)
        {
            ThrowError("Incorrect data offset", "B3dReaderImpl::ReadData");
        }

        Data res;

        std::uint32_t blockSeparator = 0;
        size_t count = file_io::helpers::ReadInt(*this, blockSeparator);
        if (blockSeparator != DataBeginMagic)
        {
            ThrowError("Incorrect data begin magic", "B3dReaderImpl::ReadData");
        }

        bool gotEndOfData = false;
        for (; ; )
        {
            Block block;
            count += ReadBlock(block, gotEndOfData);
            data.push_back(std::move(block));

            if (gotEndOfData)
            {
                // reached EOF, exiting
                break;
            }
        }

        // check postconditions
        if (count != sectionInfo.size)
        {
            ThrowError("Incorrect data size", "B3dReaderImpl::ReadData");
        }

        data = std::move(res);
    }

    size_t DispatchBlock(Block& block)
    {
        if (block.header.type == EmptyBlock0)
        {
            return ReadBlockData0(block);
        }
        else if (block.header.type == GroupObjectsBlock5)
        {
            return ReadBlockData5(block);
        }
        else if (block.header.type == GroupLodParametersBlock10)
        {
            return ReadBlockData10(block);
        }
        else if (block.header.type == GroupTransformMatrixBlock24)
        {
            return ReadBlockData24(block);
        }
        else if (block.header.type == SimpleIndexAndTexturesBlock35)
        {
            return ReadBlockData35(block);
        }
        else if (block.header.type == GroupIndexAndTexturesBlock37)
        {
            return ReadBlockData37(block);
        }

        ThrowError("Unknown block id", "B3dReaderImpl::ReadBlock");
    }

    size_t ReadBlock(Block& block, bool& gotEndOfData)
    {
        std::uint32_t blockSeparator = 0;
        size_t count = file_io::helpers::ReadInt(*this, blockSeparator);
        B3D_LOG(blockSeparator);
        if (blockSeparator == BlockNextMagic)
        {
            // reread new block separator
            count += file_io::helpers::ReadInt(*this, blockSeparator);
            B3D_LOG(blockSeparator);
        }

        if (blockSeparator == DataEndMagic)
        {
            gotEndOfData = true;
            return count;
        }
        if (blockSeparator != BlockBeginMagic)
        {
            ThrowError("Incorrect block begin magic", "B3dReaderImpl::ReadBlock");
        }

#ifdef ENABLE_TRACE_B3D
        m_trace_offset += 1;
#endif //ENABLE_TRACE_B3D

        count += file_io::helpers::ReadUntil(*this, block.header.name.begin(), StringSize);

        count += file_io::helpers::ReadInt(*this, block.header.type);
        if (block.header.type > MaxBlockId)
        {
            ThrowError("Incorrect block id, possible b3d corruption?", "B3dReaderImpl::ReadBlock");
        }

        B3D_LOG("name: [" << block.header.name.data() << "]");
        B3D_LOG("type: " << block.header.type);

        count += DispatchBlock(block);

#ifdef ENABLE_TRACE_B3D
        m_trace_offset -= 1;
#endif //ENABLE_TRACE_B3D

        count += file_io::helpers::ReadInt(*this, blockSeparator);
        B3D_LOG(blockSeparator);
        if ((blockSeparator != BlockNextMagic) && (blockSeparator != BlockEndMagic))
        {
            ThrowError("Incorrect block end magic", "B3dReaderImpl::ReadBlock");
        }

        return count;
    }

    size_t ReadNestedBlocks(NestedBlockList& nestedBlocks)
    {
        std::uint32_t nestedBlocksCount = 0;
        size_t count = file_io::helpers::ReadInt(*this, nestedBlocksCount);

        B3D_LOG("nested: " << nestedBlocksCount);

        nestedBlocks.resize(nestedBlocksCount);
        for (auto& nestedBlock : nestedBlocks)
        {
            assert(!nestedBlock);

            nestedBlock = std::make_shared<Block>();

            bool gotEndOfData = false;
            count += ReadBlock(*nestedBlock, gotEndOfData);
            if (gotEndOfData)
            {
                ThrowError("Unexpected end of data", "B3dReaderImpl::ReadNestedBlocks");
            }
        }

        return count;
    }

    size_t ReadBlockData0(Block& block)
    {
        block_data::Empty0 blockData;

        size_t count = file_io::helpers::ReadUntil(*this, blockData.emptyData0, sizeof(blockData.emptyData0));
        count += file_io::helpers::ReadFloat(*this, blockData.unknown);
        count += file_io::helpers::ReadUntil(*this, blockData.emptyData1, sizeof(blockData.emptyData1));

        block.data = std::move(blockData);
        return count;
    }

    size_t ReadBlockData5(Block& block)
    {
        block_data::GroupObjects5 blockData;

        size_t count = helpers::ReadVector(*this, blockData.center);
        count += file_io::helpers::ReadFloat(*this, blockData.boundingSphereRadius);
        count += file_io::helpers::ReadUntil(*this, blockData.name.begin(), blockData.name.size());

        count += ReadNestedBlocks(blockData.nestedBlocks);

        block.data = std::move(blockData);
        return count;
    }

    size_t ReadBlockData10(Block& block)
    {
        block_data::GroupLodParameters10 blockData;

        size_t count = helpers::ReadVector(*this, blockData.center);
        count += file_io::helpers::ReadFloat(*this, blockData.boundingSphereRadius);

        count += helpers::ReadVector(*this, blockData.unknown);
        count += file_io::helpers::ReadFloat(*this, blockData.distanceToPlayer);

        count += ReadNestedBlocks(blockData.nestedBlocks);

        block.data = std::move(blockData);
        return count;
    }

    size_t ReadBlockData24(Block& block)
    {
        block_data::GroupTransformMatrix24 blockData;

        size_t count = helpers::ReadVector(*this, blockData.x);
        count += helpers::ReadVector(*this, blockData.y);
        count += helpers::ReadVector(*this, blockData.z);

        count += helpers::ReadVector(*this, blockData.position);

        count += file_io::helpers::ReadInt(*this, blockData.unknown);

        count += ReadNestedBlocks(blockData.nestedBlocks);

        block.data = std::move(blockData);
        return count;
    }

    size_t __DebugB3d()
    {
        std::string data;
        size_t count = 0;
        for (; ; )
        {
            char c;
            count += file_io::helpers::ReadUntil(*this, &c, 1);
            data.push_back(c);
            if (data.size() >= 8)
            {
                char buff1[8];
                memcpy(buff1, &BlockNextMagic, 4);
                memcpy(&buff1[4], &BlockBeginMagic, 4);

                size_t pos1 = data.find(buff1, 0, 8);
                if (pos1 == std::string::npos)
                {
                    char buff2[8];
                    memcpy(buff2, &BlockEndMagic, 4);
                    memcpy(&buff2[4], &BlockBeginMagic, 4);

                    size_t pos2 = data.find(buff2, 0, 8);
                    if (pos2 == std::string::npos)
                    {
                        continue;
                    }
                    else
                    {
                        //__debugbreak();
                    }
                }

                //__debugbreak();
                std::string hexed;
                size_t cnt = 0;
                for (unsigned char d : data)
                {
                    char buff2[] = "0123456789ABCDEF";
                    hexed.push_back(buff2[d >> 4]);
                    hexed.push_back(buff2[d & 0x0F]);
                    hexed.push_back(' ');
                    cnt += 1;
                    if ((cnt % 4) == 0)
                    {
                        const std::uint8_t* p = (const std::uint8_t*)(data.data() + (cnt - 4));
                        hexed += std::to_string(file_io::details::ToNumericUnsafe<float>(p));
                        hexed += " ";
                        hexed += std::to_string(file_io::details::ToNumericUnsafe<std::uint32_t>(p));
                        hexed.push_back('\n');
                    }
                }
                B3D_LOG("unknown: [" << hexed << "], cnt: " << cnt << ", size: " << data.size())
                    break;
            }
        }

        return count;
    }

    size_t ReadBlockData35(Block& /* block */)
    {
        return __DebugB3d();
    }

    size_t ReadBlockData37(Block& block)
    {
        block_data::GroupIndexAndTextures37 blockData;

        size_t count = helpers::ReadVector(*this, blockData.mayBeCenter);
        count += file_io::helpers::ReadFloat(*this, blockData.mayBeBoundingSphereRadius);
        count += file_io::helpers::ReadUntil(*this, blockData.mayBeName.begin(), blockData.mayBeName.size());
        count += file_io::helpers::ReadInt(*this, blockData.unknown);

        std::uint32_t dataSize = 0;
        count += file_io::helpers::ReadInt(*this, dataSize);
        blockData.mayBePositionAndNormalList.reserve(dataSize);
        while (dataSize > 0)
        {
            dataSize -= 1;

            block_data::MayBePositionAndNormal item;
            count += helpers::ReadVector(*this, item.position);
            count += helpers::ReadVector(*this, item.normal);

            blockData.mayBePositionAndNormalList.push_back(std::move(item));
        }

        count += ReadNestedBlocks(blockData.nestedBlocks);

        block.data = std::move(blockData);
        return count;
    }
};

void B3dReader::Read(std::istream &input, B3d& data)
{
    B3dReaderImpl reader{input};
    
    reader.Read(data);
}

} // namespace b3d