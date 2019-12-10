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
static const std::uint32_t GroupVertexBlock7 = 7;
static const std::uint32_t GroupLodParametersBlock10 = 10;
static const std::uint32_t GroupTransformMatrixBlock24 = 24;
static const std::uint32_t SimpleFaceDataBlock35 = 35;
static const std::uint32_t GroupIndexAndTexturesBlock37 = 37;

static const std::uint32_t MaxBlockId = 40;


namespace helpers
{

Ogre::Vector3 ReadVector3(file_io::Reader& reader)
{
    Ogre::Vector3 res;

    res.x = file_io::helpers::ReadFloat(reader);
    res.y = file_io::helpers::ReadFloat(reader);
    res.z = file_io::helpers::ReadFloat(reader);

    return res;
}

Ogre::Vector2 ReadVector2(file_io::Reader& reader)
{
    Ogre::Vector2 res;

    res.x = file_io::helpers::ReadFloat(reader);
    res.y = file_io::helpers::ReadFloat(reader);

    return res;
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
        fileHeader.fileSize = file_io::helpers::ReadUint32(*this) * 4;

        // material section
        fileHeader.materials.offset = file_io::helpers::ReadUint32(*this) * 4;
        fileHeader.materials.size = file_io::helpers::ReadUint32(*this) * 4;


        // data section
        fileHeader.data.offset = file_io::helpers::ReadUint32(*this) * 4;
        fileHeader.data.size = file_io::helpers::ReadUint32(*this) * 4;
    }

    void ReadMaterials(const FileHeader::Section& sectionInfo, Materials& materials)
    {
        const size_t materialsStartOffset = GetOffset();
        if (materialsStartOffset != sectionInfo.offset)
        {
            ThrowError("Incorrect materials offset", "B3dReaderImpl::ReadMaterials");
        }

        Materials res;

        const std::uint32_t materialsCount = file_io::helpers::ReadUint32(*this);
        res.resize(materialsCount);
        for (auto& material : res)
        {
            file_io::helpers::ReadUntil(*this, material.begin(), material.size());
        }

        const size_t materialsEndOffset = GetOffset();
        assert(materialsEndOffset > materialsStartOffset);

        if ((materialsEndOffset - materialsStartOffset) != sectionInfo.size)
        {
            ThrowError("Incorrect materials size", "B3dReaderImpl::ReadMaterials");
        }

        materials = std::move(res);
    }

    void ReadData(const FileHeader::Section& sectionInfo, Data& data)
    {
        const size_t dataStartOffset = GetOffset();
        if (dataStartOffset != sectionInfo.offset)
        {
            ThrowError("Incorrect data offset", "B3dReaderImpl::ReadData");
        }

        Data res;

        const std::uint32_t blockSeparator = file_io::helpers::ReadUint32(*this);
        if (blockSeparator != DataBeginMagic)
        {
            ThrowError("Incorrect data begin magic", "B3dReaderImpl::ReadData");
        }

        bool gotEndOfData = false;
        for (; ; )
        {
            Block block;
            ReadBlock(block, gotEndOfData);
            data.push_back(std::move(block));

            if (gotEndOfData)
            {
                // reached EOF, exiting
                break;
            }
        }

        const size_t dataEndOffset = GetOffset();
        assert(dataEndOffset > dataStartOffset);
        // check postconditions
        if ((dataEndOffset - dataStartOffset) != sectionInfo.size)
        {
            ThrowError("Incorrect data size", "B3dReaderImpl::ReadData");
        }

        data = std::move(res);
    }

    void DispatchBlock(Block& block)
    {
        if (block.header.type == EmptyBlock0)
        {
            return ReadBlockData0(block);
        }
        else if (block.header.type == GroupObjectsBlock5)
        {
            return ReadBlockData5(block);
        }
        else if (block.header.type == GroupVertexBlock7)
        {
            return ReadBlockData7(block);
        }
        else if (block.header.type == GroupLodParametersBlock10)
        {
            return ReadBlockData10(block);
        }
        else if (block.header.type == GroupTransformMatrixBlock24)
        {
            return ReadBlockData24(block);
        }
        else if (block.header.type == SimpleFaceDataBlock35)
        {
            return ReadBlockData35(block);
        }
        else if (block.header.type == GroupIndexAndTexturesBlock37)
        {
            return ReadBlockData37(block);
        }

        ThrowError("Unknown block id", "B3dReaderImpl::ReadBlock");
    }

    void __DebugB3d()
    {
        std::string data;
        bool eof = false;
        for (; ; )
        {
            char c;
            try
            {
                file_io::helpers::ReadUntil(*this, &c, 1);
            }
            catch (...)
            {
                eof = true;
                break;
            }
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

                break;
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
        B3D_LOG("unknown: [\n" << hexed << "], cnt: " << cnt << ", size: " << data.size());
        if (eof)
        {
            B3D_LOG("\nunknown: EOF: " << GetOffset());
        }
    }

    void ReadBlock(Block& block, bool& gotEndOfData)
    {
        std::uint32_t blockSeparator = file_io::helpers::ReadUint32(*this);
        B3D_LOG(blockSeparator);
        if ((blockSeparator == BlockNextMagic) || (blockSeparator == BlockEndMagic))
        {
            // reread new block separator
            blockSeparator = file_io::helpers::ReadUint32(*this);
            B3D_LOG(blockSeparator);
        }

        if (blockSeparator == DataEndMagic)
        {
            gotEndOfData = true;
            return;
        }
        if (blockSeparator != BlockBeginMagic)
        {
            ThrowError("Incorrect block begin magic", "B3dReaderImpl::ReadBlock");
        }

#ifdef ENABLE_TRACE_B3D
        m_trace_offset += 1;
#endif //ENABLE_TRACE_B3D

        file_io::helpers::ReadUntil(*this, block.header.name.begin(), StringSize);
        block.header.type = file_io::helpers::ReadUint32(*this);

        if (block.header.type > MaxBlockId)
        {
            ThrowError("Incorrect block id, possible b3d corruption?", "B3dReaderImpl::ReadBlock");
        }

        B3D_LOG("name: [" << block.header.name.data() << "]");
        B3D_LOG("type: " << block.header.type);

        DispatchBlock(block);

#ifdef ENABLE_TRACE_B3D
        m_trace_offset -= 1;
#endif //ENABLE_TRACE_B3D

        blockSeparator = file_io::helpers::ReadUint32(*this);
        B3D_LOG(blockSeparator);
        if ((blockSeparator != BlockNextMagic) && (blockSeparator != BlockEndMagic))
        {
            ThrowError("Incorrect block end magic", "B3dReaderImpl::ReadBlock");
        }
    }

    void ReadNestedBlocks(NestedBlockList& nestedBlocks)
    {
        const std::uint32_t nestedBlocksCount = file_io::helpers::ReadUint32(*this);
        B3D_LOG("nested: " << nestedBlocksCount);

        if (nestedBlocksCount > 1000)
        {
            __debugbreak();
            ThrowError("Unexpectedly big nested size", "B3dReaderImpl::ReadNestedBlocks");
        }

        nestedBlocks.resize(nestedBlocksCount);
        for (auto& nestedBlock : nestedBlocks)
        {
            assert(!nestedBlock);

            nestedBlock = std::make_shared<Block>();

            bool gotEndOfData = false;
            ReadBlock(*nestedBlock, gotEndOfData);
            if (gotEndOfData)
            {
                ThrowError("Unexpected end of data", "B3dReaderImpl::ReadNestedBlocks");
            }
        }
    }

    void ReadBlockData0(Block& block)
    {
        block_data::Empty0 blockData;

        file_io::helpers::ReadUntil(*this, blockData.emptyData0, sizeof(blockData.emptyData0));
        blockData.unknown = file_io::helpers::ReadFloat(*this);
        file_io::helpers::ReadUntil(*this, blockData.emptyData1, sizeof(blockData.emptyData1));

        block.data = std::move(blockData);
    }

    void ReadBlockData5(Block& block)
    {
        block_data::GroupObjects5 blockData;

        blockData.center = helpers::ReadVector3(*this);
        blockData.boundingSphereRadius = file_io::helpers::ReadFloat(*this);
        file_io::helpers::ReadUntil(*this, blockData.name.begin(), blockData.name.size());

        ReadNestedBlocks(blockData.nestedBlocks);

        block.data = std::move(blockData);
    }

    void ReadBlockData7(Block& block)
    {
        static int cnt = 0;
        cnt += 1;
        if (cnt == 1)
        {
            cnt = 1;
            //__debugbreak();
            //return __DebugB3d();
        }

        block_data::GroupVertex7 blockData;

        blockData.center = helpers::ReadVector3(*this);
        blockData.boundingSphereRadius = file_io::helpers::ReadFloat(*this);
        file_io::helpers::ReadUntil(*this, blockData.name.begin(), blockData.name.size());

        const std::uint32_t vertexAmount = file_io::helpers::ReadUint32(*this);
        blockData.vertices.resize(vertexAmount);
        for (auto& vertexEntry : blockData.vertices)
        {
            vertexEntry.point = helpers::ReadVector3(*this);
            vertexEntry.textureCoord = helpers::ReadVector2(*this);
        }

        ReadNestedBlocks(blockData.nestedBlocks);

        block.data = std::move(blockData);
    }

    void ReadBlockData10(Block& block)
    {
        block_data::GroupLodParameters10 blockData;

        blockData.center = helpers::ReadVector3(*this);
        blockData.boundingSphereRadius = file_io::helpers::ReadFloat(*this);

        blockData.unknown = helpers::ReadVector3(*this);
        blockData.distanceToPlayer = file_io::helpers::ReadFloat(*this);

        ReadNestedBlocks(blockData.nestedBlocks);

        block.data = std::move(blockData);
    }

    void ReadBlockData24(Block& block)
    {
        block_data::GroupTransformMatrix24 blockData;

        blockData.x = helpers::ReadVector3(*this);
        blockData.y = helpers::ReadVector3(*this);
        blockData.z = helpers::ReadVector3(*this);

        blockData.position = helpers::ReadVector3(*this);

        blockData.unknown = file_io::helpers::ReadUint32(*this);

        ReadNestedBlocks(blockData.nestedBlocks);

        block.data = std::move(blockData);
    }

    void DispatchReadMeshData35(const std::uint32_t blockType, const std::uint32_t meshType, block_data::Mesh35::MeshData& meshData)
    {
        if (blockType == block_data::SimpleFaceData35::Unknown1)
        {
            if (meshType == block_data::Mesh35::UnknownType48)
            {
                block_data::Mesh35::Unknown48 meshDataValue;

                meshDataValue.faceIndex = file_io::helpers::ReadUint32(*this);
                meshDataValue.position = helpers::ReadVector3(*this);

                meshData = meshDataValue;
            }
            else if (meshType == block_data::Mesh35::UnknownType50)
            {
                block_data::Mesh35::Unknown50 meshDataValue;

                meshDataValue.faceIndex = file_io::helpers::ReadUint32(*this);
                meshDataValue.position = helpers::ReadVector3(*this);
                meshDataValue.texCoord = helpers::ReadVector2(*this);

                meshData = meshDataValue;
            }
            else
            {
                ThrowError("Unknown mesh type for block type 1", "B3dReaderImpl::DispatchReadMesh35");
            }
        }
        else if (blockType == block_data::SimpleFaceData35::Unknown2)
        {
            if (meshType == block_data::Mesh35::Indices1)
            {
                block_data::Mesh35::Indices meshDataValue;
                meshDataValue.index = file_io::helpers::ReadUint32(*this);

                meshData = meshDataValue;
            }
            else if (meshType == block_data::Mesh35::UnknownType49)
            {
                block_data::Mesh35::Unknown49 meshDataValue;

                meshDataValue.unknown0 = file_io::helpers::ReadUint32(*this);
                meshDataValue.unknown1 = file_io::helpers::ReadFloat(*this);

                meshData = meshDataValue;
            }
            else
            {
                ThrowError("Unknown mesh type for block type 2", "B3dReaderImpl::DispatchReadMesh35");
            }
        }
        else if (blockType == block_data::SimpleFaceData35::IndicesOnly3)
        {
            if ((meshType == block_data::Mesh35::Indices1) || (meshType == block_data::Mesh35::Indices16))
            {
                block_data::Mesh35::Indices meshDataValue;
                meshDataValue.index = file_io::helpers::ReadUint32(*this);

                meshData = meshDataValue;
            }
            else
            {
                ThrowError("Unknown mesh type for block type 3", "B3dReaderImpl::DispatchReadMesh35");
            }
        }
        else
        {
            ThrowError("Unknown block type", "B3dReaderImpl::DispatchReadMesh35");
        }
    }

    void ReadBlockData35(Block& block)
    {
        static int cnt = 0;
        cnt += 1;
        if (cnt == 7)
        {
            cnt = 7;
            //__debugbreak();
            //return __DebugB3d();
        }

        block_data::SimpleFaceData35 blockData;

        blockData.center = helpers::ReadVector3(*this);
        blockData.boundingSphereRadius = file_io::helpers::ReadFloat(*this);
        blockData.type = file_io::helpers::ReadUint32(*this);
        blockData.meshIndex = file_io::helpers::ReadUint32(*this);

        const std::uint32_t meshListSize = file_io::helpers::ReadUint32(*this);
        blockData.meshList.resize(meshListSize);
        for (auto& mesh : blockData.meshList)
        {
            mesh.type = file_io::helpers::ReadUint32(*this);
            mesh.unknown0 = file_io::helpers::ReadFloat(*this);
            mesh.unknown1 = file_io::helpers::ReadUint32(*this);
            mesh.materialIndex = file_io::helpers::ReadUint32(*this);

            const std::uint32_t itemsInMesh = file_io::helpers::ReadUint32(*this);
            mesh.meshDataList.resize(itemsInMesh);
            for (auto& meshData : mesh.meshDataList)
            {
                DispatchReadMeshData35(blockData.type, mesh.type, meshData);
            }
        }

        block.data = std::move(blockData);
    }

    void ReadBlockData37(Block& block)
    {
        block_data::GroupIndexAndTextures37 blockData;

        blockData.mayBeCenter = helpers::ReadVector3(*this);
        blockData.mayBeBoundingSphereRadius = file_io::helpers::ReadFloat(*this);
        file_io::helpers::ReadUntil(*this, blockData.mayBeName.begin(), blockData.mayBeName.size());
        blockData.unknownIf2ThenUseUnknown0And1 = file_io::helpers::ReadUint32(*this);

        std::uint32_t dataSize = file_io::helpers::ReadUint32(*this);
        blockData.mayBePositionAndNormalList.resize(dataSize);
        for (auto& item: blockData.mayBePositionAndNormalList)
        {
            item.mayBePosition = helpers::ReadVector3(*this);
            item.mayBeNormal = helpers::ReadVector3(*this);

            if (blockData.unknownIf2ThenUseUnknown0And1 == block_data::GroupIndexAndTextures37::Unknown2)
            {
                item.unknown0 = file_io::helpers::ReadFloat(*this);
                item.unknown1 = file_io::helpers::ReadFloat(*this);
            }
        }

        ReadNestedBlocks(blockData.nestedBlocks);

        block.data = std::move(blockData);
    }
};

void B3dReader::Read(std::istream &input, B3d& data)
{
    B3dReaderImpl reader{input};
    
    reader.Read(data);
}

} // namespace b3d