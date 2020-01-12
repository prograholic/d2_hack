// Implementation based on https://github.com/AlexKimov/HardTruck-RignRoll-file-formats

#include <d2_hack/codec/data/b3d_reader.h>

#include <cstring>

#include <d2_hack/common/reader.h>

namespace d2_hack
{
namespace codec
{
namespace data
{
namespace b3d
{

static const std::uint8_t Signature[FileHeader::SignatureSize] = { 'b', '3', 'd', '\0' };

static const std::uint32_t DataBeginMagic = 111;
static const std::uint32_t DataEndMagic = 222;

static const std::uint32_t BlockBeginMagic = 333;
static const std::uint32_t BlockNextMagic = 444;
static const std::uint32_t BlockEndMagic = 555;

static const std::uint32_t EmptyBlock0 = 0;
static const std::uint32_t GroupRoadInfraObjectsBlock4 = 4;
static const std::uint32_t GroupObjectsBlock5 = 5;
static const std::uint32_t GroupVertexBlock7 = 7;
static const std::uint32_t SimpleFacesBlock8 = 8;
static const std::uint32_t GroupTriggerBlock9 = 9;
static const std::uint32_t GroupLodParametersBlock10 = 10;
static const std::uint32_t GroupUnknownBlock12 = 12;
static const std::uint32_t SimpleTriggerBlock13 = 13;
static const std::uint32_t SimpleObjectConnectorBlock18 = 18;
static const std::uint32_t GroupObjectsBlock19 = 19;
static const std::uint32_t SimpleFlatCollisionBlock20 = 20;
static const std::uint32_t GroupObjectsBlock21 = 21;
static const std::uint32_t SimpleVolumeCollisionBlock23 = 23;
static const std::uint32_t GroupTransformMatrixBlock24 = 24;
static const std::uint32_t SimpleFacesBlock28 = 28;
static const std::uint32_t SimplePortalBlock30 = 30;
static const std::uint32_t GroupLightingObjectBlock33 = 33;
static const std::uint32_t SimpleFaceDataBlock35 = 35;
static const std::uint32_t GroupIndexAndTexturesBlock37 = 37;
static const std::uint32_t SimpleGeneratedObjectsBlock40 = 40;

static const std::uint32_t MaxBlockId = 40;


#define B3D_VISIT() if (m_visitor) m_visitor

class B3dReaderImpl : public common::Reader
{
public:
    explicit B3dReaderImpl(std::istream& input, B3dVisitorInterface* visitor = nullptr)
        : common::Reader(input)
        , m_visitor(visitor)
    {
    }

    void Read(B3d& data)
    {
        ReadFileHeader(data.header);

        ReadMaterials(data.header.materials, data.materials);
        ReadData(data.header.data, data.data);
    }

private:
    B3dVisitorInterface* m_visitor;

    void ReadFileHeader(FileHeader& fileHeader)
    {
        ReadUntil(fileHeader.signature, FileHeader::SignatureSize);
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

        B3D_VISIT()->VisitMaterials(materials);
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
        else if (block.header.type == GroupRoadInfraObjectsBlock4)
        {
            return ReadBlockData4(block);
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
        else if (block.header.type == GroupUnknownBlock12)
        {
            return ReadBlockData12(block);
        }
        else if (block.header.type == SimpleTriggerBlock13)
        {
            return ReadBlockData13(block);
        }
        else if (block.header.type == SimpleObjectConnectorBlock18)
        {
            return ReadBlockData18(block);
        }
        else if (block.header.type == GroupObjectsBlock19)
        {
            return ReadBlockData19(block);
        }
        else if (block.header.type == SimpleFlatCollisionBlock20)
        {
            return ReadBlockData20(block);
        }
        else if (block.header.type == GroupObjectsBlock21)
        {
            return ReadBlockData21(block);
        }
        else if (block.header.type == SimpleVolumeCollisionBlock23)
        {
            return ReadBlockData23(block);
        }
        else if (block.header.type == GroupTransformMatrixBlock24)
        {
            return ReadBlockData24(block);
        }
        else if (block.header.type == SimpleFacesBlock28)
        {
            return ReadBlockData28(block);
        }
        else if (block.header.type == SimplePortalBlock30)
        {
            return ReadBlockData30(block);
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
        else if (block.header.type == SimpleGeneratedObjectsBlock40)
        {
            return ReadBlockData40(block);
        }

        ThrowError("Unknown block id", "B3dReaderImpl::ReadBlock");
    }

    void __DebugB3d()
    {
        std::ostream* out = nullptr;
        if (m_visitor)
        {
            out = m_visitor->GetDebugOutStream();
        }

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
                hexed += std::to_string(common::details::ToNumericUnsafe<float>(p));
                hexed += " ";
                hexed += std::to_string(common::details::ToNumericUnsafe<std::uint32_t>(p));
                hexed.push_back('\n');
            }
        }
        
        if (out)
        {
            *out << "unknown: [\n" << hexed << "], cnt: " << cnt << ", size: " << data.size() << std::endl;
        }
        if (eof)
        {
            if (out)
            {
                *out << "\nunknown: EOF: " << GetOffset() << std::endl;
            }
        }
    }

    void ReadBlock(Block& block, bool& gotEndOfData)
    {
        std::uint32_t blockSeparator = ReadUint32();

        B3D_VISIT()->VisitBlockSeparator(blockSeparator);
        while ((blockSeparator == BlockNextMagic) || (blockSeparator == BlockEndMagic))
        {
            // reread new block separator
            blockSeparator = ReadUint32();
            B3D_VISIT()->VisitBlockSeparator(blockSeparator);
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

        ReadUntil(block.header.name.begin(), block.header.name.size());
        block.header.type = ReadUint32();

        B3D_VISIT()->VisitBlockHeader(block.header.name, block.header.type);

        if (block.header.type > MaxBlockId)
        {
            ThrowError("Incorrect block id, possible b3d corruption?", "B3dReaderImpl::ReadBlock");
        }

        DispatchBlock(block);

        B3D_VISIT()->VisitBlock(block.header.name, block.header.type);

        blockSeparator = ReadUint32();
        B3D_VISIT()->VisitBlockSeparator(blockSeparator);
        if ((blockSeparator != BlockNextMagic) && (blockSeparator != BlockEndMagic))
        {
            ThrowError("Incorrect block end magic", "B3dReaderImpl::ReadBlock");
        }
    }

    void ReadNestedBlocks(NestedBlockList& nestedBlocks)
    {
        const std::uint32_t nestedBlocksCount = ReadUint32();
        B3D_VISIT()->VisitNestedBlockHeader(nestedBlocksCount);

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

        B3D_VISIT()->VisitBlockData(blockData);

        block.data = std::move(blockData);
    }

    void ReadBlockData4(Block& /* block */)
    {
        block_data::GroupRoadInfraObjects4 blockData;

        blockData.boundingSphere = ReadBoundingSphere();
        B3D_VISIT()->VisitBoundingSphere(blockData.boundingSphere);

        ReadUntil(blockData.name.begin(), blockData.name.size());
        ReadUntil(blockData.data.begin(), blockData.data.size());

        B3D_VISIT()->VisitBlockData(blockData);

        ReadNestedBlocks(blockData.nestedBlocks);

        //block.data = std::move(blockData);
    }

    void ReadBlockData5(Block& block)
    {
        block_data::GroupObjects5 blockData;

        blockData.boundingSphere = ReadBoundingSphere();
        B3D_VISIT()->VisitBoundingSphere(blockData.boundingSphere);
        
        ReadUntil(blockData.name.begin(), blockData.name.size());

        B3D_VISIT()->VisitBlockData(blockData);

        ReadNestedBlocks(blockData.nestedBlocks);

        block.data = std::move(blockData);
    }

    void ReadBlockData7(Block& block)
    {
        block_data::GroupVertex7 blockData;

        blockData.boundingSphere = ReadBoundingSphere();
        B3D_VISIT()->VisitBoundingSphere(blockData.boundingSphere);
        
        ReadUntil(blockData.name.begin(), blockData.name.size());

        const std::uint32_t vertexAmount = ReadUint32();
        blockData.vertices.resize(vertexAmount);
        for (auto& vertexEntry : blockData.vertices)
        {
            vertexEntry = ReadPositionWithTexCoord();

            B3D_VISIT()->VisitPosition(vertexEntry.position);
        }

        B3D_VISIT()->VisitBlockData(blockData);

        ReadNestedBlocks(blockData.nestedBlocks);

        block.data = std::move(blockData);
    }

    void DispatchReadFaceData8(const std::uint32_t faceType, block_data::Face8::FaceData& faceData)
    {
        switch (faceType)
        {
        case block_data::Face8::UnknownType0:
        case block_data::Face8::UnknownType1:
        case block_data::Face8::UnknownType16:
        case block_data::Face8::FaceIndexType129:
        case block_data::Face8::UnknownType144:
        {
            common::Index index = ReadIndex();

            faceData = std::move(index);
        }
        break;
        
        case block_data::Face8::FaceIndexType3:
        {
            common::IndexWithTexCoord faceDataValue = ReadIndexWithTexCoord();

            faceData = std::move(faceDataValue);
        }
        break;

        case block_data::Face8::FaceIndexType48:
        case block_data::Face8::FaceIndexType176:
        {
            common::IndexWithPosition faceDataValue = ReadIndexWithPosition();

            //B3D_VISIT()->VisitVector3(faceDataValue.position);

            faceData = std::move(faceDataValue);
        }
        break;

        case block_data::Face8::FaceIndexType50:
        case block_data::Face8::FaceIndexType178:
        {
            common::IndexWithPositionTexCoord faceDataValue = ReadIndexWithPositionTexCoord();

            //B3D_VISIT()->VisitVector3(faceDataValue.position);

            faceData = std::move(faceDataValue);
        }
        break;
        
        default:
            ThrowError("Unknown face type", "B3dReaderImpl::DispatchReadFaceData8");
        };
    }

    void ReadBlockData8(Block& block)
    {
        block_data::SimpleFaces8 blockData;

        blockData.boundingSphere = ReadBoundingSphere();
        B3D_VISIT()->VisitBoundingSphere(blockData.boundingSphere);

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

        B3D_VISIT()->VisitBlockData(blockData);

        block.data = std::move(blockData);
    }

    void ReadBlockData9(Block& block)
    {
        block_data::GroupTrigger9 blockData;

        blockData.boundingSphere = ReadBoundingSphere();
        B3D_VISIT()->VisitBoundingSphere(blockData.boundingSphere);

        blockData.unknown = ReadVector3();
        blockData.distanceToPlayer = ReadFloat();

        B3D_VISIT()->VisitBlockData(blockData);

        ReadNestedBlocks(blockData.nestedBlocks);

        block.data = std::move(blockData);
    }

    void ReadBlockData10(Block& block)
    {
        block_data::GroupLodParameters10 blockData;

        blockData.boundingSphere = ReadBoundingSphere();
        B3D_VISIT()->VisitBoundingSphere(blockData.boundingSphere);

        blockData.unknown = ReadVector3();
        blockData.distanceToPlayer = ReadFloat();

        B3D_VISIT()->VisitBlockData(blockData);

        ReadNestedBlocks(blockData.nestedBlocks);

        block.data = std::move(blockData);
    }

    void ReadBlockData12(Block& block)
    {
        block_data::GroupUnknown12 blockData;

        blockData.boundingSphere = ReadBoundingSphere();
        B3D_VISIT()->VisitBoundingSphere(blockData.boundingSphere);

        blockData.unknown0 = ReadFloat();
        blockData.unknown1 = ReadFloat();
        blockData.unknown2 = ReadFloat();
        blockData.unknown3 = ReadFloat();

        blockData.unknown4 = ReadUint32();
        blockData.unknown5 = ReadUint32();

        B3D_VISIT()->VisitBlockData(blockData);

        ReadNestedBlocks(blockData.nestedBlocks);

        block.data = std::move(blockData);
    }

    void ReadBlockData13(Block& block)
    {
        block_data::SimpleTrigger13 blockData;

        blockData.boundingSphere = ReadBoundingSphere();
        B3D_VISIT()->VisitBoundingSphere(blockData.boundingSphere);

        blockData.unknown0 = ReadUint32();
        blockData.unknown1 = ReadUint32();
        
        const std::uint32_t unknown2Count = ReadUint32();
        blockData.unknown2.resize(unknown2Count);
        for (auto& u : blockData.unknown2)
        {
            u = ReadFloat();
        }

        B3D_VISIT()->VisitBlockData(blockData);

        block.data = std::move(blockData);
    }

    void ReadBlockData18(Block& block)
    {
        block_data::SimpleObjectConnector18 blockData;

        blockData.boundingSphere = ReadBoundingSphere();
        B3D_VISIT()->VisitBoundingSphere(blockData.boundingSphere);

        ReadUntil(blockData.space.begin(), blockData.space.size());
        ReadUntil(blockData.object.begin(), blockData.object.size());

        B3D_VISIT()->VisitBlockData(blockData);

        block.data = std::move(blockData);
    }

    void ReadBlockData19(Block& block)
    {
        block_data::GroupObjects19 blockData;

        B3D_VISIT()->VisitBlockData(blockData);

        ReadNestedBlocks(blockData.nestedBlocks);

        block.data = std::move(blockData);
    }

    void ReadBlockData20(Block& block)
    {
        block_data::SimpleFlatCollision20 blockData;

        blockData.boundingSphere = ReadBoundingSphere();
        B3D_VISIT()->VisitBoundingSphere(blockData.boundingSphere);

        const std::uint32_t countA = ReadUint32();

        blockData.unknown0 = ReadUint32();
        blockData.unknown1 = ReadUint32();

        const std::uint32_t countB = ReadUint32();

        blockData.a.resize(countA);
        for (auto& a : blockData.a)
        {
            a = ReadVector3();
            //B3D_VISIT()->VisitVector3(a);
        }

        blockData.b.resize(countB);
        for (auto& b : blockData.b)
        {
            b = ReadFloat();
        }

        B3D_VISIT()->VisitBlockData(blockData);

        block.data = std::move(blockData);
    }

    void ReadBlockData21(Block& block)
    {
        block_data::GroupObjects21 blockData;

        blockData.boundingSphere = ReadBoundingSphere();
        B3D_VISIT()->VisitBoundingSphere(blockData.boundingSphere);
        
        blockData.count = ReadUint32();
        blockData.unknown = ReadUint32();

        B3D_VISIT()->VisitBlockData(blockData);

        ReadNestedBlocks(blockData.nestedBlocks);

        block.data = std::move(blockData);
    }

    void DispatchCollisionEntry23(const std::uint32_t type, block_data::SimpleVolumeCollision23::CollisionEntry& collisionEntry)
    {
        if (type == block_data::SimpleVolumeCollision23::UnknownType0)
        {
            block_data::SimpleVolumeCollision23::Unknown0 collisionEntryData;

            const std::uint32_t count = ReadUint32();
            collisionEntryData.unknown.resize(count);
            for (auto& pos : collisionEntryData.unknown)
            {
                pos = ReadVector3();
                //B3D_VISIT()->VisitVector3(pos);
            }
            
            collisionEntry = std::move(collisionEntryData);
        }
        else
        {
            ThrowError("Unknown type", "B3dReaderImpl::DispatchCollisionEntry23");
        }
    }

    void ReadBlockData23(Block& block)
    {
        block_data::SimpleVolumeCollision23 blockData;

        blockData.unknown0 = ReadUint32();
        blockData.unknown1 = ReadUint32();
        blockData.type = ReadUint32();

        if (blockData.type == block_data::SimpleVolumeCollision23::UnknownType0)
        {
            // does nothing
        }
        else
        {
            ThrowError("Unknown type", "B3dReaderImpl::ReadBlockData23");
        }

        const std::uint32_t collisionCount = ReadUint32();
        blockData.collisionEntries.resize(collisionCount);
        for (auto& collisionEntry : blockData.collisionEntries)
        {
            DispatchCollisionEntry23(blockData.type, collisionEntry);
        }

        B3D_VISIT()->VisitBlockData(blockData);

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

        B3D_VISIT()->VisitBlockData(blockData);

        ReadNestedBlocks(blockData.nestedBlocks);

        block.data = std::move(blockData);
    }

    void DispatchFaceEntry28(const std::uint32_t type, block_data::Face28Entry::FaceData &faceData)
    {
        if (type == block_data::Face28Entry::Unknown2)
        {
            block_data::Face28Entry::Unknown faceDataValue;

            faceDataValue.unknown0 = ReadFloat();
            faceDataValue.unknown1 = ReadFloat();
            faceDataValue.unknown2 = ReadFloat();
            faceDataValue.unknown3 = ReadFloat();

            faceData = std::move(faceDataValue);
        }
        else
        {
            ThrowError("Unknown type", "B3dReaderImpl::DispatchFaceEntry28");
        }
    }

    void ReadBlockData28(Block& block)
    {
        block_data::SimpleFaces28 blockData;

        blockData.boundingSphere = ReadBoundingSphere();
        B3D_VISIT()->VisitBoundingSphere(blockData.boundingSphere);

        blockData.unknown = ReadVector3();

        const std::uint32_t facesCount = ReadUint32();
        blockData.facesEntries.resize(facesCount);
        for (auto& faceEntry : blockData.facesEntries)
        {
            faceEntry.type = ReadUint32();
            faceEntry.unknown0 = ReadFloat();
            faceEntry.unknown1 = ReadUint32();
            faceEntry.unknown2 = ReadUint32();

            const std::uint32_t count = ReadUint32();
            faceEntry.faceData.resize(count);
            for (auto& d : faceEntry.faceData)
            {
                DispatchFaceEntry28(faceEntry.type, d);
            }
        }

        B3D_VISIT()->VisitBlockData(blockData);

        block.data = std::move(blockData);
    }

    void ReadBlockData30(Block& block)
    {
        block_data::SimplePortal30 blockData;

        blockData.boundingSphere = ReadBoundingSphere();
        B3D_VISIT()->VisitBoundingSphere(blockData.boundingSphere);

        ReadUntil(blockData.connectedRoom.begin(), blockData.connectedRoom.size());

        blockData.leftDown = ReadVector3();
        blockData.upRight = ReadVector3();

        B3D_VISIT()->VisitBlockData(blockData);

        block.data = std::move(blockData);
    }

    void ReadBlockData33(Block& block)
    {
        block_data::GroupLightingObjects33 blockData;

        blockData.boundingSphere = ReadBoundingSphere();
        B3D_VISIT()->VisitBoundingSphere(blockData.boundingSphere);

        blockData.unknown0 = ReadUint32();
        blockData.unknown1 = ReadUint32();
        blockData.unknown2 = ReadUint32();

        blockData.position = ReadVector3();
        for (auto& colorEntry : blockData.color)
        {
            colorEntry = ReadFloat();
        }

        B3D_VISIT()->VisitBlockData(blockData);

        ReadNestedBlocks(blockData.nestedBlocks);

        block.data = std::move(blockData);
    }

    void DispatchReadMeshData35(const std::uint32_t blockType, const std::uint32_t meshType, block_data::Mesh35::MeshData& meshData)
    {
        if (blockType == block_data::SimpleFaceData35::Unknown1)
        {
            if (meshType == block_data::Mesh35::UnknownType48)
            {
                common::IndexWithPosition meshDataValue = ReadIndexWithPosition();
                
                //B3D_VISIT()->VisitVector3(meshDataValue.position);

                meshData = meshDataValue;
            }
            else if (meshType == block_data::Mesh35::UnknownType50)
            {
                common::IndexWithPositionTexCoord meshDataValue = ReadIndexWithPositionTexCoord();

                //B3D_VISIT()->VisitVector3(meshDataValue.position);

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
                common::Index meshDataValue = ReadIndex();

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
            switch (meshType)
            {
            case block_data::Mesh35::Indices0:
            case block_data::Mesh35::Indices1:
            case block_data::Mesh35::Indices3:
            case block_data::Mesh35::Indices16:
            {
                common::Index meshDataValue = ReadIndex();

                meshData = meshDataValue;
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

    void ReadBlockData35(Block& block)
    {
        block_data::SimpleFaceData35 blockData;

        blockData.boundingSphere = ReadBoundingSphere();
        B3D_VISIT()->VisitBoundingSphere(blockData.boundingSphere);
        
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

        B3D_VISIT()->VisitBlockData(blockData);

        block.data = std::move(blockData);
    }

    void DispatchVertexData37(const std::uint32_t type, block_data::GroupVertexData37::VertexData& vertexData)
    {
        if (type == block_data::GroupVertexData37::Vertex2)
        {
            common::PositionWithTexCoordNormal vertexDataValue = ReadPositionWithTexCoordNormal();

            //B3D_VISIT()->VisitVector3(vertexDataValue.position);

            vertexData = std::move(vertexDataValue);
        }
        else if (type == block_data::GroupVertexData37::Vertex3)
        {
            common::PositionWithNormal positionWithNormal = ReadPositionWithNormal();

            vertexData = std::move(positionWithNormal);
        }
        else if (type == block_data::GroupVertexData37::UnknownType514)
        {
            block_data::GroupVertexData37::Unknown514 vertexDataValue;

            vertexDataValue.unknown0 = ReadVector3();
            vertexDataValue.unknown1 = ReadVector3();
            vertexDataValue.unknown2 = ReadVector3();
            vertexDataValue.unknown3 = ReadVector3();

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

        blockData.boundingSphere = ReadBoundingSphere();
        B3D_VISIT()->VisitBoundingSphere(blockData.boundingSphere);
        
        ReadUntil(blockData.name.begin(), blockData.name.size());
        blockData.type = ReadUint32();

        const std::uint32_t dataSize = ReadUint32();
        blockData.vertexDataList.resize(dataSize);
        for (auto& vertexData : blockData.vertexDataList)
        {
            DispatchVertexData37(blockData.type, vertexData);
        }

        B3D_VISIT()->VisitBlockData(blockData);

        ReadNestedBlocks(blockData.nestedBlocks);

        block.data = std::move(blockData);
    }

    void ReadBlockData40(Block& block)
    {
        block_data::SimpleGeneratedObjects40 blockData;

        blockData.boundingSphere = ReadBoundingSphere();
        B3D_VISIT()->VisitBoundingSphere(blockData.boundingSphere);
        
        ReadUntil(blockData.empty.begin(), blockData.empty.size());
        ReadUntil(blockData.name.begin(), blockData.name.size());

        blockData.type = ReadUint32();
        blockData.unknown0 = ReadFloat();

        const std::uint32_t dataSize = ReadUint32();
        blockData.unknown1.resize(dataSize);
        for (auto& item : blockData.unknown1)
        {
            item = ReadFloat();
        }

        B3D_VISIT()->VisitBlockData(blockData);

        block.data = std::move(blockData);
    }
};

void B3dReader::Read(std::istream &input, B3d& data, B3dVisitorInterface* visitor)
{
    B3dReaderImpl reader{input, visitor};
    
    reader.Read(data);
}

} // namespace b3d
} // namespace data
} // namespace codec
} // namespace d2_hack
