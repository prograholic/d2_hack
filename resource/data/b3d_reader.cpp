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

    B3dTree Read()
    {
        FileHeader fileHeader;
        ReadFileHeader(fileHeader);

        B3dTree res;

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
            NodePtr node = ReadBlock(NodePtr{}, gotEndOfData);
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

    NodePtr DispatchBlock(const block_data::BlockHeader& blockHeader, const NodePtr& parent)
    {
        if (blockHeader.type == block_data::EmptyBlock0)
        {
            return ReadBlockData0(blockHeader, parent);
        }
        else if (blockHeader.type == block_data::GroupUnknownBlock2)
        {
            return ReadBlockData2(blockHeader, parent);
        }
        else if (blockHeader.type == block_data::GroupRoadInfraObjectsBlock4)
        {
            return ReadBlockData4(blockHeader, parent);
        }
        else if (blockHeader.type == block_data::GroupObjectsBlock5)
        {
            return ReadBlockData5(blockHeader, parent);
        }
        else if (blockHeader.type == block_data::GroupVertexBlock7)
        {
            return ReadBlockData7(blockHeader, parent);
        }
        else if (blockHeader.type == block_data::SimpleFacesBlock8)
        {
            return ReadBlockData8(blockHeader, parent);
        }
        else if (blockHeader.type == block_data::GroupTriggerBlock9)
        {
            return ReadBlockData9(blockHeader, parent);
        }
        else if (blockHeader.type == block_data::GroupLodParametersBlock10)
        {
            return ReadBlockData10(blockHeader, parent);
        }
        else if (blockHeader.type == block_data::GroupUnknownBlock12)
        {
            return ReadBlockData12(blockHeader, parent);
        }
        else if (blockHeader.type == block_data::SimpleTriggerBlock13)
        {
            return ReadBlockData13(blockHeader, parent);
        }
        else if (blockHeader.type == block_data::SimpleUnknownBlock14)
        {
            return ReadBlockData14(blockHeader, parent);
        }
        else if (blockHeader.type == block_data::SimpleObjectConnectorBlock18)
        {
            return ReadBlockData18(blockHeader, parent);
        }
        else if (blockHeader.type == block_data::GroupObjectsBlock19)
        {
            return ReadBlockData19(blockHeader, parent);
        }
        else if (blockHeader.type == block_data::SimpleFlatCollisionBlock20)
        {
            return ReadBlockData20(blockHeader, parent);
        }
        else if (blockHeader.type == block_data::GroupObjectsBlock21)
        {
            return ReadBlockData21(blockHeader, parent);
        }
        else if (blockHeader.type == block_data::SimpleVolumeCollisionBlock23)
        {
            return ReadBlockData23(blockHeader, parent);
        }
        else if (blockHeader.type == block_data::GroupTransformMatrixBlock24)
        {
            return ReadBlockData24(blockHeader, parent);
        }
        else if (blockHeader.type == block_data::SimpleUnknownBlock25)
        {
            return ReadBlockData25(blockHeader, parent);
        }
        else if (blockHeader.type == block_data::SimpleFacesBlock28)
        {
            return ReadBlockData28(blockHeader, parent);
        }
        else if (blockHeader.type == block_data::GroupUnknownBlock29)
        {
            return ReadBlockData29(blockHeader, parent);
        }
        else if (blockHeader.type == block_data::SimplePortalBlock30)
        {
            return ReadBlockData30(blockHeader, parent);
        }
        else if (blockHeader.type == block_data::GroupLightingObjectBlock33)
        {
            return ReadBlockData33(blockHeader, parent);
        }
        else if (blockHeader.type == block_data::SimpleUnknownBlock34)
        {
            return ReadBlockData34(blockHeader, parent);
        }
        else if (blockHeader.type == block_data::SimpleFacesBlock35)
        {
            return ReadBlockData35(blockHeader, parent);
        }
        else if (blockHeader.type == block_data::GroupIndexAndTexturesBlock37)
        {
            return ReadBlockData37(blockHeader, parent);
        }
        else if (blockHeader.type == block_data::SimpleGeneratedObjectsBlock40)
        {
            return ReadBlockData40(blockHeader, parent);
        }

        ThrowError("Unknown block id", "B3dReaderImpl::ReadBlock");
    }

    NodePtr ReadBlock(const NodePtr& parent, bool& gotEndOfData)
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

        NodePtr res = DispatchBlock(blockHeader, parent);

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
            NodePtr child = ReadBlock(parent, gotEndOfData);
            if (gotEndOfData)
            {
                ThrowError("Unexpected end of data", "B3dReaderImpl::ReadNestedBlocks");
            }
            childNodes.push_back(child);
        }

        parent->SetChildNodes(std::move(childNodes));

        return parent;
    }

    NodePtr ReadBlockData0(const block_data::BlockHeader& blockHeader, const NodePtr& parent)
    {
        block_data::Empty0 block;

        ReadBytes(block.emptyData0, sizeof(block.emptyData0));
        block.unknown = ReadFloat();
        ReadBytes(block.emptyData1, sizeof(block.emptyData1));

        return std::make_shared<NodeEmpty0>(blockHeader, block, parent);
    }

    NodePtr ReadBlockData2(const block_data::BlockHeader& blockHeader, const NodePtr& parent)
    {
        block_data::GroupUnknown2 block;

        block.boundingSphere = ReadBoundingSphere();
        block.unknown0 = ReadVector3();
        block.unknown1 = ReadFloat();

        NodePtr res = std::make_shared<NodeGroupUnknown2>(blockHeader, block, parent);

        return ReadNestedBlocks(res);
    }

    NodePtr ReadBlockData4(const block_data::BlockHeader& blockHeader, const NodePtr& parent)
    {
        block_data::GroupRoadInfraObjects4 block;

        block.boundingSphere = ReadBoundingSphere();

        ReadBytes(block.name.data(), block.name.size());
        ReadBytes(block.data.data(), block.data.size());

        NodePtr res = std::make_shared<NodeGroupRoadInfraObjects4>(blockHeader, block, parent);

        return ReadNestedBlocks(res);
    }

    NodePtr ReadBlockData5(const block_data::BlockHeader& blockHeader, const NodePtr& parent)
    {
        block_data::GroupObjects5 block;

        block.boundingSphere = ReadBoundingSphere();
        ReadBytes(block.name.data(), block.name.size());

        NodePtr res = std::make_shared<NodeGroupObjects5>(blockHeader, block, parent);

        return ReadNestedBlocks(res);
    }

    NodePtr ReadBlockData7(const block_data::BlockHeader& blockHeader, const NodePtr& parent)
    {
        block_data::GroupVertex7 block;

        block.boundingSphere = ReadBoundingSphere();
        ReadBytes(block.name.data(), block.name.size());

        const std::uint32_t vertexAmount = ReadUint32();
        ReadCount(block.vertices, vertexAmount);

        NodePtr res = std::make_shared<NodeGroupVertex7>(blockHeader, block, parent);

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
            common::IndexList data;
            ReadCount(data, itemsInFace);

            face.data = std::move(data);
        }
        break;
        
        case block_data::Face8::FaceIndexType2:
        case block_data::Face8::FaceIndexType3:
        case block_data::Face8::FaceIndexType131:
        {
            common::IndexWithTexCoordList data;
            ReadCount(data, itemsInFace);

            face.data = std::move(data);
        }
        break;

        case block_data::Face8::FaceIndexType48:
        case block_data::Face8::FaceIndexType51:
        case block_data::Face8::FaceIndexType176:
        case block_data::Face8::FaceIndexType179:
        {
            common::IndexWithPositionList data;
            ReadCount(data, itemsInFace);

            face.data = std::move(data);
        }
        break;

        case block_data::Face8::FaceIndexType50:
        case block_data::Face8::FaceIndexType178:
        {
            common::IndexWithPositionTexCoordList data;
            ReadCount(data, itemsInFace);

            face.data = std::move(data);
        }
        break;

        case block_data::Face8::FaceIndexType177:
        {
            common::TexCoordList data;
            ReadCount(data, itemsInFace);

            face.data = std::move(data);
        }
        break;
        
        default:
            ThrowError("Unknown face type", "B3dReaderImpl::DispatchReadFaceData8");
        };
    }

    NodePtr ReadBlockData8(const block_data::BlockHeader& blockHeader, const NodePtr& parent)
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

        return std::make_shared<NodeSimpleFaces8>(blockHeader, block, parent);
    }

    NodePtr ReadBlockData9(const block_data::BlockHeader& blockHeader, const NodePtr& parent)
    {
        block_data::GroupTrigger9 block;

        block.boundingSphere = ReadBoundingSphere();
        block.unknown = ReadVector3();
        block.distanceToPlayer = ReadFloat();

        NodePtr res = std::make_shared<NodeGroupTrigger9>(blockHeader, block, parent);

        return ReadNestedBlocks(res);
    }

    NodePtr ReadBlockData10(const block_data::BlockHeader& blockHeader, const NodePtr& parent)
    {
        block_data::GroupLodParameters10 block;

        block.boundingSphere = ReadBoundingSphere();
        block.unknown = ReadVector3();
        block.distanceToPlayer = ReadFloat();

        NodePtr res = std::make_shared<NodeGroupLodParameters10>(blockHeader, block, parent);

        return ReadNestedBlocks(res);
    }

    NodePtr ReadBlockData12(const block_data::BlockHeader& blockHeader, const NodePtr& parent)
    {
        block_data::GroupUnknown12 block;

        block.boundingSphere = ReadBoundingSphere();
        block.unknown0 = ReadFloat();
        block.unknown1 = ReadFloat();
        block.unknown2 = ReadFloat();
        block.unknown3 = ReadFloat();
        block.unknown4 = ReadUint32();
        block.unknown5 = ReadUint32();

        NodePtr res = std::make_shared<NodeGroupUnknown12>(blockHeader, block, parent);

        return ReadNestedBlocks(res);
    }

    NodePtr ReadBlockData13(const block_data::BlockHeader& blockHeader, const NodePtr& parent)
    {
        block_data::SimpleTrigger13 block;

        block.boundingSphere = ReadBoundingSphere();
        block.unknown0 = ReadUint32();
        block.unknown1 = ReadUint32();

        const std::uint32_t unknown2Count = ReadUint32();
        ReadCount(block.unknown2, unknown2Count);
        
        return std::make_shared<NodeSimpleTrigger13>(blockHeader, block, parent);
    }

    NodePtr ReadBlockData14(const block_data::BlockHeader& blockHeader, const NodePtr& parent)
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
        
        return std::make_shared<NodeSimpleUnknown14>(blockHeader, block, parent);
    }

    NodePtr ReadBlockData18(const block_data::BlockHeader& blockHeader, const NodePtr& parent)
    {
        block_data::SimpleObjectConnector18 block;

        block.boundingSphere = ReadBoundingSphere();
        ReadBytes(block.space.data(), block.space.size());
        ReadBytes(block.object.data(), block.object.size());

        return std::make_shared<NodeSimpleObjectConnector18>(blockHeader, block, parent);
    }

    NodePtr ReadBlockData19(const block_data::BlockHeader& blockHeader, const NodePtr& parent)
    {
        block_data::GroupObjects19 block;

        NodePtr res = std::make_shared<NodeGroupObjects19>(blockHeader, block, parent);
        return ReadNestedBlocks(res);
    }

    NodePtr ReadBlockData20(const block_data::BlockHeader& blockHeader, const NodePtr& parent)
    {
        block_data::SimpleFlatCollision20 block;

        block.boundingSphere = ReadBoundingSphere();
        const std::uint32_t countUnknown2 = ReadUint32();

        block.unknown0 = ReadUint32();
        block.unknown1 = ReadUint32();

        const std::uint32_t countUnknown3 = ReadUint32();

        ReadCount(block.unknown2, countUnknown2);
        ReadCount(block.unknown3, countUnknown3);

        return std::make_shared<NodeSimpleFlatCollision20>(blockHeader, block, parent);
    }

    NodePtr ReadBlockData21(const block_data::BlockHeader& blockHeader, const NodePtr& parent)
    {
        block_data::GroupObjects21 block;

        block.boundingSphere = ReadBoundingSphere();
        block.count = ReadUint32();
        block.unknown = ReadUint32();

        NodePtr res = std::make_shared<NodeGroupObjects21>(blockHeader, block, parent);
        return ReadNestedBlocks(res);
    }

    NodePtr ReadBlockData23(const block_data::BlockHeader& blockHeader, const NodePtr& parent)
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

        return std::make_shared<NodeSimpleVolumeCollision23>(blockHeader, block, parent);
    }

    NodePtr ReadBlockData24(const block_data::BlockHeader& blockHeader, const NodePtr& parent)
    {
        block_data::GroupTransformMatrix24 block;

        block.x = ReadVector3();
        block.y = ReadVector3();
        block.z = ReadVector3();

        block.position = ReadVector3();
        block.unknown = ReadUint32();

        NodePtr res = std::make_shared<NodeGroupTransformMatrix24>(blockHeader, block, parent);
        return ReadNestedBlocks(res);
    }

    NodePtr ReadBlockData25(const block_data::BlockHeader& blockHeader, const NodePtr& parent)
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

        return std::make_shared<NodeSimpleUnknown25>(blockHeader, block, parent);
    }

    void DispatchFace28(block_data::Face28& face)
    {
        if (face.type == block_data::Face28::Unknown2)
        {
            const std::uint32_t count = ReadUint32();
            
            std::vector<block_data::Face28::Unknown> data;
            ReadCount(data, count);

            face.data = std::move(data);
        }
        else
        {
            ThrowError("Unknown type", "B3dReaderImpl::DispatchFaceEntry28");
        }
    }

    NodePtr ReadBlockData28(const block_data::BlockHeader& blockHeader, const NodePtr& parent)
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

        return std::make_shared<NodeSimpleFaces28>(blockHeader, block, parent);
    }

    NodePtr ReadBlockData29(const block_data::BlockHeader& blockHeader, const NodePtr& parent)
    {
        block_data::GroupUnknown29 block;

        block.boundingSphere = ReadBoundingSphere();
        block.type = ReadUint32();
        block.unknown0 = ReadUint32();

        ReadBytes(block.unknown1.data(), (block.type == 3 ? 7 : 8) * sizeof(block.unknown1[0]));

        NodePtr res = std::make_shared<NodeGroupUnknown29>(blockHeader, block, parent);
        return ReadNestedBlocks(res);
    }

    NodePtr ReadBlockData30(const block_data::BlockHeader& blockHeader, const NodePtr& parent)
    {
        block_data::SimplePortal30 block;

        block.boundingSphere = ReadBoundingSphere();
        ReadBytes(block.connectedRoom.data(), block.connectedRoom.size());

        block.leftDown = ReadVector3();
        block.upRight = ReadVector3();

        return std::make_shared<NodeSimplePortal30>(blockHeader, block, parent);
    }

    NodePtr ReadBlockData33(const block_data::BlockHeader& blockHeader, const NodePtr& parent)
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

        NodePtr res = std::make_shared<NodeGroupLightingObjects33>(blockHeader, block, parent);
        return ReadNestedBlocks(res);
    }

    NodePtr ReadBlockData34(const block_data::BlockHeader& blockHeader, const NodePtr& parent)
    {
        block_data::SimpleUnknown34 block;

        block.boundingSphere = ReadBoundingSphere();
        block.unknown0 = ReadFloat();

        std::uint32_t dataCount = ReadUint32();
        ReadCount(block.data, dataCount);

        return std::make_shared<NodeSimpleUnknown34>(blockHeader, block, parent);
    }

    void DispatchReadFace35(const std::uint32_t blockType, block_data::Face35& face)
    {
        const std::uint32_t dataCount = ReadUint32();

        if (blockType == block_data::SimpleFaces35::Unknown1)
        {
            if (face.type == block_data::Face35::Indices0)
            {
                common::IndexList data;
                ReadCount(data, dataCount);

                face.data = std::move(data);
            }
            else if (face.type == block_data::Face35::Unknown2)
            {
                common::IndexWithTexCoordList data;
                ReadCount(data, dataCount);

                face.data = std::move(data);
            }
            else if (face.type == block_data::Face35::UnknownType48)
            {
                common::IndexWithPositionList data;
                ReadCount(data, dataCount);

                face.data = std::move(data);
            }
            else if (face.type == block_data::Face35::UnknownType50)
            {
                common::IndexWithPositionTexCoordList data;
                ReadCount(data, dataCount);

                face.data = std::move(data);
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
                common::IndexList data;
                ReadCount(data, dataCount);

                face.data = std::move(data);
            }
            else if (face.type == block_data::Face35::UnknownType3)
            {
                common::IndexWithTexCoordList data;
                ReadCount(data, dataCount);

                face.data = std::move(data);
            }
            else if (face.type == block_data::Face35::UnknownType49)
            {
                std::vector<block_data::Face35::Unknown49> data;
                ReadCount(data, dataCount);

                face.data = std::move(data);
            }
            else if (face.type == block_data::Face35::UnknownType51)
            {
                common::IndexWithPositionList data;
                ReadCount(data, dataCount);

                face.data = std::move(data);
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
                common::IndexList data;
                ReadCount(data, dataCount);

                face.data = std::move(data);
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

    NodePtr ReadBlockData35(const block_data::BlockHeader& blockHeader, const NodePtr& parent)
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

        return std::make_shared<NodeSimpleFaces35>(blockHeader, block, parent);
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
            common::PositionWithTexCoordNormalList data;
            ReadCount(data, dataSize);

            block.data = std::move(data);
        }
        else if (block.type == block_data::GroupVertexData37::Vertex3)
        {
            common::PositionWithNormalList data;
            ReadCount(data, dataSize);

            block.data = std::move(data);
        }
        else if (block.type == block_data::GroupVertexData37::UnknownType514)
        {
            std::vector<block_data::GroupVertexData37::Unknown514> data;
            ReadCount(data, dataSize);

            block.data = std::move(data);
        }
        else if (block.type == block_data::GroupVertexData37::UnknownType258)
        {
            std::vector<block_data::GroupVertexData37::Unknown258Or515> data;
            ReadCount(data, dataSize);

            block.data = std::move(data);
        }
        else if (block.type == block_data::GroupVertexData37::UnknownType515)
        {
            std::vector<block_data::GroupVertexData37::Unknown258Or515> data;
            ReadCount(data, dataSize);

            block.data = std::move(data);
        }
        else
        {
            ThrowError("Unknown vertex type", "B3dReaderImpl::DispatchVertexData37");
        }
    }

    NodePtr ReadBlockData37(const block_data::BlockHeader& blockHeader, const NodePtr& parent)
    {
        block_data::GroupVertexData37 block;

        block.boundingSphere = ReadBoundingSphere();
        ReadBytes(block.name.data(), block.name.size());
        block.type = ReadUint32();

        DispatchVertexData37(block);

        NodePtr res = std::make_shared<NodeGroupVertexData37>(blockHeader, block, parent);
        return ReadNestedBlocks(res);
    }

    NodePtr ReadBlockData40(const block_data::BlockHeader& blockHeader, const NodePtr& parent)
    {
        block_data::SimpleGeneratedObjects40 block;

        block.boundingSphere = ReadBoundingSphere();
        ReadBytes(block.empty.data(), block.empty.size());
        ReadBytes(block.name.data(), block.name.size());

        block.type = ReadUint32();
        block.unknown0 = ReadFloat();

        const std::uint32_t unknown1Size = ReadUint32();
        ReadCount(block.unknown1, unknown1Size);

        return std::make_shared<NodeSimpleGeneratedObjects40>(blockHeader, block, parent);
    }
};

B3dTree B3dReader::Read(Ogre::DataStream& input)
{
    B3dReaderImpl reader{input};
    
    return reader.Read();
}

} // namespace b3d
} // namespace data
} // namespace resource
} // namespace d2_hack
