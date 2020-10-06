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



class BlockProcessor
{
public:
    explicit BlockProcessor(B3dListenerInterface& listener)
        : m_listener(listener)
    {
    }

    BlockAction MergeBlockActions(BlockAction parentBlockAction, BlockAction blockAction)
    {
        if (parentBlockAction == BlockAction::Skip)
        {
            return parentBlockAction;
        }

        return blockAction;
    }

    void OnMaterials(Materials&& materials)
    {
        m_listener.OnMaterials(std::move(materials));
    }

    BlockAction OnBlockBegin(const block_data::BlockHeader& blockHeader)
    {
        return m_listener.OnBlockBegin(blockHeader);
    }

    void OnBlockEnd(const block_data::BlockHeader& blockHeader, BlockAction blockAction)
    {
        m_listener.OnBlockEnd(blockHeader, blockAction);
    }

    void OnNestedBlockBegin(std::uint32_t nestedBlockNumber)
    {
        m_listener.OnNestedBlockBegin(nestedBlockNumber);
    }

    void OnNestedBlockEnd(std::uint32_t nestedBlockNumber)
    {
        m_listener.OnNestedBlockEnd(nestedBlockNumber);
    }

    template <typename BlockType>
    void OnBlock(const BlockType& block, BlockAction blockAction)
    {
        switch (blockAction)
        {
        case BlockAction::Process:
            return m_listener.OnBlock(block);

        case BlockAction::Skip:
            // does nothing
            return;

        default:
            assert(0 && "not implemented");
        }
    }

    template <typename DataType>
    void OnData(DataType&& data, BlockAction blockAction)
    {
        switch (blockAction)
        {
        case BlockAction::Process:
            return m_listener.OnData(std::forward<DataType>(data));

        case BlockAction::Skip:
            // does nothing
            return;

        default:
            assert(0 && "not implemented");
        }
    }

private:
    B3dListenerInterface& m_listener;
};



class B3dReaderImpl : private common::Reader, private BlockProcessor
{
public:
    explicit B3dReaderImpl(Ogre::DataStream& input, B3dListenerInterface& listener)
        : common::Reader(input)
        , BlockProcessor(listener)
    {
    }

    void Read()
    {
        FileHeader fileHeader;
        ReadFileHeader(fileHeader);

        ReadMaterials(fileHeader.materials);
        ReadData(fileHeader.data);
    }

private:
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

    void ReadMaterials(const FileHeader::Section& sectionInfo)
    {
        const size_t materialsStartOffset = GetStream().tell();
        if (materialsStartOffset != sectionInfo.offset)
        {
            ThrowError("Incorrect materials offset", "B3dReaderImpl::ReadMaterials");
        }

        Materials materials;
        const std::uint32_t materialsCount = ReadUint32();
        ReadCount(materials, materialsCount);

        const size_t materialsEndOffset = GetStream().tell();
        assert(materialsEndOffset > materialsStartOffset);

        if ((materialsEndOffset - materialsStartOffset) != sectionInfo.size)
        {
            ThrowError("Incorrect materials size", "B3dReaderImpl::ReadMaterials");
        }

        OnMaterials(std::move(materials));
    }

    void ReadData(const FileHeader::Section& sectionInfo)
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

        bool gotEndOfData = false;
        for (; ; )
        {
            ReadBlock(gotEndOfData, BlockAction::Process);
            if (gotEndOfData)
            {
                // reached EOF, exiting
                break;
            }
        }

        const size_t dataEndOffset = GetStream().tell();
        assert(dataEndOffset > dataStartOffset);
        // check postconditions
        if ((dataEndOffset - dataStartOffset) != sectionInfo.size)
        {
            ThrowError("Incorrect data size", "B3dReaderImpl::ReadData");
        }
    }

    void DispatchBlock(std::uint32_t blockType, BlockAction blockAction)
    {
        if (blockType == block_data::EmptyBlock0)
        {
            return ReadBlockData0(blockAction);
        }
        else if (blockType == block_data::GroupRoadInfraObjectsBlock4)
        {
            return ReadBlockData4(blockAction);
        }
        else if (blockType == block_data::GroupObjectsBlock5)
        {
            return ReadBlockData5(blockAction);
        }
        else if (blockType == block_data::GroupVertexBlock7)
        {
            return ReadBlockData7(blockAction);
        }
        else if (blockType == block_data::SimpleFacesBlock8)
        {
            return ReadBlockData8(blockAction);
        }
        else if (blockType == block_data::GroupTriggerBlock9)
        {
            return ReadBlockData9(blockAction);
        }
        else if (blockType == block_data::GroupLodParametersBlock10)
        {
            return ReadBlockData10(blockAction);
        }
        else if (blockType == block_data::GroupUnknownBlock12)
        {
            return ReadBlockData12(blockAction);
        }
        else if (blockType == block_data::SimpleTriggerBlock13)
        {
            return ReadBlockData13(blockAction);
        }
        else if (blockType == block_data::SimpleUnknownBlock14)
        {
            return ReadBlockData14(blockAction);
        }
        else if (blockType == block_data::SimpleObjectConnectorBlock18)
        {
            return ReadBlockData18(blockAction);
        }
        else if (blockType == block_data::GroupObjectsBlock19)
        {
            return ReadBlockData19(blockAction);
        }
        else if (blockType == block_data::SimpleFlatCollisionBlock20)
        {
            return ReadBlockData20(blockAction);
        }
        else if (blockType == block_data::GroupObjectsBlock21)
        {
            return ReadBlockData21(blockAction);
        }
        else if (blockType == block_data::SimpleVolumeCollisionBlock23)
        {
            return ReadBlockData23(blockAction);
        }
        else if (blockType == block_data::GroupTransformMatrixBlock24)
        {
            return ReadBlockData24(blockAction);
        }
        else if (blockType == block_data::SimpleFacesBlock28)
        {
            return ReadBlockData28(blockAction);
        }
        else if (blockType == block_data::GroupUnknownBlock29)
        {
            return ReadBlockData29(blockAction);
        }
        else if (blockType == block_data::SimplePortalBlock30)
        {
            return ReadBlockData30(blockAction);
        }
        else if (blockType == block_data::GroupLightingObjectBlock33)
        {
            return ReadBlockData33(blockAction);
        }
        else if (blockType == block_data::SimpleFaceDataBlock35)
        {
            return ReadBlockData35(blockAction);
        }
        else if (blockType == block_data::GroupIndexAndTexturesBlock37)
        {
            return ReadBlockData37(blockAction);
        }
        else if (blockType == block_data::SimpleGeneratedObjectsBlock40)
        {
            return ReadBlockData40(blockAction);
        }

        ThrowError("Unknown block id", "B3dReaderImpl::ReadBlock");
    }

    void ReadBlock(bool& gotEndOfData, BlockAction parentBlockAction)
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
            return;
        }
        if (blockSeparator != BlockBeginMagic)
        {
            ThrowError("Incorrect block begin magic", "B3dReaderImpl::ReadBlock");
        }

        block_data::BlockHeader blockHeader;
        ReadBytes(blockHeader.name.data(), blockHeader.name.size());
        blockHeader.type = ReadUint32();

        if (blockHeader.type > block_data::MaxBlockId)
        {
            ThrowError("Incorrect block id, possible b3d corruption?", "B3dReaderImpl::ReadBlock");
        }

        BlockAction blockAction = OnBlockBegin(blockHeader);

        BlockAction mergedBlockAction = MergeBlockActions(parentBlockAction, blockAction);

        DispatchBlock(blockHeader.type, mergedBlockAction);

        OnBlockEnd(blockHeader, mergedBlockAction);

        blockSeparator = ReadUint32();
        if ((blockSeparator != BlockNextMagic) && (blockSeparator != BlockEndMagic))
        {
            ThrowError("Incorrect block end magic", "B3dReaderImpl::ReadBlock");
        }
    }

    void ReadNestedBlocks(BlockAction parentBlockAction)
    {
        const std::uint32_t nestedBlocksCount = ReadUint32();
        if (nestedBlocksCount > 1000)
        {
            __debugbreak();
            ThrowError("Unexpectedly big nested size", "B3dReaderImpl::ReadNestedBlocks");
        }

        for (std::uint32_t nestedBlockNumber = 0; nestedBlockNumber != nestedBlocksCount; ++nestedBlockNumber)
        {
            OnNestedBlockBegin(nestedBlockNumber);

            bool gotEndOfData = false;
            ReadBlock(gotEndOfData, parentBlockAction);
            if (gotEndOfData)
            {
                ThrowError("Unexpected end of data", "B3dReaderImpl::ReadNestedBlocks");
            }

            OnNestedBlockEnd(nestedBlockNumber);
        }
    }

    void ReadBlockData0(BlockAction blockAction)
    {
        block_data::Empty0 block;

        ReadBytes(block.emptyData0, sizeof(block.emptyData0));
        block.unknown = ReadFloat();
        ReadBytes(block.emptyData1, sizeof(block.emptyData1));

        OnBlock(block, blockAction);
    }

    void ReadBlockData4(BlockAction blockAction)
    {
        block_data::GroupRoadInfraObjects4 block;

        block.boundingSphere = ReadBoundingSphere();

        ReadBytes(block.name.data(), block.name.size());
        ReadBytes(block.data.data(), block.data.size());

        OnBlock(block, blockAction);

        ReadNestedBlocks(blockAction);
    }

    void ReadBlockData5(BlockAction blockAction)
    {
        block_data::GroupObjects5 block;

        block.boundingSphere = ReadBoundingSphere();
        ReadBytes(block.name.data(), block.name.size());

        OnBlock(block, blockAction);

        ReadNestedBlocks(blockAction);
    }

    void ReadBlockData7(BlockAction blockAction)
    {
        block_data::GroupVertex7 block;

        block.boundingSphere = ReadBoundingSphere();
        ReadBytes(block.name.data(), block.name.size());

        const std::uint32_t vertexAmount = ReadUint32();
        common::PositionWithTexCoordList data;
        ReadCount(data, vertexAmount);

        OnBlock(block, blockAction);
        OnData(std::move(data), blockAction);

        ReadNestedBlocks(blockAction);
    }

    void DispatchReadFaceData8(const std::uint32_t faceType, const std::uint32_t itemsInFace, BlockAction blockAction)
    {
        switch (faceType)
        {
        case block_data::Face8::UnknownType0:
        case block_data::Face8::UnknownType1:
        case block_data::Face8::UnknownType16:
        case block_data::Face8::FaceIndexType128:
        case block_data::Face8::FaceIndexType129:
        case block_data::Face8::UnknownType144:
        {
            common::IndexList data;
            ReadCount(data, itemsInFace);

            OnData(std::move(data), blockAction);
        }
        break;
        
        case block_data::Face8::FaceIndexType2:
        case block_data::Face8::FaceIndexType3:
        {
            common::IndexWithTexCoordList data;
            ReadCount(data, itemsInFace);

            OnData(std::move(data), blockAction);
        }
        break;

        case block_data::Face8::FaceIndexType48:
        case block_data::Face8::FaceIndexType176:
        {
            common::IndexWithPositionList data;
            ReadCount(data, itemsInFace);

            OnData(std::move(data), blockAction);
        }
        break;

        case block_data::Face8::FaceIndexType50:
        case block_data::Face8::FaceIndexType178:
        {
            common::IndexWithPositionTexCoordList data;
            ReadCount(data, itemsInFace);

            OnData(std::move(data), blockAction);
        }
        break;
        
        default:
            ThrowError("Unknown face type", "B3dReaderImpl::DispatchReadFaceData8");
        };
    }

    void ReadBlockData8(BlockAction blockAction)
    {
        {
            block_data::SimpleFaces8 block;
            block.boundingSphere = ReadBoundingSphere();

            OnBlock(block, blockAction);
        }

        const std::uint32_t facesCount = ReadUint32();
        for (std::uint32_t faceNumber = 0; faceNumber != facesCount; ++faceNumber)
        {
            block_data::Face8 face;

            face.type = ReadUint32();
            face.unknown0 = ReadFloat();
            face.unknown1 = ReadUint32();
            face.materialIndex = ReadUint32();

            auto faceType = face.type;
            OnData(std::move(face), blockAction);

            const std::uint32_t itemsInFace = ReadUint32();
            DispatchReadFaceData8(faceType, itemsInFace, blockAction);
        }
    }

    void ReadBlockData9(BlockAction blockAction)
    {
        block_data::GroupTrigger9 block;

        block.boundingSphere = ReadBoundingSphere();
        block.unknown = ReadVector3();
        block.distanceToPlayer = ReadFloat();

        OnBlock(block, blockAction);

        ReadNestedBlocks(blockAction);
    }

    void ReadBlockData10(BlockAction blockAction)
    {
        block_data::GroupLodParameters10 block;

        block.boundingSphere = ReadBoundingSphere();
        block.unknown = ReadVector3();
        block.distanceToPlayer = ReadFloat();

        OnBlock(block, blockAction);

        ReadNestedBlocks(blockAction);
    }

    void ReadBlockData12(BlockAction blockAction)
    {
        block_data::GroupUnknown12 block;

        block.boundingSphere = ReadBoundingSphere();
        block.unknown0 = ReadFloat();
        block.unknown1 = ReadFloat();
        block.unknown2 = ReadFloat();
        block.unknown3 = ReadFloat();
        block.unknown4 = ReadUint32();
        block.unknown5 = ReadUint32();

        OnBlock(block, blockAction);

        ReadNestedBlocks(blockAction);
    }

    void ReadBlockData13(BlockAction blockAction)
    {
        {
            block_data::SimpleTrigger13 block;

            block.boundingSphere = ReadBoundingSphere();
            block.unknown0 = ReadUint32();
            block.unknown1 = ReadUint32();

            OnBlock(block, blockAction);
        }

        const std::uint32_t unknown2Count = ReadUint32();
        common::TriggerInfoList data;
        ReadCount(data.data, unknown2Count);
        
        OnData(std::move(data), blockAction);
    }

    void ReadBlockData14(BlockAction blockAction)
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
        
        OnBlock(block, blockAction);
    }

    void ReadBlockData18(BlockAction blockAction)
    {
        block_data::SimpleObjectConnector18 block;

        block.boundingSphere = ReadBoundingSphere();
        ReadBytes(block.space.data(), block.space.size());
        ReadBytes(block.object.data(), block.object.size());

        OnBlock(block, blockAction);
    }

    void ReadBlockData19(BlockAction blockAction)
    {
        {
            block_data::GroupObjects19 block;
            OnBlock(block, blockAction);
        }

        ReadNestedBlocks(blockAction);
    }

    void ReadBlockData20(BlockAction blockAction)
    {
        block_data::SimpleFlatCollision20 block;

        block.boundingSphere = ReadBoundingSphere();
        const std::uint32_t countA = ReadUint32();

        block.unknown0 = ReadUint32();
        block.unknown1 = ReadUint32();

        const std::uint32_t countB = ReadUint32();

        common::CollisionPositionList a;
        ReadCount(a.data, countA);

        common::CollisionUnknownList b;
        ReadCount(b.data, countB);

        OnBlock(block, blockAction);
        OnData(std::move(a), blockAction);
        OnData(std::move(b), blockAction);
    }

    void ReadBlockData21(BlockAction blockAction)
    {
        block_data::GroupObjects21 block;

        block.boundingSphere = ReadBoundingSphere();
        block.count = ReadUint32();
        block.unknown = ReadUint32();

        OnBlock(block, blockAction);

        ReadNestedBlocks(blockAction);
    }

    void DispatchCollisionEntry23(const std::uint32_t type, BlockAction blockAction)
    {
        if (type == block_data::SimpleVolumeCollision23::UnknownType0)
        {
            const std::uint32_t count = ReadUint32();
            common::CollisionPositionList data;
            ReadCount(data.data, count);

            OnData(std::move(data), blockAction);
        }
        else
        {
            ThrowError("Unknown type", "B3dReaderImpl::DispatchCollisionEntry23");
        }
    }

    void ReadBlockData23(BlockAction blockAction)
    {
        block_data::SimpleVolumeCollision23 block;

        block.unknown0 = ReadUint32();
        block.unknown1 = ReadUint32();
        block.type = ReadUint32();

        if (block.type == block_data::SimpleVolumeCollision23::UnknownType0)
        {
            // does nothing
        }
        else
        {
            ThrowError("Unknown type", "B3dReaderImpl::ReadBlockData23");
        }

        OnBlock(block, blockAction);

        const std::uint32_t collisionCount = ReadUint32();
        for (std::uint32_t collisionNumber = 0; collisionNumber != collisionCount; ++collisionNumber)
        {
            DispatchCollisionEntry23(block.type, blockAction);
        }
    }

    void ReadBlockData24(BlockAction blockAction)
    {
        block_data::GroupTransformMatrix24 block;

        block.x = ReadVector3();
        block.y = ReadVector3();
        block.z = ReadVector3();

        block.position = ReadVector3();

        block.unknown = ReadUint32();

        OnBlock(block, blockAction);

        ReadNestedBlocks(blockAction);
    }

    void DispatchFaceEntry28(const std::uint32_t type, BlockAction blockAction)
    {
        if (type == block_data::Face28Entry::Unknown2)
        {
            const std::uint32_t count = ReadUint32();
            std::vector<block_data::Face28Entry::Unknown> data;
            ReadCount(data, count);

            OnData(std::move(data), blockAction);
        }
        else
        {
            ThrowError("Unknown type", "B3dReaderImpl::DispatchFaceEntry28");
        }
    }

    void ReadBlockData28(BlockAction blockAction)
    {
        {
            block_data::SimpleFaces28 block;

            block.boundingSphere = ReadBoundingSphere();
            block.unknown = ReadVector3();

            OnBlock(block, blockAction);
        }

        const std::uint32_t facesCount = ReadUint32();
        for (std::uint32_t faceNumber = 0; faceNumber != facesCount; ++faceNumber)
        {
            block_data::Face28Entry data;
            data.type = ReadUint32();
            data.unknown0 = ReadFloat();
            data.unknown1 = ReadUint32();
            data.materialIndex = ReadUint32();

            auto dataType = data.type;
            OnData(std::move(data), blockAction);

            DispatchFaceEntry28(dataType, blockAction);
        }
    }

    void ReadBlockData29(BlockAction blockAction)
    {
        block_data::GroupUnknown29 block;

        block.boundingSphere = ReadBoundingSphere();
        block.type = ReadUint32();
        block.unknown0 = ReadUint32();

        ReadBytes(block.unknown1.data(), (block.type == 3 ? 7 : 8) * sizeof(block.unknown1[0]));

        OnBlock(block, blockAction);
        ReadNestedBlocks(blockAction);
    }

    void ReadBlockData30(BlockAction blockAction)
    {
        block_data::SimplePortal30 block;

        block.boundingSphere = ReadBoundingSphere();
        ReadBytes(block.connectedRoom.data(), block.connectedRoom.size());

        block.leftDown = ReadVector3();
        block.upRight = ReadVector3();

        OnBlock(block, blockAction);
    }

    void ReadBlockData33(BlockAction blockAction)
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

        OnBlock(block, blockAction);

        ReadNestedBlocks(blockAction);
    }

    void DispatchReadMeshData35(const std::uint32_t blockType, const std::uint32_t meshType, BlockAction blockAction)
    {
        const std::uint32_t dataCount = ReadUint32();

        if (blockType == block_data::SimpleFaceData35::Unknown1)
        {
            if (meshType == block_data::Mesh35::Indices0)
            {
                common::IndexList data;
                ReadCount(data, dataCount);

                OnData(std::move(data), blockAction);
            }
            else if (meshType == block_data::Mesh35::Unknown2)
            {
                common::IndexWithTexCoordList data;
                ReadCount(data, dataCount);

                OnData(std::move(data), blockAction);
            }
            else if (meshType == block_data::Mesh35::UnknownType48)
            {
                common::IndexWithPositionList data;
                ReadCount(data, dataCount);

                OnData(std::move(data), blockAction);
            }
            else if (meshType == block_data::Mesh35::UnknownType50)
            {
                common::IndexWithPositionTexCoordList data;
                ReadCount(data, dataCount);

                OnData(std::move(data), blockAction);
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
                common::IndexList data;
                ReadCount(data, dataCount);

                OnData(std::move(data), blockAction);
            }
            else if (meshType == block_data::Mesh35::UnknownType3)
            {
                common::IndexWithTexCoordList data;
                ReadCount(data, dataCount);

                OnData(std::move(data), blockAction);
            }
            else if (meshType == block_data::Mesh35::UnknownType49)
            {
                std::vector<block_data::Mesh35::Unknown49> data;
                ReadCount(data, dataCount);

                OnData(std::move(data), blockAction);
            }
            else if (meshType == block_data::Mesh35::UnknownType51)
            {
                common::IndexWithPositionList data;
                ReadCount(data, dataCount);

                OnData(std::move(data), blockAction);
            }
            else
            {
                ThrowError("Unknown mesh type for block type 2", "B3dReaderImpl::DispatchReadMesh35");
            }
        }
        else if (blockType == block_data::SimpleFaceData35::IndicesOnly3)
        {
            switch (meshType)
            {
            case block_data::Mesh35::Indices0:
            case block_data::Mesh35::Indices1:
            case block_data::Mesh35::Indices3:
            case block_data::Mesh35::Indices16:
            case block_data::Mesh35::Indices17:
            {
                common::IndexList data;
                ReadCount(data, dataCount);

                OnData(std::move(data), blockAction);
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

    void ReadBlockData35(BlockAction blockAction)
    {
        block_data::SimpleFaceData35 block;

        block.boundingSphere = ReadBoundingSphere();
        block.type = ReadUint32();
        block.materialIndex = ReadUint32();

        OnBlock(block, blockAction);

        const std::uint32_t meshCount = ReadUint32();
        for (std::uint32_t meshNumber = 0; meshNumber != meshCount; ++meshNumber)
        {
            block_data::Mesh35 data;
            data.type = ReadUint32();
            data.unknown0 = ReadFloat();
            data.unknown1 = ReadUint32();
            data.materialIndex = ReadUint32();

            auto dataType = data.type;
            OnData(std::move(data), blockAction);

            DispatchReadMeshData35(block.type, dataType, blockAction);
        }
    }

    void DispatchVertexData37(const std::uint32_t type, BlockAction blockAction)
    {
        const std::uint32_t dataSize = ReadUint32();

        if (dataSize == 0)
        {
            if (type != 0)
            {
                ThrowError("Non-Null Data cannot be empty", "B3dReaderImpl::DispatchVertexData37");
            }
            return;
        }

        if (type == block_data::GroupVertexData37::Vertex2)
        {
            common::PositionWithTexCoordNormalList data;
            ReadCount(data, dataSize);

            OnData(std::move(data), blockAction);
        }
        else if (type == block_data::GroupVertexData37::Vertex3)
        {
            common::PositionWithNormalList data;
            ReadCount(data, dataSize);

            OnData(std::move(data), blockAction);
        }
        else if (type == block_data::GroupVertexData37::UnknownType514)
        {
            std::vector<block_data::GroupVertexData37::Unknown514> data;
            ReadCount(data, dataSize);

            OnData(std::move(data), blockAction);
        }
        else if (type == block_data::GroupVertexData37::UnknownType258)
        {
            std::vector<block_data::GroupVertexData37::Unknown258> data;
            ReadCount(data, dataSize);

            OnData(std::move(data), blockAction);
        }
        else
        {
            ThrowError("Unknown vertex type", "B3dReaderImpl::DispatchVertexData37");
        }
    }

    void ReadBlockData37(BlockAction blockAction)
    {
        block_data::GroupVertexData37 block;

        block.boundingSphere = ReadBoundingSphere();
        ReadBytes(block.name.data(), block.name.size());
        block.type = ReadUint32();

        OnBlock(block, blockAction);

        DispatchVertexData37(block.type, blockAction);

        ReadNestedBlocks(blockAction);
    }

    void ReadBlockData40(BlockAction blockAction)
    {
        block_data::SimpleGeneratedObjects40 block;

        block.boundingSphere = ReadBoundingSphere();
        ReadBytes(block.empty.data(), block.empty.size());
        ReadBytes(block.name.data(), block.name.size());

        block.type = ReadUint32();
        block.unknown0 = ReadFloat();

        const std::uint32_t dataSize = ReadUint32();
        common::GeneratedObjectInfo data;
        ReadCount(data.data, dataSize);

        OnBlock(block, blockAction);
        OnData(std::move(data), blockAction);
    }
};

void B3dReader::Read(Ogre::DataStream& input, B3dListenerInterface& listener)
{
    B3dReaderImpl reader{input, listener };
    
    reader.Read();
}

} // namespace b3d
} // namespace data
} // namespace resource
} // namespace d2_hack
