// Implementation based on https://github.com/AlexKimov/HardTruck-RignRoll-file-formats

#include <d2_hack/resource/data/b3d_reader.h>

#include <cstring>

#include <d2_hack/common/reader.h>

namespace d2_hack
{
namespace resource
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

static const std::uint8_t Signature[FileHeader::SignatureSize] = { 'b', '3', 'd', '\0' };

static const std::uint32_t DataBeginMagic = 111;
static const std::uint32_t DataEndMagic = 222;

static const std::uint32_t BlockBeginMagic = 333;
static const std::uint32_t BlockNextMagic = 444;
static const std::uint32_t BlockEndMagic = 555;







class B3dReaderImpl : private common::Reader
{
public:
    explicit B3dReaderImpl(Ogre::DataStream& input)
        : common::Reader(input)
    {
    }

    B3dTree Read(const B3dRegistryEntry& registryEntry)
    {
        FileHeader fileHeader;
        ReadFileHeader(fileHeader);

        B3dTree res;

        res.registryEntry = registryEntry;
        res.materials = ReadMaterials(fileHeader.materials);
        res.rootNodes = ReadData(fileHeader.data);

        return res;
    }

    common::ResourceName GetProperResourceName(const common::ResourceName& name)
    {
        if (name[0] == 0)
        {
            std::string uniqueName = std::to_string(m_unnamedObjectCounter++);

            common::ResourceName res{0};
            std::copy(uniqueName.begin(), uniqueName.end(), res.begin());
            return res;
        }

        return name;
    }

private:
    int m_unnamedObjectCounter = 0;

    void ReadFileHeader(FileHeader& fileHeader)
    {
        ReadBytes(fileHeader.signature, FileHeader::SignatureSize);
        if (std::memcmp(fileHeader.signature, Signature, FileHeader::SignatureSize) != 0)
        {
            ThrowError("Incorrect b3d signature", "B3dReaderImpl::ReadHeader");
        }

        // file size
        fileHeader.fileSize = ReadUint32() * 4;

        // material section
        fileHeader.materials.offset = ReadUint32() * 4;
        fileHeader.materials.size = ReadUint32() * 4;


        // data section
        fileHeader.data.offset = ReadUint32() * 4;
        fileHeader.data.size = ReadUint32() * 4;
    }

    common::Materials ReadMaterials(const FileHeader::Section& sectionInfo)
    {
        const size_t materialsStartOffset = GetStream().tell();
        if (materialsStartOffset != sectionInfo.offset)
        {
            ThrowError("Incorrect materials offset", "B3dReaderImpl::ReadMaterials");
        }

        common::Materials materials;
        const std::uint32_t materialsCount = ReadUint32();
        ReadCount(materials, materialsCount);

        const size_t materialsEndOffset = GetStream().tell();
        assert(materialsEndOffset > materialsStartOffset);

        if ((materialsEndOffset - materialsStartOffset) != sectionInfo.size)
        {
            ThrowError("Incorrect materials size", "B3dReaderImpl::ReadMaterials");
        }

        return materials;
    }

    NodeList ReadData(const FileHeader::Section& sectionInfo)
    {
        const size_t dataStartOffset = GetStream().tell();
        if (dataStartOffset != sectionInfo.offset)
        {
            ThrowError("Incorrect data offset", "B3dReaderImpl::ReadData");
        }

        const std::uint32_t blockSeparator = ReadUint32();
        if (blockSeparator != DataBeginMagic)
        {
            ThrowError("Incorrect data begin magic", "B3dReaderImpl::ReadData");
        }

        NodeList res;
        bool gotEndOfData = false;
        for (; ; )
        {
            NodePtr node = ReadBlock(gotEndOfData);
            if (gotEndOfData)
            {
                // reached EOF, exiting
                assert(!node);
                break;
            }
            res.push_back(node);
        }

        const size_t dataEndOffset = GetStream().tell();
        assert(dataEndOffset > dataStartOffset);
        // check postconditions
        if ((dataEndOffset - dataStartOffset) != sectionInfo.size)
        {
            ThrowError("Incorrect data size", "B3dReaderImpl::ReadData");
        }

        return res;
    }

    NodePtr DispatchBlock(const block_data::BlockHeader& blockHeader)
    {
        if (blockHeader.type == block_data::EmptyBlock0)
        {
            return ReadBlockData0(blockHeader);
        }
        else if (blockHeader.type == block_data::SimpleObjectConnectorBlock1)
        {
            return ReadBlockData1(blockHeader);
        }
        else if (blockHeader.type == block_data::GroupUnknownBlock2)
        {
            return ReadBlockData2(blockHeader);
        }
        else if (blockHeader.type == block_data::GroupRoadInfraObjectsBlock4)
        {
            return ReadBlockData4(blockHeader);
        }
        else if (blockHeader.type == block_data::GroupObjectsBlock5)
        {
            return ReadBlockData5(blockHeader);
        }
        else if (blockHeader.type == block_data::GroupVertexDataBlock7)
        {
            return ReadBlockData7(blockHeader);
        }
        else if (blockHeader.type == block_data::SimpleFacesBlock8)
        {
            return ReadBlockData8(blockHeader);
        }
        else if (blockHeader.type == block_data::GroupTriggerBlock9)
        {
            return ReadBlockData9(blockHeader);
        }
        else if (blockHeader.type == block_data::GroupLodParametersBlock10)
        {
            return ReadBlockData10(blockHeader);
        }
        else if (blockHeader.type == block_data::GroupUnknownBlock12)
        {
            return ReadBlockData12(blockHeader);
        }
        else if (blockHeader.type == block_data::SimpleTriggerBlock13)
        {
            return ReadBlockData13(blockHeader);
        }
        else if (blockHeader.type == block_data::SimpleUnknownBlock14)
        {
            return ReadBlockData14(blockHeader);
        }
        else if (blockHeader.type == block_data::SimpleObjectConnectorBlock18)
        {
            return ReadBlockData18(blockHeader);
        }
        else if (blockHeader.type == block_data::GroupObjectsBlock19)
        {
            return ReadBlockData19(blockHeader);
        }
        else if (blockHeader.type == block_data::SimpleFlatCollisionBlock20)
        {
            return ReadBlockData20(blockHeader);
        }
        else if (blockHeader.type == block_data::GroupObjectsBlock21)
        {
            return ReadBlockData21(blockHeader);
        }
        else if (blockHeader.type == block_data::SimpleVolumeCollisionBlock23)
        {
            return ReadBlockData23(blockHeader);
        }
        else if (blockHeader.type == block_data::GroupTransformMatrixBlock24)
        {
            return ReadBlockData24(blockHeader);
        }
        else if (blockHeader.type == block_data::SimpleUnknownBlock25)
        {
            return ReadBlockData25(blockHeader);
        }
        else if (blockHeader.type == block_data::SimpleFacesBlock28)
        {
            return ReadBlockData28(blockHeader);
        }
        else if (blockHeader.type == block_data::GroupUnknownBlock29)
        {
            return ReadBlockData29(blockHeader);
        }
        else if (blockHeader.type == block_data::SimplePortalBlock30)
        {
            return ReadBlockData30(blockHeader);
        }
        else if (blockHeader.type == block_data::GroupLightingObjectBlock33)
        {
            return ReadBlockData33(blockHeader);
        }
        else if (blockHeader.type == block_data::SimpleUnknownBlock34)
        {
            return ReadBlockData34(blockHeader);
        }
        else if (blockHeader.type == block_data::SimpleFacesBlock35)
        {
            return ReadBlockData35(blockHeader);
        }
        else if (blockHeader.type == block_data::GroupVertexDataBlock36)
        {
            return ReadBlockData36(blockHeader);
        }
        else if (blockHeader.type == block_data::GroupVertexDataBlock37)
        {
            return ReadBlockData37(blockHeader);
        }
        else if (blockHeader.type == block_data::GroupUnknownBlock39)
        {
        return ReadBlockData39(blockHeader);
        }
        else if (blockHeader.type == block_data::SimpleGeneratedObjectsBlock40)
        {
            return ReadBlockData40(blockHeader);
        }

        ThrowError("Unknown block id", "B3dReaderImpl::ReadBlock");
    }

    NodePtr ReadBlock(bool& gotEndOfData)
    {
        std::uint32_t blockSeparator = ReadUint32();

        while ((blockSeparator == BlockNextMagic) || (blockSeparator == BlockEndMagic))
        {
            // reread new block separator
            blockSeparator = ReadUint32();
        }

        if (blockSeparator == DataEndMagic)
        {
            gotEndOfData = true;
            return NodePtr{};
        }
        if (blockSeparator != BlockBeginMagic)
        {
            ThrowError("Incorrect block begin magic", "B3dReaderImpl::ReadBlock");
        }

        block_data::BlockHeader blockHeader{0};
        ReadBytes(blockHeader.name.data(), blockHeader.name.size());
        // Rename all unnammed objects
        blockHeader.name = GetProperResourceName(blockHeader.name);
        blockHeader.type = ReadUint32();

        if (blockHeader.type > block_data::MaxBlockId)
        {
            ThrowError("Incorrect block id, possible b3d corruption?", "B3dReaderImpl::ReadBlock");
        }

        NodePtr res = DispatchBlock(blockHeader);

        blockSeparator = ReadUint32();
        if ((blockSeparator != BlockNextMagic) && (blockSeparator != BlockEndMagic))
        {
            ThrowError("Incorrect block end magic", "B3dReaderImpl::ReadBlock");
        }

        return res;
    }

    NodePtr ReadNestedBlocks(const NodePtr& parent)
    {
        const std::uint32_t nestedBlocksCount = ReadUint32();
        if (nestedBlocksCount > 1000)
        {
            __debugbreak();
            ThrowError("Unexpectedly big nested size", "B3dReaderImpl::ReadNestedBlocks");
        }

        NodeList childNodes;
        for (std::uint32_t nestedBlockNumber = 0; nestedBlockNumber != nestedBlocksCount; ++nestedBlockNumber)
        {
            bool gotEndOfData = false;
            NodePtr child = ReadBlock(gotEndOfData);
            if (gotEndOfData)
            {
                ThrowError("Unexpected end of data", "B3dReaderImpl::ReadNestedBlocks");
            }
            childNodes.push_back(child);
        }

        parent->SetChildNodes(std::move(childNodes));

        return parent;
    }

    NodePtr ReadBlockData0(const block_data::BlockHeader& blockHeader)
    {
        block_data::Empty0 block;

        ReadBytes(block.emptyData0, sizeof(block.emptyData0));
        block.unknown = ReadFloat();
        ReadBytes(block.emptyData1, sizeof(block.emptyData1));

        return std::make_shared<NodeEmpty0>(blockHeader, block);
    }

    NodePtr ReadBlockData1(const block_data::BlockHeader& blockHeader)
    {
        block_data::SimpleObjectConnector1 block;

        ReadBytes(block.space.data(), block.space.size());
        ReadBytes(block.object.data(), block.object.size());

        return std::make_shared<NodeSimpleObjectConnector1>(blockHeader, block);
    }

    NodePtr ReadBlockData2(const block_data::BlockHeader& blockHeader)
    {
        block_data::GroupUnknown2 block;

        block.boundingSphere = ReadBoundingSphere();
        block.unknown0 = ReadVector3();
        block.unknown1 = ReadFloat();

        NodePtr res = std::make_shared<NodeGroupUnknown2>(blockHeader, block);

        return ReadNestedBlocks(res);
    }

    NodePtr ReadBlockData4(const block_data::BlockHeader& blockHeader)
    {
        block_data::GroupRoadInfraObjects4 block;

        block.boundingSphere = ReadBoundingSphere();

        ReadBytes(block.name.data(), block.name.size());
        ReadBytes(block.data.data(), block.data.size());

        NodePtr res = std::make_shared<NodeGroupRoadInfraObjects4>(blockHeader, block);

        return ReadNestedBlocks(res);
    }

    NodePtr ReadBlockData5(const block_data::BlockHeader& blockHeader)
    {
        block_data::GroupObjects5 block;

        block.boundingSphere = ReadBoundingSphere();
        ReadBytes(block.name.data(), block.name.size());

        NodePtr res = std::make_shared<NodeGroupObjects5>(blockHeader, block);

        return ReadNestedBlocks(res);
    }

    NodePtr ReadBlockData7(const block_data::BlockHeader& blockHeader)
    {
        block_data::GroupVertexData7 block;

        block.boundingSphere = ReadBoundingSphere();
        ReadBytes(block.name.data(), block.name.size());

        const std::uint32_t vertexAmount = ReadUint32();
        for (size_t i = 0; i != vertexAmount; ++i)
        {
            common::Position pos = ReadVector3();
            common::TexCoord texCoord = ReadVector2();

            block.meshInfo.positions.push_back(pos);
            block.meshInfo.texCoords.push_back(texCoord);
        }

        NodePtr res = std::make_shared<NodeGroupVertexData7>(blockHeader, block);

        return ReadNestedBlocks(res);
    }

    void DispatchReadFaceData8(const std::uint32_t itemsInFace, block_data::Face8& face)
    {
        switch (face.type)
        {
        case block_data::Face8::UnknownType0:
        case block_data::Face8::UnknownType1:
        case block_data::Face8::UnknownType16:
        case block_data::Face8::FaceIndexType17:
        case block_data::Face8::FaceIndexType128:
        case block_data::Face8::FaceIndexType129:
        case block_data::Face8::UnknownType144:
        case block_data::Face8::UnknownType145:
        {
            ReadCount(face.meshInfo.indices, itemsInFace);
        }
        break;
        
        case block_data::Face8::FaceIndexType2:
        case block_data::Face8::FaceIndexType3:
        case block_data::Face8::FaceIndexType131:
        {
            for (size_t i = 0; i != itemsInFace; ++i)
            {
                face.meshInfo.indices.push_back(ReadUint32());
                face.meshInfo.texCoords.push_back(ReadVector2());
            }
        }
        break;

        case block_data::Face8::FaceIndexType48:
        case block_data::Face8::FaceIndexType51:
        case block_data::Face8::FaceIndexType176:
        case block_data::Face8::FaceIndexType179:
        {
            for (size_t i = 0; i != itemsInFace; ++i)
            {
                face.meshInfo.indices.push_back(ReadUint32());
                face.meshInfo.normals.push_back(ReadVector3());
            }
        }
        break;

        case block_data::Face8::FaceIndexType50:
        case block_data::Face8::FaceIndexType178:
        {
            for (size_t i = 0; i != itemsInFace; ++i)
            {
                face.meshInfo.indices.push_back(ReadUint32());
                face.meshInfo.texCoords.push_back(ReadVector2());
                face.meshInfo.normals.push_back(ReadVector3());
            }
        }
        break;

        case block_data::Face8::FaceIndexType177:
        {
            for (size_t i = 0; i != itemsInFace; ++i)
            {
                face.meshInfo.indices.push_back(ReadUint32());
                face.unknown177.push_back({ReadFloat()});
            }
        }
        break;
        
        default:
            ThrowError("Unknown face type", "B3dReaderImpl::DispatchReadFaceData8");
        };
    }

    NodePtr ReadBlockData8(const block_data::BlockHeader& blockHeader)
    {
        block_data::SimpleFaces8 block;
        block.boundingSphere = ReadBoundingSphere();

        const std::uint32_t facesCount = ReadUint32();
        for (std::uint32_t faceNumber = 0; faceNumber != facesCount; ++faceNumber)
        {
            block_data::Face8 face;

            face.type = ReadUint32();
            face.unknown0 = ReadFloat();
            face.unknown1 = ReadUint32();
            face.materialIndex = ReadUint32();

            const std::uint32_t itemsInFace = ReadUint32();
            DispatchReadFaceData8(itemsInFace, face);
            block.faces.push_back(face);
        }

        return std::make_shared<NodeSimpleFaces8>(blockHeader, block);
    }

    NodePtr ReadBlockData9(const block_data::BlockHeader& blockHeader)
    {
        block_data::GroupTrigger9 block;

        block.boundingSphere = ReadBoundingSphere();
        block.unknown = ReadVector3();
        block.distanceToPlayer = ReadFloat();

        NodePtr res = std::make_shared<NodeGroupTrigger9>(blockHeader, block);

        return ReadNestedBlocks(res);
    }

    NodePtr ReadBlockData10(const block_data::BlockHeader& blockHeader)
    {
        block_data::GroupLodParameters10 block;

        block.boundingSphere = ReadBoundingSphere();
        block.unknown = ReadVector3();
        block.distanceToPlayer = ReadFloat();

        NodePtr res = std::make_shared<NodeGroupLodParameters10>(blockHeader, block);

        return ReadNestedBlocks(res);
    }

    NodePtr ReadBlockData12(const block_data::BlockHeader& blockHeader)
    {
        block_data::GroupUnknown12 block;

        block.boundingSphere = ReadBoundingSphere();
        block.unknown0 = ReadFloat();
        block.unknown1 = ReadFloat();
        block.unknown2 = ReadFloat();
        block.unknown3 = ReadFloat();
        block.unknown4 = ReadUint32();
        block.unknown5 = ReadUint32();

        NodePtr res = std::make_shared<NodeGroupUnknown12>(blockHeader, block);

        return ReadNestedBlocks(res);
    }

    NodePtr ReadBlockData13(const block_data::BlockHeader& blockHeader)
    {
        block_data::SimpleTrigger13 block;

        block.boundingSphere = ReadBoundingSphere();
        block.unknown0 = ReadUint32();
        block.unknown1 = ReadUint32();

        const std::uint32_t unknown2Count = ReadUint32();
        ReadCount(block.unknown2, unknown2Count);
        
        return std::make_shared<NodeSimpleTrigger13>(blockHeader, block);
    }

    NodePtr ReadBlockData14(const block_data::BlockHeader& blockHeader)
    {
        block_data::SimpleUnknown14 block;
        
        block.boundingSphere = ReadBoundingSphere();
        block.unknown0 = ReadUint32();
        block.unknown1 = ReadUint32();

        block.unknown2 = ReadFloat();
        block.unknown3 = ReadFloat();
        block.unknown4 = ReadFloat();
        block.unknown5 = ReadFloat();
        block.unknown6 = ReadFloat();
        
        return std::make_shared<NodeSimpleUnknown14>(blockHeader, block);
    }

    NodePtr ReadBlockData18(const block_data::BlockHeader& blockHeader)
    {
        block_data::SimpleObjectConnector18 block;

        block.boundingSphere = ReadBoundingSphere();
        ReadBytes(block.space.data(), block.space.size());
        ReadBytes(block.object.data(), block.object.size());

        return std::make_shared<NodeSimpleObjectConnector18>(blockHeader, block);
    }

    NodePtr ReadBlockData19(const block_data::BlockHeader& blockHeader)
    {
        block_data::GroupObjects19 block;

        NodePtr res = std::make_shared<NodeGroupObjects19>(blockHeader, block);
        return ReadNestedBlocks(res);
    }

    NodePtr ReadBlockData20(const block_data::BlockHeader& blockHeader)
    {
        block_data::SimpleFlatCollision20 block;

        block.boundingSphere = ReadBoundingSphere();
        const std::uint32_t countUnknown2 = ReadUint32();

        block.unknown0 = ReadUint32();
        block.unknown1 = ReadUint32();

        const std::uint32_t countUnknown3 = ReadUint32();

        ReadCount(block.unknown2, countUnknown2);
        ReadCount(block.unknown3, countUnknown3);

        return std::make_shared<NodeSimpleFlatCollision20>(blockHeader, block);
    }

    NodePtr ReadBlockData21(const block_data::BlockHeader& blockHeader)
    {
        block_data::GroupObjects21 block;

        block.boundingSphere = ReadBoundingSphere();
        block.count = ReadUint32();
        block.unknown = ReadUint32();

        NodePtr res = std::make_shared<NodeGroupObjects21>(blockHeader, block);
        return ReadNestedBlocks(res);
    }

    NodePtr ReadBlockData23(const block_data::BlockHeader& blockHeader)
    {
        block_data::SimpleVolumeCollision23 block;

        block.unknown0 = ReadUint32();
        block.surfaceType = ReadUint32();

        const std::uint32_t unknown1Count = ReadUint32();
        ReadCount(block.unknown1, unknown1Count);

        const std::uint32_t collisionCount = ReadUint32();
        for (std::uint32_t collisionNumber = 0; collisionNumber != collisionCount; ++collisionNumber)
        {
            const std::uint32_t vertexCount = ReadUint32();
            common::PositionList polygon;
            ReadCount(polygon, vertexCount);

            block.polygons.push_back(polygon);
        }

        return std::make_shared<NodeSimpleVolumeCollision23>(blockHeader, block);
    }

    NodePtr ReadBlockData24(const block_data::BlockHeader& blockHeader)
    {
        block_data::GroupTransformMatrix24 block;

        block.x = ReadVector3();
        block.y = ReadVector3();
        block.z = ReadVector3();

        block.position = ReadVector3();
        block.unknown = ReadUint32();

        NodePtr res = std::make_shared<NodeGroupTransformMatrix24>(blockHeader, block);
        return ReadNestedBlocks(res);
    }

    NodePtr ReadBlockData25(const block_data::BlockHeader& blockHeader)
    {
        block_data::SimpleUnknown25 block;

        block.unknown0 = ReadFloat();
        block.unknown1 = ReadUint32();
        block.unknown2 = ReadUint32();
        ReadBytes(block.soundName.data(), block.soundName.size());
        for (auto& unknown : block.unknown3)
        {
            unknown = ReadFloat();
        }

        return std::make_shared<NodeSimpleUnknown25>(blockHeader, block);
    }

    void DispatchFace28(block_data::Face28& face)
    {
        const std::uint32_t count = ReadUint32();

        if (face.type == block_data::Face28::Unknown2)
        {
            for (size_t i = 0; i != count; ++i)
            {
                face.meshInfo.texCoords.push_back(ReadVector2());
                face.unknown.push_back({ReadVector2()});
            }
        }
        else
        {
            ThrowError("Unknown type", "B3dReaderImpl::DispatchFaceEntry28");
        }
    }

    NodePtr ReadBlockData28(const block_data::BlockHeader& blockHeader)
    {
        block_data::SimpleFaces28 block;

        block.boundingSphere = ReadBoundingSphere();
        block.unknown = ReadVector3();


        const std::uint32_t facesCount = ReadUint32();
        for (std::uint32_t faceNumber = 0; faceNumber != facesCount; ++faceNumber)
        {
            block_data::Face28 data;
            data.type = ReadUint32();
            data.unknown0 = ReadFloat();
            data.unknown1 = ReadUint32();
            data.materialIndex = ReadUint32();

            DispatchFace28(data);
            block.faces.push_back(data);
        }

        return std::make_shared<NodeSimpleFaces28>(blockHeader, block);
    }

    NodePtr ReadBlockData29(const block_data::BlockHeader& blockHeader)
    {
        block_data::GroupUnknown29 block;

        block.boundingSphere = ReadBoundingSphere();
        block.type = ReadUint32();
        block.unknown0 = ReadUint32();

        ReadBytes(block.unknown1.data(), (block.type == 3 ? 7 : 8) * sizeof(block.unknown1[0]));

        NodePtr res = std::make_shared<NodeGroupUnknown29>(blockHeader, block);
        return ReadNestedBlocks(res);
    }

    NodePtr ReadBlockData30(const block_data::BlockHeader& blockHeader)
    {
        block_data::SimplePortal30 block;

        block.boundingSphere = ReadBoundingSphere();
        ReadBytes(block.connectedRoom.data(), block.connectedRoom.size());

        block.leftDown = ReadVector3();
        block.upRight = ReadVector3();

        return std::make_shared<NodeSimplePortal30>(blockHeader, block);
    }

    NodePtr ReadBlockData33(const block_data::BlockHeader& blockHeader)
    {
        block_data::GroupLightingObjects33 block;

        block.boundingSphere = ReadBoundingSphere();
        block.unknown0 = ReadUint32();
        block.unknown1 = ReadUint32();
        block.unknown2 = ReadUint32();
        block.position = ReadVector3();
        for (auto& colorEntry : block.color)
        {
            colorEntry = ReadFloat();
        }

        NodePtr res = std::make_shared<NodeGroupLightingObjects33>(blockHeader, block);
        return ReadNestedBlocks(res);
    }

    NodePtr ReadBlockData34(const block_data::BlockHeader& blockHeader)
    {
        block_data::SimpleUnknown34 block;

        block.boundingSphere = ReadBoundingSphere();
        block.unknown0 = ReadFloat();

        std::uint32_t dataCount = ReadUint32();
        ReadCount(block.data, dataCount);

        return std::make_shared<NodeSimpleUnknown34>(blockHeader, block);
    }

    void DispatchReadFace35(const std::uint32_t blockType, block_data::Face35& face)
    {
        const std::uint32_t dataCount = ReadUint32();

        if (blockType == block_data::SimpleFaces35::Unknown1)
        {
            if (face.type == block_data::Face35::Indices0)
            {
                for (size_t i = 0; i != dataCount; ++i)
                {
                    face.meshInfo.indices.push_back(ReadUint32());
                }
            }
            else if (face.type == block_data::Face35::Unknown2)
            {
                for (size_t i = 0; i != dataCount; ++i)
                {
                    face.meshInfo.indices.push_back(ReadUint32());
                    face.meshInfo.texCoords.push_back(ReadVector2());
                }
            }
            else if (face.type == block_data::Face35::UnknownType48)
            {
                for (size_t i = 0; i != dataCount; ++i)
                {
                    face.meshInfo.indices.push_back(ReadUint32());
                    face.meshInfo.normals.push_back(ReadVector3());
                }
            }
            else if (face.type == block_data::Face35::UnknownType50)
            {
                for (size_t i = 0; i != dataCount; ++i)
                {
                    face.meshInfo.indices.push_back(ReadUint32());
                    face.meshInfo.texCoords.push_back(ReadVector2());
                    face.meshInfo.normals.push_back(ReadVector3());
                }
            }
            else
            {
                ThrowError("Unknown mesh type for block type 1", "B3dReaderImpl::DispatchReadMesh35");
            }
        }
        else if (blockType == block_data::SimpleFaces35::Unknown2)
        {
            if (face.type == block_data::Face35::Indices1)
            {
                for (size_t i = 0; i != dataCount; ++i)
                {
                    face.meshInfo.indices.push_back(ReadUint32());
                }
            }
            else if (face.type == block_data::Face35::UnknownType3)
            {
                for (size_t i = 0; i != dataCount; ++i)
                {
                    face.meshInfo.indices.push_back(ReadUint32());
                    face.meshInfo.texCoords.push_back(ReadVector2());
                }
            }
            else if (face.type == block_data::Face35::UnknownType49)
            {
                for (size_t i = 0; i != dataCount; ++i)
                {
                    face.meshInfo.indices.push_back(ReadUint32());
                    face.unknown49.push_back({ReadFloat()});
                }
            }
            else if (face.type == block_data::Face35::UnknownType51)
            {
                for (size_t i = 0; i != dataCount; ++i)
                {
                    face.meshInfo.indices.push_back(ReadUint32());
                    face.meshInfo.normals.push_back(ReadVector3());
                }
            }
            else
            {
                ThrowError("Unknown mesh type for block type 2", "B3dReaderImpl::DispatchReadMesh35");
            }
        }
        else if (blockType == block_data::SimpleFaces35::IndicesOnly3)
        {
            switch (face.type)
            {
            case block_data::Face35::Indices0:
            case block_data::Face35::Indices1:
            case block_data::Face35::Indices3:
            case block_data::Face35::Indices16:
            case block_data::Face35::Indices17:
            {
                for (size_t i = 0; i != dataCount; ++i)
                {
                    face.meshInfo.indices.push_back(ReadUint32());
                }
            }
            break;

            default:
                ThrowError("Unknown mesh type for block type 3", "B3dReaderImpl::DispatchReadMesh35");
            };
        }
        else
        {
            ThrowError("Unknown block type", "B3dReaderImpl::DispatchReadMesh35");
        }
    }

    NodePtr ReadBlockData35(const block_data::BlockHeader& blockHeader)
    {
        block_data::SimpleFaces35 block;

        block.boundingSphere = ReadBoundingSphere();
        block.type = ReadUint32();
        block.materialIndex = ReadUint32();

        const std::uint32_t meshCount = ReadUint32();
        for (std::uint32_t meshNumber = 0; meshNumber != meshCount; ++meshNumber)
        {
            block_data::Face35 face;
            face.type = ReadUint32();
            face.unknown0 = ReadFloat();
            face.unknown1 = ReadUint32();
            face.materialIndex = ReadUint32();

            DispatchReadFace35(block.type, face);

            block.faces.push_back(face);
        }

        return std::make_shared<NodeSimpleFaces35>(blockHeader, block);
    }

    NodePtr ReadBlockData36(const block_data::BlockHeader& blockHeader)
    {
        block_data::GroupVertexData36 block;

        ReadBytes(block.unknown0.data(), block.unknown0.size());
        block.type = ReadUint32();
        const std::uint32_t count = ReadUint32();
        if (block.type == block_data::GroupVertexData36::Vertex2)
        {
            for (std::uint32_t i = 0; i != count; ++i)
            {
                block.meshInfo.positions.push_back(ReadVector3());
                block.meshInfo.texCoords.push_back(ReadVector2());
                block.meshInfo.normals.push_back(ReadVector3());
            }
        }
        else if (block.type == block_data::GroupVertexData36::Vertex3)
        {
            for (std::uint32_t i = 0; i != count; ++i)
            {
                block.meshInfo.positions.push_back(ReadVector3());
                block.meshInfo.normals.push_back(ReadVector3());
            }
        }
        else
        {
            ThrowError("Unknown type " + std::to_string(block.type), "B3dReaderImpl::ReadBlockData36");
        }

        NodePtr res = std::make_shared<NodeGroupVertexData36>(blockHeader, block);
        return ReadNestedBlocks(res);
    }

    void DispatchVertexData37(block_data::GroupVertexData37& block)
    {
        const std::uint32_t dataSize = ReadUint32();

        if (dataSize == 0)
        {
            if (block.type != 0)
            {
                ThrowError("Non-Null Data cannot be empty", "B3dReaderImpl::DispatchVertexData37");
            }
            return;
        }

        if (block.type == block_data::GroupVertexData37::Vertex2)
        {
            for (size_t i = 0; i != dataSize; ++i)
            {
                block.meshInfo.positions.push_back(ReadVector3());
                block.meshInfo.texCoords.push_back(ReadVector2());
                block.meshInfo.normals.push_back(ReadVector3());
            }
        }
        else if (block.type == block_data::GroupVertexData37::Vertex3)
        {
            for (size_t i = 0; i != dataSize; ++i)
            {
                block.meshInfo.positions.push_back(ReadVector3());
                block.meshInfo.normals.push_back(ReadVector3());
            }
        }
        else if (block.type == block_data::GroupVertexData37::UnknownType514)
        {
            for (size_t i = 0; i != dataSize; ++i)
            {
                block.meshInfo.positions.push_back(ReadVector3());
                block.meshInfo.texCoords.push_back(ReadVector2());
                block.meshInfo.normals.push_back(ReadVector3());
                block.unknown514.push_back({ReadVector4()});
            }
        }
        else if (block.type == block_data::GroupVertexData37::UnknownType258)
        {
            for (size_t i = 0; i != dataSize; ++i)
            {
                block.meshInfo.positions.push_back(ReadVector3());
                block.meshInfo.texCoords.push_back(ReadVector2());
                block.meshInfo.normals.push_back(ReadVector3());
                block.unknown258Or515.push_back({ReadVector2()});
            }
        }
        else if (block.type == block_data::GroupVertexData37::UnknownType515)
        {
            for (size_t i = 0; i != dataSize; ++i)
            {
                block.meshInfo.positions.push_back(ReadVector3());
                block.meshInfo.texCoords.push_back(ReadVector2());
                block.meshInfo.normals.push_back(ReadVector3());
                block.unknown258Or515.push_back({ ReadVector2() });
            }
        }
        else
        {
            ThrowError("Unknown vertex type", "B3dReaderImpl::DispatchVertexData37");
        }
    }

    NodePtr ReadBlockData37(const block_data::BlockHeader& blockHeader)
    {
        block_data::GroupVertexData37 block;

        block.boundingSphere = ReadBoundingSphere();
        ReadBytes(block.name.data(), block.name.size());
        block.type = ReadUint32();

        DispatchVertexData37(block);

        NodePtr res = std::make_shared<NodeGroupVertexData37>(blockHeader, block);
        return ReadNestedBlocks(res);
    }

    NodePtr ReadBlockData39(const block_data::BlockHeader& blockHeader)
    {
        block_data::GroupUnknown39 block;

        ReadBytes(block.unknown.data(), block.unknown.size());

        NodePtr res = std::make_shared<NodeGroupUnknown39>(blockHeader, block);
        return ReadNestedBlocks(res);
    }

    NodePtr ReadBlockData40(const block_data::BlockHeader& blockHeader)
    {
        block_data::SimpleGeneratedObjects40 block;

        block.boundingSphere = ReadBoundingSphere();
        ReadBytes(block.empty.data(), block.empty.size());
        ReadBytes(block.name.data(), block.name.size());

        block.type = ReadUint32();
        block.unknown0 = ReadFloat();

        const std::uint32_t unknown1Size = ReadUint32();
        ReadCount(block.unknown1, unknown1Size);

        return std::make_shared<NodeSimpleGeneratedObjects40>(blockHeader, block);
    }
};

B3dForest ReadB3d(const B3dRegistry& registry)
{
    B3dForest forest;
    for (const auto& entry : registry.entries)
    {
        const std::string fullB3dName = registry.rootDir + "/" + entry.dir + "/" + entry.id + ".b3d";
        std::ifstream inputFile{ fullB3dName, std::ios_base::binary };
        if (!inputFile)
        {
            OGRE_EXCEPT(Ogre::Exception::ERR_FILE_NOT_FOUND, "failed to open file");
        }

        Ogre::FileStreamDataStream dataStream(&inputFile, false);
        B3dReaderImpl reader{ dataStream };

        forest.push_back(reader.Read(entry));
    }

    return forest;
}

} // namespace b3d
} // namespace data
} // namespace resource
} // namespace d2_hack
