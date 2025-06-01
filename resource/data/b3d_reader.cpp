// Implementation based on https://github.com/AlexKimov/HardTruck-RignRoll-file-formats

#include <d2_hack/resource/data/b3d_reader.h>

#include <cstring>
#include <stack>

#include <d2_hack/common/reader.h>
#include <d2_hack/common/utils.h>

#include <d2_hack/resource/data/b3d_visitor.h>
#include <d2_hack/resource/data/b3d_utils.h>

namespace d2_hack
{
namespace resource
{
namespace data
{
namespace b3d
{

using namespace common;

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

static const std::uint32_t DataBeginMagic = 111;           // 0x006F, 6F 00 00 00
static const std::uint32_t DataEndMagic = 222;             // 0x00DE, DE 00 00 00
static const std::uint32_t BlockBeginMagic = 333;          // 0x014D, 4D 01 00 00
static const std::uint32_t BlockHierarchyBreaker = 444;    // 0x01BC, BC 01 00 00
static const std::uint32_t BlockEndMagic = 555;            // 0x022B, 2B 02 00 00


class B3dReaderImpl : private common::Reader
{
public:
    explicit B3dReaderImpl(Ogre::DataStream& input)
        : common::Reader(input)
    {
    }

    B3dTreePtr Read(const std::string& dir, const std::string& id)
    {
        FileHeader fileHeader;
        ReadFileHeader(fileHeader);

        m_originalRoot = std::make_shared<B3dTree>();

        m_originalRoot->dir = dir;
        m_originalRoot->id = id;
        m_originalRoot->materials = ReadMaterials(fileHeader.materials);
        ReadData(fileHeader.data, *m_originalRoot);

        return m_originalRoot;
    }

private:
    B3dTreePtr m_originalRoot;

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

    block_data::SomeUintsWithBuffer ReadSomeUintsWithBuffer(bool readCount = true)
    {
        block_data::SomeUintsWithBuffer res;

        res.type = ReadUint32();
        res.unknown0 = ReadUint32();

        if (readCount)
        {
            const std::uint32_t unknown1Count = ReadUint32();
            ReadCount(res.unknown1, unknown1Count);
        }

        return res;
    }

    block_data::BlockHeader ReadBlockHeader()
    {
        common::ResourceName name;
        ReadBytes(name.data(), name.size());
        std::uint32_t type = ReadUint32();

        return MakeBlockHeader(name, type);
    }

    void ReadData(const FileHeader::Section& sectionInfo, B3dTree& tree)
    {
        const size_t dataStartOffset = GetStream().tell();
        if (dataStartOffset != sectionInfo.offset)
        {
            ThrowError("Incorrect data offset", "B3dReaderImpl::ReadData");
        }

        std::stack<B3dNodePtr> roots;

        for ( ; ; )
        {
            std::uint32_t separator = ReadUint32();
            if (separator == DataEndMagic)
            {
                break;
            }
            else if (separator == BlockBeginMagic)
            {
                block_data::BlockHeader blockHeader = ReadBlockHeader();
                bool noRoots = roots.empty();
                B3dNodePtr current = DispatchBlock(noRoots ? NodePtr{} : roots.top(), blockHeader);
                roots.push(current);
                if (current->HasNestedCount())
                {
                    // NOTE: do we need this field (nested count)
                    std::uint32_t nestedCount = ReadUint32();
                    (void)nestedCount;
                }
            }
            else if (separator == BlockHierarchyBreaker)
            {
                bool noRoots = roots.empty();
                B3dNodePtr current = MakeHierarhyBreaker(noRoots ? NodePtr{} : roots.top());
                if (noRoots)
                {
                    tree.AddRootNode(current);
                }
            }
            else if (separator == BlockEndMagic)
            {
                if (roots.size() == 1)
                {
                    tree.AddRootNode(roots.top());
                }
                roots.pop();
            }
        }
    }

    B3dNodePtr DispatchBlock(const NodePtr& parent, const block_data::BlockHeader& blockHeader)
    {
        if (blockHeader.type == block_data::EmptyBlock0)
        {
            return ReadBlockData0(parent, blockHeader);
        }
        if (blockHeader.type == block_data::SimpleObjectConnectorBlock1)
        {
            return ReadBlockData1(parent, blockHeader);
        }
        else if (blockHeader.type == block_data::GroupUnknownBlock2)
        {
            return ReadBlockData2(parent, blockHeader);
        }
        else if (blockHeader.type == block_data::GroupRoadInfraObjectsBlock4)
        {
            return ReadBlockData4(parent, blockHeader);
        }
        else if (blockHeader.type == block_data::GroupObjectsBlock5)
        {
            return ReadBlockData5(parent, blockHeader);
        }
        else if (blockHeader.type == block_data::GroupVertexDataBlock7)
        {
            return ReadBlockData7(parent, blockHeader);
        }
        else if (blockHeader.type == block_data::SimpleFacesBlock8)
        {
            return ReadBlockData8(parent, blockHeader);
        }
        else if (blockHeader.type == block_data::GroupTriggerBlock9)
        {
            return ReadBlockData9(parent, blockHeader);
        }
        else if (blockHeader.type == block_data::GroupLodParametersBlock10)
        {
            return ReadBlockData10(parent, blockHeader);
        }
        else if (blockHeader.type == block_data::GroupUnknownBlock12)
        {
            return ReadBlockData12(parent, blockHeader);
        }
        else if (blockHeader.type == block_data::SimpleTriggerBlock13)
        {
            return ReadBlockData13(parent, blockHeader);
        }
        else if (blockHeader.type == block_data::SimpleUnknownBlock14)
        {
            return ReadBlockData14(parent, blockHeader);
        }
        else if (blockHeader.type == block_data::SimpleObjectConnectorBlock18)
        {
            return ReadBlockData18(parent, blockHeader);
        }
        else if (blockHeader.type == block_data::GroupObjectsBlock19)
        {
            return ReadBlockData19(parent, blockHeader);
        }
        else if (blockHeader.type == block_data::SimpleFlatCollisionBlock20)
        {
            return ReadBlockData20(parent, blockHeader);
        }
        else if (blockHeader.type == block_data::GroupObjectsBlock21)
        {
            return ReadBlockData21(parent, blockHeader);
        }
        else if (blockHeader.type == block_data::SimpleVolumeCollisionBlock23)
        {
            return ReadBlockData23(parent, blockHeader);
        }
        else if (blockHeader.type == block_data::GroupTransformMatrixBlock24)
        {
            return ReadBlockData24(parent, blockHeader);
        }
        else if (blockHeader.type == block_data::SimpleUnknownBlock25)
        {
            return ReadBlockData25(parent, blockHeader);
        }
        else if (blockHeader.type == block_data::SimpleFacesBlock28)
        {
            return ReadBlockData28(parent, blockHeader);
        }
        else if (blockHeader.type == block_data::GroupUnknownBlock29)
        {
            return ReadBlockData29(parent, blockHeader);
        }
        else if (blockHeader.type == block_data::SimplePortalBlock30)
        {
            return ReadBlockData30(parent, blockHeader);
        }
        else if (blockHeader.type == block_data::GroupLightingObjectBlock33)
        {
            return ReadBlockData33(parent, blockHeader);
        }
        else if (blockHeader.type == block_data::SimpleUnknownBlock34)
        {
            return ReadBlockData34(parent, blockHeader);
        }
        else if (blockHeader.type == block_data::SimpleFacesBlock35)
        {
            return ReadBlockData35(parent, blockHeader);
        }
        else if (blockHeader.type == block_data::GroupVertexDataBlock36)
        {
            return ReadBlockData36(parent, blockHeader);
        }
        else if (blockHeader.type == block_data::GroupVertexDataBlock37)
        {
            return ReadBlockData37(parent, blockHeader);
        }
        else if (blockHeader.type == block_data::GroupUnknownBlock39)
        {
            return ReadBlockData39(parent, blockHeader);
        }
        else if (blockHeader.type == block_data::SimpleGeneratedObjectsBlock40)
        {
            return ReadBlockData40(parent, blockHeader);
        }

        ThrowError("Unknown block id: " + std::to_string(blockHeader.type), "B3dReaderImpl::ReadBlock");
    }

    B3dNodePtr MakeHierarhyBreaker(const WeakNodePtr& parent)
    {
        block_data::BlockHeader blockHeader = MakeBlockHeader(common::ResourceName{}, block_data::HierarchyBreakerBlockXxx);

        block_data::HierarchyBreaker block{};
        return MakeVisitableNode(m_originalRoot, parent, blockHeader, block);
    }

    B3dNodePtr ReadBlockData0(const NodePtr& parent, const block_data::BlockHeader& blockHeader)
    {
        block_data::Empty0 block;

        ReadBytes(block.emptyData0, sizeof(block.emptyData0));
        block.unknown = ReadFloat();

        return MakeVisitableNode(m_originalRoot, parent, blockHeader, block);
    }

    B3dNodePtr ReadBlockData1(const NodePtr& parent, const block_data::BlockHeader& blockHeader)
    {
        block_data::SimpleObjectConnector1 block;

        ReadBytes(block.space.data(), block.space.size());
        ReadBytes(block.object.data(), block.object.size());

        return MakeVisitableNode(m_originalRoot, parent, blockHeader, block);
    }

    B3dNodePtr ReadBlockData2(const NodePtr& parent, const block_data::BlockHeader& blockHeader)
    {
        block_data::GroupUnknown2 block;

        block.boundingSphere = ReadBoundingSphere();
        block.unknown0 = ReadBoundingSphere();

        return MakeVisitableNode(m_originalRoot, parent, blockHeader, block);
    }

    B3dNodePtr ReadBlockData4(const NodePtr& parent, const block_data::BlockHeader& blockHeader)
    {
        block_data::GroupRoadInfraObjects4 block;

        block.boundingSphere = ReadBoundingSphere();

        ReadBytes(block.name.data(), block.name.size());
        ReadBytes(block.data.data(), block.data.size());

        return MakeVisitableNode(m_originalRoot, parent, blockHeader, block);
    }

    B3dNodePtr ReadBlockData5(const NodePtr& parent, const block_data::BlockHeader& blockHeader)
    {
        block_data::GroupObjects5 block;

        block.boundingSphere = ReadBoundingSphere();
        ReadBytes(block.name.data(), block.name.size());

        return MakeVisitableNode(m_originalRoot, parent, blockHeader, block);
    }

    B3dNodePtr ReadBlockData7(const NodePtr& parent, const block_data::BlockHeader& blockHeader)
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

        return MakeVisitableNode(m_originalRoot, parent, blockHeader, block);
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

    B3dNodePtr ReadBlockData8(const NodePtr& parent, const block_data::BlockHeader& blockHeader)
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

        return MakeVisitableNode(m_originalRoot, parent, blockHeader, block);
    }

    B3dNodePtr ReadBlockData9(const NodePtr& parent, const block_data::BlockHeader& blockHeader)
    {
        block_data::GroupTrigger9 block;

        block.boundingSphere = ReadBoundingSphere();
        block.unknown = ReadVector3();
        block.distanceToPlayer = ReadFloat();

        return MakeVisitableNode(m_originalRoot, parent, blockHeader, block);
    }

    B3dNodePtr ReadBlockData10(const NodePtr& parent, const block_data::BlockHeader& blockHeader)
    {
        block_data::GroupLodParameters10 block;

        block.boundingSphere = ReadBoundingSphere();
        block.unknown = ReadVector3();
        block.distanceToPlayer = ReadFloat();

        return MakeVisitableNode(m_originalRoot, parent, blockHeader, block);
    }

    B3dNodePtr ReadBlockData12(const NodePtr& parent, const block_data::BlockHeader& blockHeader)
    {
        block_data::GroupUnknown12 block;

        block.boundingSphere = ReadBoundingSphere();
        block.unknown0 = ReadBoundingSphere();
        block.unknown1 = ReadSomeUintsWithBuffer(false);

        return MakeVisitableNode(m_originalRoot, parent, blockHeader, block);
    }

    B3dNodePtr ReadBlockData13(const NodePtr& parent, const block_data::BlockHeader& blockHeader)
    {
        block_data::SimpleTrigger13 block;

        block.boundingSphere = ReadBoundingSphere();
        block.unknown0 = ReadSomeUintsWithBuffer();
        
        return MakeVisitableNode(m_originalRoot, parent, blockHeader, block);
    }

    B3dNodePtr ReadBlockData14(const NodePtr& parent, const block_data::BlockHeader& blockHeader)
    {
        block_data::SimpleUnknown14 block;
        
        block.boundingSphere = ReadBoundingSphere();

        block.unknown0 = ReadBoundingSphere();
        block.unknown1 = ReadSomeUintsWithBuffer();
        
        return MakeVisitableNode(m_originalRoot, parent, blockHeader, block);
    }

    B3dNodePtr ReadBlockData18(const NodePtr& parent, const block_data::BlockHeader& blockHeader)
    {
        block_data::SimpleObjectConnector18 block;

        block.boundingSphere = ReadBoundingSphere();
        common::ResourceName space;
        ReadBytes(space.data(), space.size());
        std::string spaceName = common::ResourceNameToString(space);
        if (spaceName == "$$world")
        {
            spaceName = "";
        }
        if (!spaceName.empty())
        {
            auto transformPos = m_originalRoot->transformations.find(spaceName);
            if (transformPos == m_originalRoot->transformations.end())
            {
                ThrowError("Cannot find transformation by name `" + spaceName + "`", "ReadBlockData18");
            }
            block.transformation = transformPos->second;
        }

        ReadBytes(block.object.data(), block.object.size());

        return MakeVisitableNode(m_originalRoot, parent, blockHeader, block);
    }

    B3dNodePtr ReadBlockData19(const NodePtr& parent, const block_data::BlockHeader& blockHeader)
    {
        block_data::GroupObjects19 block{};

        return MakeVisitableNode(m_originalRoot, parent, blockHeader, block);
    }

    B3dNodePtr ReadBlockData20(const NodePtr& parent, const block_data::BlockHeader& blockHeader)
    {
        block_data::SimpleFlatCollision20 block{};

        block.boundingSphere = ReadBoundingSphere();
        std::uint32_t verticesCount = ReadUint32();

        block.unknown0 = ReadSomeUintsWithBuffer();
        ReadCount(block.maybeVertices, verticesCount);

        return MakeVisitableNode(m_originalRoot, parent, blockHeader, block);
    }

    B3dNodePtr ReadBlockData21(const NodePtr& parent, const block_data::BlockHeader& blockHeader)
    {
        block_data::GroupObjects21 block;

        block.boundingSphere = ReadBoundingSphere();
        block.count = ReadUint32();
        block.defaultValue = ReadUint32();

        return MakeVisitableNode(m_originalRoot, parent, blockHeader, block);
    }

    B3dNodePtr ReadBlockData23(const NodePtr& parent, const block_data::BlockHeader& blockHeader)
    {
        block_data::SimpleVolumeCollision23 block;

        block.unknown0 = ReadSomeUintsWithBuffer();

        const std::uint32_t collisionCount = ReadUint32();
        for (std::uint32_t collisionNumber = 0; collisionNumber != collisionCount; ++collisionNumber)
        {
            const std::uint32_t vertexCount = ReadUint32();
            common::PositionList polygon;
            ReadCount(polygon, vertexCount);

            block.polygons.push_back(polygon);
        }

        return MakeVisitableNode(m_originalRoot, parent, blockHeader, block);
    }

    B3dNodePtr ReadBlockData24(const NodePtr& parent, const block_data::BlockHeader& blockHeader)
    {
        block_data::GroupTransformMatrix24 block;

        block.x = ReadVector3();
        block.y = ReadVector3();
        block.z = ReadVector3();

        block.position = ReadVector3();
        block.unknown = ReadUint32();

        return MakeVisitableNode(m_originalRoot, parent, blockHeader, block);
    }

    B3dNodePtr ReadBlockData25(const NodePtr& parent, const block_data::BlockHeader& blockHeader)
    {
        block_data::SimpleUnknown25 block;

        block.unknown0 = ReadUint32();
        block.unknown1 = ReadUint32();
        block.unknown2 = ReadUint32();
        ReadBytes(block.soundName.data(), block.soundName.size());
        block.unknown3 = ReadVector3();
        block.unknown4 = ReadVector3();
        for (auto& unknown : block.unknown5)
        {
            unknown = ReadFloat();
        }

        return MakeVisitableNode(m_originalRoot, parent, blockHeader, block);
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
        else if (face.type == block_data::Face28::UnknownMinus256)
        {
            face.meshInfo.texCoords.push_back(ReadVector2());
        }
        else
        {
            ThrowError("Unknown type", "B3dReaderImpl::DispatchFaceEntry28");
        }
    }

    B3dNodePtr ReadBlockData28(const NodePtr& parent, const block_data::BlockHeader& blockHeader)
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

        return MakeVisitableNode(m_originalRoot, parent, blockHeader, block);
    }

    B3dNodePtr ReadBlockData29(const NodePtr& parent, const block_data::BlockHeader& blockHeader)
    {
        block_data::GroupUnknown29 block;

        block.boundingSphere = ReadBoundingSphere();
        block.count = ReadUint32();
        block.unknown0 = ReadUint32();
        block.unknown1 = ReadBoundingSphere();
        ReadCount(block.unknown2, block.count);

        return MakeVisitableNode(m_originalRoot, parent, blockHeader, block);
    }

    B3dNodePtr ReadBlockData30(const NodePtr& parent, const block_data::BlockHeader& blockHeader)
    {
        block_data::SimplePortal30 block;

        block.boundingSphere = ReadBoundingSphere();
        ReadBytes(block.connectedRoom.data(), block.connectedRoom.size());

        block.leftDown = ReadVector3();
        block.upRight = ReadVector3();

        return MakeVisitableNode(m_originalRoot, parent, blockHeader, block);
    }

    B3dNodePtr ReadBlockData33(const NodePtr& parent, const block_data::BlockHeader& blockHeader)
    {
        block_data::GroupLightingObjects33 block;

        block.boundingSphere = ReadBoundingSphere();
        block.unknown0 = ReadUint32();
        block.unknown1 = ReadUint32();
        block.unknown2 = ReadUint32();
        block.position = ReadVector3();
        block.unknown3 = ReadVector3();
        for (auto& colorEntry : block.color)
        {
            colorEntry = ReadFloat();
        }

        return MakeVisitableNode(m_originalRoot, parent, blockHeader, block);
    }

    B3dNodePtr ReadBlockData34(const NodePtr& parent, const block_data::BlockHeader& blockHeader)
    {
        block_data::SimpleUnknown34 block;

        block.boundingSphere = ReadBoundingSphere();
        block.unknown0 = ReadFloat();

        std::uint32_t dataCount = ReadUint32();
        ReadCount(block.data, dataCount);

        return MakeVisitableNode(m_originalRoot, parent, blockHeader, block);
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

    B3dNodePtr ReadBlockData35(const NodePtr& parent, const block_data::BlockHeader& blockHeader)
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

        return MakeVisitableNode(m_originalRoot, parent, blockHeader, block);
    }

    B3dNodePtr ReadBlockData36(const NodePtr& parent, const block_data::BlockHeader& blockHeader)
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

        return MakeVisitableNode(m_originalRoot, parent, blockHeader, block);
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

    B3dNodePtr ReadBlockData37(const NodePtr& parent, const block_data::BlockHeader& blockHeader)
    {
        block_data::GroupVertexData37 block;

        block.boundingSphere = ReadBoundingSphere();
        ReadBytes(block.name.data(), block.name.size());
        block.type = ReadUint32();

        DispatchVertexData37(block);

        return MakeVisitableNode(m_originalRoot, parent, blockHeader, block);
    }

    B3dNodePtr ReadBlockData39(const NodePtr& parent, const block_data::BlockHeader& blockHeader)
    {
        block_data::GroupUnknown39 block;

        ReadBytes(block.unknown.data(), block.unknown.size());

        return MakeVisitableNode(m_originalRoot, parent, blockHeader, block);
    }

    B3dNodePtr ReadBlockData40(const NodePtr& parent, const block_data::BlockHeader& blockHeader)
    {
        block_data::SimpleGeneratedObjects40 block;

        block.boundingSphere = ReadBoundingSphere();
        ReadBytes(block.empty.data(), block.empty.size());
        ReadBytes(block.name.data(), block.name.size());

        block.unknown0 = ReadSomeUintsWithBuffer();

        if (block.unknown0.unknown0 != 0)
        {
            OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS, "SimpleGeneratedObjects40: unknown0.unknown0 is not zero: " + std::to_string(block.unknown0.unknown0));
        }

        return MakeVisitableNode(m_originalRoot, parent, blockHeader, block);
    }
};

static B3dTreePtr ReadTree(const std::string& rootDir, const std::string& dir, const std::string& id)
{
    const std::string fullB3dName = rootDir + "/" + dir + "/" + id + ".b3d";
    std::ifstream inputFile{ fullB3dName, std::ios_base::binary };
    if (!inputFile)
    {
        OGRE_EXCEPT(Ogre::Exception::ERR_FILE_NOT_FOUND, "failed to open file `" + fullB3dName + "`");
    }

    Ogre::FileStreamDataStream dataStream(&inputFile, false);
    B3dReaderImpl reader{ dataStream };
    
    return reader.Read(dir, id);
}

B3dForest ReadB3d(const B3dRegistry& registry)
{
    B3dForest forest;
    forest.common = ReadTree(registry.rootDir, "COMMON", "COMMON");
    forest.trucks = ReadTree(registry.rootDir, "COMMON", "TRUCKS");
    for (const auto& id : registry.entries)
    {
        forest.forest.push_back(ReadTree(registry.rootDir, registry.dir, id));
    }

    return forest;
}

} // namespace b3d
} // namespace data
} // namespace resource
} // namespace d2_hack
