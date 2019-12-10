// Implementation based on https://github.com/AlexKimov/HardTruck-RignRoll-file-formats

#include "d2_b3d_reader.h"

#include <cstring>
#include <cctype>

#include "d2_hack_common.h"


#define ENABLE_TRACE_B3D

#ifdef ENABLE_TRACE_B3D
#include <iostream>

#define B3D_LOG(data) {std::string off; off.resize(m_trace_offset * 4, ' '); std::cout << off << data << std::endl;}
#define INIT_TRACE_OFFSET() , m_trace_offset(0)
#define DECLARE_TRACE_OFFSET() size_t m_trace_offset

#define INC_TRACE_OFFSET() m_trace_offset += 1
#define DEC_TRACE_OFFSET() m_trace_offset -= 1

#else //ENABLE_TRACE_B3D

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
static const std::uint32_t SimpleFacesBlock8 = 8;
static const std::uint32_t GroupTriggerBlock9 = 9;
static const std::uint32_t GroupLodParametersBlock10 = 10;
static const std::uint32_t SimpleTriggerBlock13 = 13;
static const std::uint32_t SimpleFlatCollisionBlock20 = 20;
static const std::uint32_t GroupObjectsBlock21 = 21;
static const std::uint32_t GroupTransformMatrixBlock24 = 24;
static const std::uint32_t GroupLightingObjectBlock33 = 33;
static const std::uint32_t SimpleFaceDataBlock35 = 35;
static const std::uint32_t GroupIndexAndTexturesBlock37 = 37;

static const std::uint32_t MaxBlockId = 40;


class B3dReaderImpl : public file_io::Reader
{
public:
    explicit B3dReaderImpl(std::istream& input)
        : file_io::Reader(input)
        INIT_TRACE_OFFSET()
    {
    }

    void Read(B3d& data)
    {
        ReadFileHeader(data.header);

        ReadMaterials(data.header.materials, data.materials);
        ReadData(data.header.data, data.data);
    }

private:
    DECLARE_TRACE_OFFSET();

    template <typename OutputIteratorT>
    void ReadUntil(OutputIteratorT res, size_t count)
    {
        file_io::helpers::ReadUntil(*this, res, count);
    }

    std::uint32_t ReadUint32()
    {
        return file_io::helpers::ReadUint32(*this);
    }

    Ogre::Real ReadFloat()
    {
        return file_io::helpers::ReadFloat(*this);
    }

    Ogre::Vector3 ReadVector3()
    {
        Ogre::Vector3 res;

        res.x = ReadFloat();
        res.y = ReadFloat();
        res.z = ReadFloat();

        return res;
    }

    Ogre::Vector2 ReadVector2()
    {
        Ogre::Vector2 res;

        res.x = ReadFloat();
        res.y = ReadFloat();

        return res;
    }

    void ReadFileHeader(FileHeader& fileHeader)
    {
        ReadUntil(fileHeader.signature, FileHeader::SignatureSize);
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

        const std::uint32_t materialsCount = ReadUint32();
        res.resize(materialsCount);
        for (auto& material : res)
        {
            ReadUntil(material.begin(), material.size());
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

        const std::uint32_t blockSeparator = ReadUint32();
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
        else if (block.header.type == SimpleFacesBlock8)
        {
            return ReadBlockData8(block);
        }
        else if (block.header.type == GroupTriggerBlock9)
        {
            return ReadBlockData9(block);
        }
        else if (block.header.type == GroupLodParametersBlock10)
        {
            return ReadBlockData10(block);
        }
        else if (block.header.type == SimpleTriggerBlock13)
        {
            return ReadBlockData13(block);
        }
        else if (block.header.type == SimpleFlatCollisionBlock20)
        {
            return ReadBlockData20(block);
        }
        else if (block.header.type == GroupObjectsBlock21)
        {
            return ReadBlockData21(block);
        }
        else if (block.header.type == GroupTransformMatrixBlock24)
        {
            return ReadBlockData24(block);
        }
        else if (block.header.type == GroupLightingObjectBlock33)
        {
            return ReadBlockData33(block);
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
                ReadUntil(&c, 1);
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
        std::uint32_t blockSeparator = ReadUint32();
        B3D_LOG(blockSeparator);
        while ((blockSeparator == BlockNextMagic) || (blockSeparator == BlockEndMagic))
        {
            // reread new block separator
            blockSeparator = ReadUint32();
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

        INC_TRACE_OFFSET();

        ReadUntil(block.header.name.begin(), StringSize);
        block.header.type = ReadUint32();

        if (block.header.type > MaxBlockId)
        {
            ThrowError("Incorrect block id, possible b3d corruption?", "B3dReaderImpl::ReadBlock");
        }

        B3D_LOG("name: [" << block.header.name.data() << "]");
        B3D_LOG("type: " << block.header.type);

        DispatchBlock(block);

        DEC_TRACE_OFFSET();

        blockSeparator = ReadUint32();
        B3D_LOG(blockSeparator);
        if ((blockSeparator != BlockNextMagic) && (blockSeparator != BlockEndMagic))
        {
            ThrowError("Incorrect block end magic", "B3dReaderImpl::ReadBlock");
        }
    }

    void ReadNestedBlocks(NestedBlockList& nestedBlocks)
    {
        const std::uint32_t nestedBlocksCount = ReadUint32();
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

        ReadUntil(blockData.emptyData0, sizeof(blockData.emptyData0));
        blockData.unknown = ReadFloat();
        ReadUntil(blockData.emptyData1, sizeof(blockData.emptyData1));

        block.data = std::move(blockData);
    }

    void ReadBlockData5(Block& block)
    {
        block_data::GroupObjects5 blockData;

        blockData.center = ReadVector3();
        blockData.boundingSphereRadius = ReadFloat();
        ReadUntil(blockData.name.begin(), blockData.name.size());

        ReadNestedBlocks(blockData.nestedBlocks);

        block.data = std::move(blockData);
    }

    void ReadBlockData7(Block& block)
    {
        block_data::GroupVertex7 blockData;

        blockData.center = ReadVector3();
        blockData.boundingSphereRadius = ReadFloat();
        ReadUntil(blockData.name.begin(), blockData.name.size());

        const std::uint32_t vertexAmount = ReadUint32();
        blockData.vertices.resize(vertexAmount);
        for (auto& vertexEntry : blockData.vertices)
        {
            vertexEntry.point = ReadVector3();
            vertexEntry.textureCoord = ReadVector2();
        }

        ReadNestedBlocks(blockData.nestedBlocks);

        block.data = std::move(blockData);
    }

    void DispatchReadFaceData8(const std::uint32_t faceType, block_data::Face8::FaceData& faceData)
    {
        if (faceType == block_data::Face8::FaceIndexType129)
        {
            block_data::Face8::FaceIndex faceDataValue;
            faceDataValue.faceIndex = ReadUint32();

            faceData = std::move(faceDataValue);
        }
        else if (faceType == block_data::Face8::UnknownType144)
        {
            block_data::Face8::Unknown144 faceDataValue;
            faceDataValue.unknown = ReadUint32();

            faceData = std::move(faceDataValue);
        }
        else
        {
            ThrowError("Unknown face type", "B3dReaderImpl::DispatchReadFaceData8");
        }
    }

    void ReadBlockData8(Block& block)
    {
        block_data::SimpleFaces8 blockData;

        blockData.center = ReadVector3();
        blockData.boundingSphereRadius = ReadFloat();

        const std::uint32_t facesCount = ReadUint32();
        blockData.faces.resize(facesCount);

        for (auto& face : blockData.faces)
        {
            face.type = ReadUint32();
            face.unknown0 = ReadFloat();
            face.unknown1 = ReadUint32();
            face.unknown2 = ReadUint32();

            const std::uint32_t itemsInFace = ReadUint32();
            face.faceDataList.resize(itemsInFace);
            for (auto& faceData : face.faceDataList)
            {
                DispatchReadFaceData8(face.type, faceData);
            }
        }

        block.data = std::move(blockData);
    }

    void ReadBlockData9(Block& block)
    {
        block_data::GroupTrigger9 blockData;

        blockData.center = ReadVector3();
        blockData.boundingSphereRadius = ReadFloat();

        blockData.unknown = ReadVector3();
        blockData.distanceToPlayer = ReadFloat();

        ReadNestedBlocks(blockData.nestedBlocks);

        block.data = std::move(blockData);
    }

    void ReadBlockData10(Block& block)
    {
        block_data::GroupLodParameters10 blockData;

        blockData.center = ReadVector3();
        blockData.boundingSphereRadius = ReadFloat();

        blockData.unknown = ReadVector3();
        blockData.distanceToPlayer = ReadFloat();

        ReadNestedBlocks(blockData.nestedBlocks);

        block.data = std::move(blockData);
    }

    void ReadBlockData13(Block& block)
    {
        block_data::SimpleTrigger13 blockData;

        blockData.center = ReadVector3();
        blockData.boundingSphereRadius = ReadFloat();

        blockData.unknown0 = ReadUint32();
        blockData.unknown1 = ReadUint32();
        
        const std::uint32_t unknown2Count = ReadUint32();
        blockData.unknown2.resize(unknown2Count);
        for (auto& u : blockData.unknown2)
        {
            u = ReadFloat();
        }

        block.data = std::move(blockData);
    }

    void ReadBlockData20(Block& block)
    {
        block_data::SimpleFlatCollision20 blockData;

        blockData.center = ReadVector3();
        blockData.boundingSphereRadius = ReadFloat();

        const std::uint32_t countA = ReadUint32();

        blockData.unknown0 = ReadUint32();
        blockData.unknown1 = ReadUint32();

        const std::uint32_t countB = ReadUint32();

        blockData.a.resize(countA);
        for (auto& a : blockData.a)
        {
            a = ReadVector3();
        }

        blockData.b.resize(countB);
        for (auto& b : blockData.b)
        {
            b = ReadFloat();
        }

        block.data = std::move(blockData);
    }

    void ReadBlockData21(Block& block)
    {
        block_data::GroupObjects21 blockData;

        blockData.center = ReadVector3();
        blockData.boundingSphereRadius = ReadFloat();
        blockData.count = ReadUint32();
        blockData.unknown = ReadUint32();

        ReadNestedBlocks(blockData.nestedBlocks);

        block.data = std::move(blockData);
    }

    void ReadBlockData24(Block& block)
    {
        block_data::GroupTransformMatrix24 blockData;

        blockData.x = ReadVector3();
        blockData.y = ReadVector3();
        blockData.z = ReadVector3();

        blockData.position = ReadVector3();

        blockData.unknown = ReadUint32();

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

                meshDataValue.faceIndex = ReadUint32();
                meshDataValue.position = ReadVector3();

                meshData = meshDataValue;
            }
            else if (meshType == block_data::Mesh35::UnknownType50)
            {
                block_data::Mesh35::Unknown50 meshDataValue;

                meshDataValue.faceIndex = ReadUint32();
                meshDataValue.position = ReadVector3();
                meshDataValue.texCoord = ReadVector2();

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
                meshDataValue.index = ReadUint32();

                meshData = meshDataValue;
            }
            else if (meshType == block_data::Mesh35::UnknownType49)
            {
                block_data::Mesh35::Unknown49 meshDataValue;

                meshDataValue.unknown0 = ReadUint32();
                meshDataValue.unknown1 = ReadFloat();

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
                meshDataValue.index = ReadUint32();

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

    void ReadBlockData33(Block& block)
    {
        block_data::GroupLightingObjects33 blockData;

        blockData.center = ReadVector3();
        blockData.boundingSphereRadius = ReadFloat();

        blockData.unknown0 = ReadUint32();
        blockData.unknown1 = ReadUint32();
        blockData.unknown2 = ReadUint32();

        blockData.position = ReadVector3();
        for (auto& colorEntry : blockData.color)
        {
            colorEntry = ReadFloat();
        }

        ReadNestedBlocks(blockData.nestedBlocks);

        block.data = std::move(blockData);
    }

    void ReadBlockData35(Block& block)
    {
        block_data::SimpleFaceData35 blockData;

        blockData.center = ReadVector3();
        blockData.boundingSphereRadius = ReadFloat();
        blockData.type = ReadUint32();
        blockData.meshIndex = ReadUint32();

        const std::uint32_t meshListSize = ReadUint32();
        blockData.meshList.resize(meshListSize);
        for (auto& mesh : blockData.meshList)
        {
            mesh.type = ReadUint32();
            mesh.unknown0 = ReadFloat();
            mesh.unknown1 = ReadUint32();
            mesh.materialIndex = ReadUint32();

            const std::uint32_t itemsInMesh = ReadUint32();
            mesh.meshDataList.resize(itemsInMesh);
            for (auto& meshData : mesh.meshDataList)
            {
                DispatchReadMeshData35(blockData.type, mesh.type, meshData);
            }
        }

        block.data = std::move(blockData);
    }

    void DispatchVertexData37(const std::uint32_t type, block_data::GroupVertexData37::VertexData& vertexData)
    {
        if (type == block_data::GroupVertexData37::Vertex2)
        {
            block_data::GroupVertexData37::Vertex vertexDataValue;

            vertexDataValue.position = ReadVector3();
            vertexDataValue.texCoord = ReadVector2();
            vertexDataValue.normal = ReadVector3();

            vertexData = std::move(vertexDataValue);
        }
        else if (type == block_data::GroupVertexData37::UnknownType3)
        {
            block_data::GroupVertexData37::Unknown3 vertexDataValue;

            vertexDataValue.unknown0 = ReadVector3();
            vertexDataValue.unknown1 = ReadVector3();

            vertexData = std::move(vertexDataValue);
        }
        else
        {
            ThrowError("Unknown vertex type", "B3dReaderImpl::DispatchVertexData37");
        }
    }

    void ReadBlockData37(Block& block)
    {
        block_data::GroupVertexData37 blockData;

        blockData.center = ReadVector3();
        blockData.boundingSphereRadius = ReadFloat();
        ReadUntil(blockData.name.begin(), blockData.name.size());
        blockData.type = ReadUint32();

        if (blockData.type == 2)
        {
        //    return __DebugB3d();
        }

        const std::uint32_t dataSize = ReadUint32();
        blockData.vertexDataList.resize(dataSize);
        for (auto& vertexData : blockData.vertexDataList)
        {
            DispatchVertexData37(blockData.type, vertexData);
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