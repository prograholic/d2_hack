#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <stdexcept>

#include <OgreLogManager.h>

#include <d2_hack/resource/data/b3d_reader.h>
#include <d2_hack/resource/data/b3d_visitor.h>
#include <d2_hack/resource/data/b3d_tree_optimization.h>
#include <d2_hack/common/utils.h>

namespace d2_hack
{
namespace resource
{
namespace data
{
namespace b3d
{

class TracingVisitor : public NodeVisitorInterface
{
public:
    TracingVisitor(bool printBoundingSphere, bool newLineForVectorData, bool printVectorData, bool printFaceInfo, bool printMeshInfo)
        : m_printBoundingSphere(printBoundingSphere)
        , m_newLineForVectorData(newLineForVectorData)
        , m_printVectorData(printVectorData)
        , m_printFaceInfo(printFaceInfo)
        , m_printMeshInfo(printMeshInfo)
    {
    }

    void Visit(const common::Materials& materials)
    {
        PrintData(materials, "materials");
    }

    void Visit(const TransformationMap& transformations)
    {
        PrintData(transformations, "transformations");
    }

    void Visit(const block_data::VolumeCollisionList& volumeCollisions)
    {
        PrintData(volumeCollisions, "volumeCollisions");
    }

    void SetLevel(int level)
    {
        m_offset = level;
    }

    void BeginNested()
    {
        GetStream(1) << "nested" << std::endl;
        GetStream(1) << "{" << std::endl;
    }

    void EndNested()
    {
        GetStream(1) << "}" << std::endl;
    }

    void EndNode()
    {
        GetStream() << "}" << std::endl;
    }

    template <typename BlockType>
    std::string GetBlockNamePrefix(const BlockType& /* block */)
    {
        return std::string(BlockType::Name) + ": ";
    }

    template <typename NodeType>
    void ProcessBlock(const NodeType& node)
    {
        GetStream() << GetBlockNamePrefix(node.GetBlockData()) << ToString(node.GetName()) << std::endl;
        GetStream() << "{" << std::endl;
        GetStream(1) << "boundingSphere: " << ToString(node.GetBoundingSphere()) << "," << std::endl;

        PrintData(node.GetBlockData(), "", 1);
    }

    virtual VisitResult Visit(NodeHierarchyBreaker& node, VisitMode /* visitMode */) override
    {
        ProcessBlock(node);

        return VisitResult::Continue;
    }

    virtual VisitResult Visit(NodeEmpty0& node, VisitMode /* visitMode */) override
    {
        ProcessBlock(node);

        return VisitResult::Continue;
    }

    virtual VisitResult Visit(NodeSimpleObjectConnector1& node, VisitMode /* visitMode */) override
    {
        ProcessBlock(node);

        return VisitResult::Continue;
    }

    virtual VisitResult Visit(NodeGroupUnknown2& node, VisitMode /* visitMode */) override
    {
        ProcessBlock(node);

        return VisitResult::Continue;
    }

    virtual VisitResult Visit(NodeGroupRoadInfraObjects4& node, VisitMode /* visitMode */) override
    {
        ProcessBlock(node);

        return VisitResult::Continue;
    }

    virtual VisitResult Visit(NodeGroupObjects5& node, VisitMode /* visitMode */) override
    {
        ProcessBlock(node);

        return VisitResult::Continue;
    }

    virtual VisitResult Visit(NodeGroupVertexData7& node, VisitMode /* visitMode */) override
    {
        ProcessBlock(node);

        return VisitResult::Continue;
    }

    virtual VisitResult Visit(NodeSimpleFaces8& node, VisitMode /* visitMode */) override
    {
        ProcessBlock(node);

        return VisitResult::Continue;
    }

    virtual VisitResult Visit(NodeGroupTrigger9& node, VisitMode /* visitMode */) override
    {
        ProcessBlock(node);

        return VisitResult::Continue;
    }

    virtual VisitResult Visit(NodeGroupLodParameters10& node, VisitMode /* visitMode */) override
    {
        ProcessBlock(node);

        return VisitResult::Continue;
    }

    virtual VisitResult Visit(NodeGroupUnknown12& node, VisitMode /* visitMode */) override
    {
        ProcessBlock(node);

        return VisitResult::Continue;
    }

    virtual VisitResult Visit(NodeSimpleTrigger13& node, VisitMode /* visitMode */) override
    {
        ProcessBlock(node);

        return VisitResult::Continue;
    }

    virtual VisitResult Visit(NodeSimpleUnknown14& node, VisitMode /* visitMode */) override
    {
        ProcessBlock(node);

        return VisitResult::Continue;
    }

    virtual VisitResult Visit(NodeSimpleObjectConnector18& node, VisitMode /* visitMode */) override
    {
        ProcessBlock(node);

        return VisitResult::Continue;
    }

    virtual VisitResult Visit(NodeGroupObjects19& node, VisitMode /* visitMode */) override
    {
        ProcessBlock(node);

        return VisitResult::Continue;
    }

    virtual VisitResult Visit(NodeSimpleFlatCollision20& node, VisitMode /* visitMode */) override
    {
        ProcessBlock(node);

        return VisitResult::Continue;
    }

    virtual VisitResult Visit(NodeGroupObjects21& node, VisitMode /* visitMode */) override
    {
        ProcessBlock(node);

        return VisitResult::Continue;
    }

    virtual VisitResult Visit(NodeSimpleVolumeCollision23& node, VisitMode /* visitMode */) override
    {
        ProcessBlock(node);

        return VisitResult::Continue;
    }

    virtual VisitResult Visit(NodeGroupTransformMatrix24& node, VisitMode /* visitMode */) override
    {
        ProcessBlock(node);

        return VisitResult::Continue;
    }

    virtual VisitResult Visit(NodeSimpleUnknown25& node, VisitMode /* visitMode */) override
    {
        ProcessBlock(node);

        return VisitResult::Continue;
    }

    virtual VisitResult Visit(NodeSimpleFaces28& node, VisitMode /* visitMode */) override
    {
        ProcessBlock(node);

        return VisitResult::Continue;
    }

    virtual VisitResult Visit(NodeGroupUnknown29& node, VisitMode /* visitMode */) override
    {
        ProcessBlock(node);

        return VisitResult::Continue;
    }

    virtual VisitResult Visit(NodeSimplePortal30& node, VisitMode /* visitMode */) override
    {
        ProcessBlock(node);

        return VisitResult::Continue;
    }

    virtual VisitResult Visit(NodeGroupLightingObjects33& node, VisitMode /* visitMode */) override
    {
        ProcessBlock(node);

        return VisitResult::Continue;
    }

    virtual VisitResult Visit(NodeSimpleUnknown34& node, VisitMode /* visitMode */) override
    {
        ProcessBlock(node);

        return VisitResult::Continue;
    }

    virtual VisitResult Visit(NodeSimpleFaces35& node, VisitMode /* visitMode */) override
    {
        ProcessBlock(node);

        return VisitResult::Continue;
    }

    virtual VisitResult Visit(NodeGroupVertexData36& node, VisitMode /* visitMode */) override
    {
        ProcessBlock(node);

        return VisitResult::Continue;
    }

    virtual VisitResult Visit(NodeGroupVertexData37& node, VisitMode /* visitMode */) override
    {
        ProcessBlock(node);

        return VisitResult::Continue;
    }

    virtual VisitResult Visit(NodeGroupUnknown39& node, VisitMode /* visitMode */) override
    {
        ProcessBlock(node);

        return VisitResult::Continue;
    }

    virtual VisitResult Visit(NodeSimpleGeneratedObjects40& node, VisitMode /* visitMode */) override
    {
        ProcessBlock(node);

        return VisitResult::Continue;
    }

private:
    bool m_printBoundingSphere;
    bool m_newLineForVectorData;
    bool m_printVectorData;
    bool m_printFaceInfo;
    bool m_printMeshInfo;
    size_t m_offset = 0;

    std::string GetOffsetString(int adjustOffset = 0) const
    {
        std::string res;
        res.resize((m_offset + adjustOffset) * 4, ' ');

        return res;
    }


private:
    std::ostream& GetStream(int adjustOffset = 0, std::ostream& ostream = std::cout)
    {
        ostream << GetOffsetString(adjustOffset);
        return ostream;
    }

    void PrintData(Ogre::Real data, const char* /* name */, int adjustOffset)
    {
        GetStream(adjustOffset) << data << std::endl;
    }

    void PrintData(std::uint32_t data, const char* /* name */, int adjustOffset)
    {
        GetStream(adjustOffset) << data << std::endl;
    }

    void PrintData(const Ogre::Vector3& data, const char* /* name */, int adjustOffset)
    {
        GetStream(adjustOffset) << data << std::endl;
    }

    void PrintData(const Ogre::Vector2& data, const char* /* name */, int adjustOffset)
    {
        GetStream(adjustOffset) << data << std::endl;
    }

    void PrintData(const common::ResourceName& name, const char* /* name */, int adjustOffset)
    {
        GetStream(adjustOffset) << ToString(name) << std::endl;
    }

    void PrintData(const common::SimpleMeshInfo& meshInfo, const char* /* name */, int adjustOffset)
    {
        GetStream(adjustOffset) << "SimpleMeshInfo" << std::endl;
        GetStream(adjustOffset) << "{" << std::endl;

        PrintData(meshInfo.positions, "positions", adjustOffset + 1, m_printMeshInfo);
        PrintData(meshInfo.texCoords, "texCoords", adjustOffset + 1, m_printMeshInfo);
        PrintData(meshInfo.normals, "normals", adjustOffset + 1, m_printMeshInfo);
        GetStream(adjustOffset) << "}" << std::endl;
    }

    void PrintData(const Transform& data, const char* /* name */, int adjustOffset)
    {
        GetStream(adjustOffset) << "Transform" << std::endl;
        GetStream(adjustOffset) << "{" << std::endl;
        GetStream(adjustOffset + 1) << "matrix: " << ToString(data.matrix) << "," << std::endl;
        GetStream(adjustOffset + 1) << "matrix: " << ToString(data.position) << "," << std::endl;
        GetStream(adjustOffset) << "}" << std::endl;
    }

    void PrintData(const block_data::HierarchyBreaker& /* block */, const char* /* name */, int /* adjustOffset */)
    {
    }

    void PrintData(const block_data::Empty0& block, const char* /* name */, int adjustOffset)
    {
        GetStream(adjustOffset) << "emptyData0: " << ToString(std::begin(block.emptyData0), std::end(block.emptyData0)) << "," << std::endl;
        GetStream(adjustOffset) << "unknown: " << block.unknown << "," << std::endl;
    }

    void PrintData(const block_data::SimpleObjectConnector1& block, const char* /* name */, int adjustOffset)
    {
        GetStream(adjustOffset) << "space: " << ToString(block.space) << "," << std::endl;
        GetStream(adjustOffset) << "object: " << ToString(block.object) << std::endl;
    }

    void PrintData(const block_data::GroupUnknown2& block, const char* /* name */, int adjustOffset)
    {
        GetStream(adjustOffset) << "unknown0: " << block.unknown0 << "," << std::endl;
        GetStream(adjustOffset) << "unknown1: " << block.unknown1 << "," << std::endl;
    }

    void PrintData(const block_data::GroupRoadInfraObjects4& block, const char* /* name */, int adjustOffset)
    {
        GetStream(adjustOffset) << "name: " << ToString(block.name) << "," << std::endl;
        GetStream(adjustOffset) << "data: " << ToString(block.data.data(), block.data.data() + block.data.size()) << std::endl;
    }

    void PrintData(const block_data::GroupObjects5& block, const char* /* name */, int adjustOffset)
    {
        GetStream(adjustOffset) << "name: " << ToString(block.name) << std::endl;
    }

    void PrintData(const block_data::GroupVertexData7& block, const char* /* name */, int adjustOffset)
    {
        GetStream(adjustOffset) << "name: " << ToString(block.name) << std::endl;
        PrintData(block.meshInfo, "", adjustOffset);
    }

    void PrintData(const block_data::Face8::Unknown177& data, const char* /* name */, int adjustOffset)
    {
        GetStream(adjustOffset) << "Face8::Unknown177" << std::endl;
        GetStream(adjustOffset) << "{" << std::endl;
        GetStream(adjustOffset + 1) << "unknown: " << ToString(data.unknown) << "," << std::endl;
        GetStream(adjustOffset) << "}" << std::endl;
    }

    void PrintData(const block_data::Face8& data, const char* /* name */, int adjustOffset)
    {
        GetStream(adjustOffset) << "Face8" << std::endl;
        GetStream(adjustOffset) << "{" << std::endl;

        GetStream(adjustOffset + 1) << "type: " << data.type << std::endl;
        GetStream(adjustOffset + 1) << "unknown0: " << data.unknown0 << std::endl;
        GetStream(adjustOffset + 1) << "unknown1: " << data.unknown1 << std::endl;
        GetStream(adjustOffset + 1) << "materialIndex: " << data.materialIndex << std::endl;
        PrintData(data.meshInfo, "", adjustOffset + 1);

        GetStream(adjustOffset) << "}" << std::endl;
    }

    void PrintData(const block_data::SimpleFaces8& block, const char* /* name */, int adjustOffset)
    {
        PrintData(block.faces, "faces", adjustOffset, m_printFaceInfo);
    }

    void PrintData(const block_data::GroupTrigger9& block, const char* /* name */, int adjustOffset)
    {
        GetStream(adjustOffset) << "unknown: " << block.unknown << "," << std::endl;
        GetStream(adjustOffset) << "distanceToPlayer: " << block.distanceToPlayer << std::endl;
    }

    void PrintData(const block_data::GroupLodParameters10& block, const char* /* name */, int adjustOffset)
    {
        GetStream(adjustOffset) << "unknown: " << ToString(block.unknown) << "," << std::endl;
        GetStream(adjustOffset) << "distanceToPlayer: " << block.distanceToPlayer << std::endl;
    }

    void PrintData(const block_data::GroupUnknown12& block, const char* /* name */, int adjustOffset)
    {
        GetStream(adjustOffset) << "unknown0: " << ToString(block.unknown0) << "," << std::endl;
        GetStream(adjustOffset) << "unknown1: " << ToString(block.unknown1) << "," << std::endl;
        GetStream(adjustOffset) << "unknown2: " << ToString(block.unknown2) << "," << std::endl;
        GetStream(adjustOffset) << "unknown3: " << ToString(block.unknown3) << "," << std::endl;
        GetStream(adjustOffset) << "unknown4: " << ToString(block.unknown4) << "," << std::endl;
        GetStream(adjustOffset) << "unknown5: " << ToString(block.unknown5) << std::endl;
    }

    void PrintData(const block_data::SimpleTrigger13& block, const char* /* name */, int adjustOffset)
    {
        GetStream(adjustOffset) << "unknown0: " << block.unknown0 << "," << std::endl;
        GetStream(adjustOffset) << "possibleNumberOrId: " << block.possibleNumberOrId << std::endl;
        PrintData(block.unknown2, "unknown2", adjustOffset);
    }

    void PrintData(const block_data::SimpleUnknown14& block, const char* /* name */, int adjustOffset)
    {
        GetStream(adjustOffset) << "unknown0: " << block.unknown0 << "," << std::endl;
        GetStream(adjustOffset) << "unknown1: " << block.unknown1 << "," << std::endl;
        GetStream(adjustOffset) << "unknown2: " << block.unknown2 << "," << std::endl;
        GetStream(adjustOffset) << "unknown3: " << block.unknown3 << "," << std::endl;
        GetStream(adjustOffset) << "unknown4: " << block.unknown4 << "," << std::endl;
        GetStream(adjustOffset) << "unknown5: " << block.unknown5 << "," << std::endl;
        GetStream(adjustOffset) << "unknown6: " << block.unknown6 << std::endl;
    }

    void PrintData(const block_data::SimpleObjectConnector18& block, const char* /* name */, int adjustOffset)
    {
        PrintData(block.transformation, "transformation", adjustOffset);
        GetStream(adjustOffset) << "object: " << ToString(block.object) << std::endl;
    }

    void PrintData(const block_data::GroupObjects19& /* block */, const char* /* name */, int /* adjustOffset */)
    {
    }

    void PrintData(const block_data::SimpleFlatCollision20& block, const char* /* name */, int adjustOffset)
    {
        GetStream(adjustOffset) << "unknown0: " << block.unknown0 << "," << std::endl;
        GetStream(adjustOffset) << "unknown1: " << block.unknown1 << std::endl;
        PrintData(block.unknown2, "unknown2", adjustOffset);
        PrintData(block.unknown3, "unknown3", adjustOffset);
    }

    void PrintData(const block_data::GroupObjects21& block, const char* /* name */, int adjustOffset)
    {
        GetStream(adjustOffset) << "count: " << block.count << "," << std::endl;
        GetStream(adjustOffset) << "unknown: " << block.unknown << std::endl;
    }

    void PrintData(const block_data::SimpleVolumeCollision23& block, const char* /* name */, int adjustOffset)
    {
        GetStream(adjustOffset) << "unknown0: " << block.unknown0 << "," << std::endl;
        GetStream(adjustOffset) << "surfaceType: " << block.surfaceType << "," << std::endl;
        PrintData(block.unknown1, "unknown1", adjustOffset);
        PrintData(block.polygons, "polygons", adjustOffset);
    }

    void PrintData(const block_data::GroupTransformMatrix24& block, const char* /* name */, int adjustOffset)
    {
        GetStream(adjustOffset) << "x: " << block.x << "," << std::endl;
        GetStream(adjustOffset) << "y: " << block.y << "," << std::endl;
        GetStream(adjustOffset) << "z: " << block.z << "," << std::endl;
        GetStream(adjustOffset) << "position: " << block.position << "," << std::endl;
        GetStream(adjustOffset) << "unknown: " << block.unknown << std::endl;
    }

    void PrintData(const block_data::SimpleUnknown25& block, const char* /* name */, int adjustOffset)
    {
        GetStream(adjustOffset) << "unknown0: " << block.unknown0 << "," << std::endl;
        GetStream(adjustOffset) << "unknown1: " << block.unknown1 << "," << std::endl;
        GetStream(adjustOffset) << "unknown2: " << block.unknown2 << "," << std::endl;
        GetStream(adjustOffset) << "soundname: \"" << common::ResourceNameToString(block.soundName) << "\"," << std::endl;
        GetStream(adjustOffset) << "unknown3: {"
            << block.unknown3[0] << ", "
            << block.unknown3[1] << ", "
            << block.unknown3[2] << ", "
            << block.unknown3[3] << ", "
            << block.unknown3[4] << ", "
            << block.unknown3[5] << ", "
            << block.unknown3[6] << ", "
            << block.unknown3[7] << ", "
            << block.unknown3[8] << ", "
            << block.unknown3[9] << ", "
            << block.unknown3[10] << "}" << std::endl;
    }

    void PrintData(const block_data::Face28& data, const char* /* name */, int adjustOffset)
    {
        GetStream(adjustOffset) << "Face28" << std::endl;
        GetStream(adjustOffset) << "{" << std::endl;
        if (m_printFaceInfo)
        {
            GetStream(adjustOffset + 1) << "type: " << data.type << std::endl;
            GetStream(adjustOffset + 1) << "unknown0: " << data.unknown0 << std::endl;
            GetStream(adjustOffset + 1) << "unknown1: " << data.unknown1 << std::endl;
            GetStream(adjustOffset + 1) << "materialIndex: " << data.materialIndex << std::endl;
            PrintData(data.meshInfo, "", adjustOffset + 1);
            PrintData(data.unknown, "unknown", adjustOffset + 1);
        }
        else
        {
            GetStream(adjustOffset + 1) << "//..." << std::endl;
        }
        GetStream(adjustOffset) << "}" << std::endl;
    }

    void PrintData(const block_data::Face28::Unknown& data, const char* /* name */, int adjustOffset)
    {
        if (m_printFaceInfo)
        {
            GetStream(adjustOffset) << "Face28::Unknown" << std::endl;
            GetStream(adjustOffset) << "{" << std::endl;
            GetStream(adjustOffset + 1) << "unknown: " << data.unknown << std::endl;
            GetStream(adjustOffset) << "}" << std::endl;
        }
    }

    void PrintData(const block_data::SimpleFaces28& block, const char* /* name */, int adjustOffset)
    {
        GetStream(adjustOffset) << "unknown: " << ToString(block.unknown) << std::endl;
        PrintData(block.faces, "faces", adjustOffset);
    }

    void PrintData(const block_data::GroupUnknown29& block, const char* /* name */, int adjustOffset)
    {
        GetStream(adjustOffset) << "type: " << ToString(block.type) << "," << std::endl;
        GetStream(adjustOffset) << "unknown0: " << ToString(block.unknown0) << "," << std::endl;
        GetStream(adjustOffset) << "unknown1: "
            "{" << block.unknown1[0] << ", "
            "" << block.unknown1[1] << ", "
            "" << block.unknown1[2] << ", "
            "" << block.unknown1[3] << ", "
            "" << block.unknown1[4] << ", "
            "" << block.unknown1[5] << ", "
            "" << block.unknown1[6] << ", "
            "" << block.unknown1[7] << ", "
            "}" << std::endl;
    }

    void PrintData(const block_data::SimplePortal30& block, const char* /* name */, int adjustOffset)
    {
        GetStream(adjustOffset) << "connectedRoom: " << ToString(block.connectedRoom) << "," << std::endl;
        GetStream(adjustOffset) << "leftDown: " << ToString(block.leftDown) << "," << std::endl;
        GetStream(adjustOffset) << "upRight: " << ToString(block.upRight) << std::endl;
    }

    void PrintData(const block_data::GroupLightingObjects33& block, const char* /* name */, int adjustOffset)
    {
        GetStream(adjustOffset) << "unknown0: " << block.unknown0 << "," << std::endl;
        GetStream(adjustOffset) << "unknown1: " << block.unknown1 << "," << std::endl;
        GetStream(adjustOffset) << "unknown2: " << block.unknown2 << "," << std::endl;
        GetStream(adjustOffset) << "position: " << ToString(block.position) << "," << std::endl;
        GetStream(adjustOffset) << "color: {"
            << block.color[0] << ", "
            << block.color[1] << ", "
            << block.color[2] << ", "
            << block.color[3] << ", "
            << block.color[4] << ", "
            << block.color[5] << ", "
            << block.color[6] << ", "
            << block.color[7] << ", "
            << block.color[8] << ", "
            << block.color[9] << ", "
            << block.color[10] << ", "
            << block.color[11] << "}" << std::endl;
    }

    void PrintData(const block_data::SimpleUnknown34::Unknown& data, const char* /* name */, int adjustOffset)
    {
        GetStream(adjustOffset) << "SimpleUnknown34::Unknown" << std::endl;
        GetStream(adjustOffset) << "{" << std::endl;
        GetStream(adjustOffset + 1) << "unknown0: " << ToString(data.unknown0) << "," << std::endl;
        GetStream(adjustOffset + 1) << "unknown1: " << ToString(data.unknown1) << "," << std::endl;
        GetStream(adjustOffset + 1) << "unknown2: " << ToString(data.unknown2) << "," << std::endl;
        GetStream(adjustOffset + 1) << "unknown3: " << ToString(data.unknown3) << "," << std::endl;
        GetStream(adjustOffset) << "}" << std::endl;
    }

    void PrintData(const block_data::SimpleUnknown34& block, const char* /* name */, int adjustOffset)
    {
        GetStream(adjustOffset) << "unknown0: " << block.unknown0 << "," << std::endl;
        PrintData(block.data, "data", adjustOffset);
    }

    void PrintData(const block_data::Face35& data, const char* /* name */, int adjustOffset)
    {
        GetStream(adjustOffset) << "Face35" << std::endl;
        GetStream(adjustOffset) << "{" << std::endl;

        if (m_printFaceInfo)
        {
            GetStream(adjustOffset + 1) << "type: " << data.type << std::endl;
            GetStream(adjustOffset + 1) << "unknown0: " << data.unknown0 << std::endl;
            GetStream(adjustOffset + 1) << "unknown1: " << data.unknown1 << std::endl;
            GetStream(adjustOffset + 1) << "materialIndex: " << data.materialIndex << std::endl;
            PrintData(data.meshInfo, "", adjustOffset + 1);
            PrintData(data.unknown49, "unknown49", adjustOffset + 1);
        }
        else
        {
            GetStream(adjustOffset + 1) << "//..." << std::endl;
        }
        GetStream(adjustOffset) << "}" << std::endl;
    }

    void PrintData(const block_data::SimpleFaces35& block, const char* /* name */, int adjustOffset)
    {
        GetStream(adjustOffset) << "type: " << block.type << "," << std::endl;
        GetStream(adjustOffset) << "materialIndex: " << block.materialIndex << std::endl;
        PrintData(block.faces, "faces", adjustOffset, m_printFaceInfo);
    }

    void PrintData(const block_data::Face35::Unknown49& data, const char* /* name */, int adjustOffset)
    {
        GetStream(adjustOffset) << "Face35::Unknown49" << std::endl;
        GetStream(adjustOffset) << "{" << std::endl;
        GetStream(adjustOffset + 1) << "unknown: " << ToString(data.unknown) << "," << std::endl;
        GetStream(adjustOffset) << "}" << std::endl;
    }

    void PrintData(const block_data::GroupVertexData36& block, const char* /* name */, int adjustOffset)
    {
        GetStream(adjustOffset) << "unknown0: " << ToString(block.unknown0.data(), block.unknown0.data() + block.unknown0.size()) << std::endl;
        GetStream(adjustOffset) << "type: " << block.type << std::endl;
        PrintData(block.meshInfo, "", adjustOffset);
    }

    void PrintData(const block_data::GroupVertexData37::Unknown514& data, const char* /* name */, int adjustOffset)
    {
        GetStream(adjustOffset) << "GroupVertexData37::Unknown514" << std::endl;
        GetStream(adjustOffset) << "{" << std::endl;
        GetStream(adjustOffset + 1) << "unknown: " << data.unknown << std::endl;
        GetStream(adjustOffset) << "}" << std::endl;
    }

    void PrintData(const block_data::GroupVertexData37::Unknown258Or515& data, const char* /* name */, int adjustOffset)
    {
        GetStream(adjustOffset) << "GroupVertexData37::Unknown258Or515" << std::endl;
        GetStream(adjustOffset) << "{" << std::endl;
        GetStream(adjustOffset + 1) << "unknown: " << ToString(data.unknown) << std::endl;
        GetStream(adjustOffset) << "}" << std::endl;
    }

    void PrintData(const block_data::GroupVertexData37& block, const char* /* name */, int adjustOffset)
    {
        GetStream(adjustOffset) << "name: " << ToString(block.name) << "," << std::endl;
        GetStream(adjustOffset) << "type: " << block.type << std::endl;
        PrintData(block.meshInfo, "", 1);
        PrintData(block.unknown258Or515, "unknown258Or515", adjustOffset);
        PrintData(block.unknown514, "unknown514", adjustOffset);
    }

    void PrintData(const block_data::GroupUnknown39& block, const char* /* name */, int adjustOffset)
    {
        GetStream(adjustOffset) << "unknown: " << ToString(block.unknown.data(), block.unknown.data() + block.unknown.size()) << std::endl;
    }

    void PrintData(const block_data::SimpleGeneratedObjects40& block, const char* /* name */, int adjustOffset)
    {
        GetStream(adjustOffset) << "empty: " << ToString(block.empty) << "," << std::endl;
        GetStream(adjustOffset) << "name: " << ToString(block.name) << "," << std::endl;
        GetStream(adjustOffset) << "type: " << block.type << "," << std::endl;
        GetStream(adjustOffset) << "unknown0: " << block.unknown0 << std::endl;
        PrintData(block.unknown1, "unknown1", adjustOffset);
    }

    std::string ToString(const std::string& data)
    {
        return "\"" + data + "\"";
    }

    std::string ToString(const uint8_t* first, const uint8_t* last)
    {
        std::ostringstream ostream;
        ostream << "{";
        for (; first != last; ++first)
        {
            ostream << "0x" << std::setw(2) << std::setfill('0') << std::hex << static_cast<unsigned int>(*first) << ", ";
        }
        ostream << "}";

        return ostream.str();
    }

    std::string ToString(const common::BoundingSphere& boundingSphere)
    {
        std::ostringstream ostream;
        ostream << "BoundingSphere {";

        if (m_printBoundingSphere)
        {
            ostream <<
                "center: " << ToString(boundingSphere.center) << ", "
                "radius: " << boundingSphere.radius;
        }
        ostream << "}";

        return ostream.str();
    }

    std::string ToString(const common::ResourceName& resourceName)
    {
        return "\"" + common::ResourceNameToString(resourceName) + "\"";
    }

    std::string ToString(const Ogre::Vector3& vector)
    {
        std::ostringstream ostream;
        ostream << vector;

        return ostream.str();
    }

    std::string ToString(const Ogre::Matrix3& matrix)
    {
        std::ostringstream ostream;
        ostream << matrix;

        return ostream.str();
    }

    std::string ToString(Ogre::Real value)
    {
        return std::to_string(value);
    }

    std::string ToString(common::Index index)
    {
        return std::to_string(index);
    }

    std::string ToString(const Ogre::Vector2& vector)
    {
        std::ostringstream ostream;
        ostream << vector;

        return ostream.str();
    }

    template <typename T, typename A>
    void PrintData(const std::vector<T, A>& data, const char* name, int adjustOffset = 0, bool printVectorData = true)
    {
        GetStream(adjustOffset) << name << "(" << data.size() << ")" << std::endl;
        GetStream(adjustOffset) << "{" << std::endl;

        if (m_printVectorData && printVectorData)
        {
            for (const auto& item : data)
            {
                PrintData(item, "", adjustOffset + 1);
            }
        }
        else
        {
            GetStream(adjustOffset + 1) << "//..." << std::endl;
        }
        GetStream(adjustOffset) << "}" << std::endl;
    }

    template <typename T, typename P, typename A>
    void PrintData(const std::map<T, P, A>& data, const char* name, int adjustOffset = 0, bool printVectorData = true)
    {
        GetStream() << name << " (" << data.size() << "):" << std::endl;
        GetStream() << "{" << std::endl;
        if (m_printVectorData && printVectorData)
        {
            GetStream(adjustOffset) << "}" << std::endl;
            for (auto pos : data)
            {
                PrintData(pos.second, pos.first.c_str(), adjustOffset + 1);
            }
        }
        else
        {
            GetStream(adjustOffset + 1) << "//..." << std::endl;
        }
        GetStream() << "}" << std::endl;
    }
};


void VisitNode(const NodePtr& node, TracingVisitor& visitor, int level)
{
    visitor.SetLevel(level);
    auto unusedRes = node->Visit(visitor, VisitMode::PreOrder);
    (void)unusedRes;

    const auto& childNodes = node->GetChildNodeList();
    if (!childNodes.empty())
    {
        visitor.BeginNested();

        for (auto child : childNodes)
        {
            VisitNode(child, visitor, level + 2);
        }

        visitor.SetLevel(level);
        visitor.EndNested();
    }
    visitor.EndNode();
}

void VisitTree(const B3dTree& tree, TracingVisitor& visitor)
{
    visitor.SetLevel(0);
    visitor.Visit(tree.materials);
    visitor.Visit(tree.transformations);
    visitor.Visit(tree.volumeCollisions);

    for (auto node : tree.rootNodes)
    {
        VisitNode(node, visitor, 0);
    }
}

std::string GetOffsetString(int indent)
{
    std::string res;
    res.resize(indent * 4, ' ');

    return res;
}
    
std::ostream& GetStream(int indent = 0, std::ostream& ostream = std::cout)
{
    ostream << GetOffsetString(indent);
    return ostream;
}

void PrintSequence(const std::vector<std::uint32_t>& sequence)
{
    if (!sequence.empty())
    {
        auto it = sequence.begin();
        std::cout << *it;
        ++it;
        while (it != sequence.end())
        {
            std::cout << " -> " << *it;
            ++it;
        }
        std::cout << std::endl;
    }
}


void PrintOnlyTypes(const NodePtr& node, const std::vector<std::uint32_t>& sequence, std::set< std::vector<std::uint32_t>>& uniqueSequences, int indent = 0)
{
    GetStream(indent) << node->GetTypeName() << "(" << node->GetName() << ")";
    if (!node->GetChildNodeList().empty())
    {
        std::cout << std::endl;
    }

    for (auto child : node->GetChildNodeList())
    {
        std::vector<std::uint32_t> newSequence = sequence;
        newSequence.push_back(child->GetType());

        PrintOnlyTypes(child, newSequence, uniqueSequences, indent + 1);
    }

    if (node->GetChildNodeList().empty())
    {
        uniqueSequences.insert(sequence);
        PrintSequence(sequence);
    }
}


void PrintSequences(const std::set< std::vector<std::uint32_t>>& sequences)
{
    for (const auto& seq : sequences)
    {
        PrintSequence(seq);
    }
}

void PrintFirstElemsOfSequences(const std::set< std::vector<std::uint32_t>>& sequences)
{
    std::set<std::uint32_t> roots;
    for (const auto& seq : sequences)
    {
        if (!seq.empty())
        {
            roots.insert(seq[0]);
        }
    }

    std::cout << "Print root types: ";
    for (auto root: roots)
    {
        std::cout << root << ", ";
    }

    std::cout << std::endl;
}

void PrintOnlyTypes(const B3dTree& tree, std::set< std::vector<std::uint32_t>>& sequences)
{
    std::cout << "Processing " << tree.dir << "/" << tree.id <<  "..." << std::endl;
    for (auto node : tree.rootNodes)
    {
        PrintOnlyTypes(node, {node->GetType()}, sequences);
    }
}

void PrintOnlyTypes(bool transform, bool optimize)
{
    std::set< std::vector<std::uint32_t>> sequences;

    B3dForest forest = ReadB3d(SinglePlayerRegistry);

    if (transform)
    {
        transformation::Transform(forest);
    }
    if (optimize)
    {
        transformation::Optimize(forest);
    }


    //PrintOnlyTypes(*forest.common, sequences);
    for (const auto& tree : forest.forest)
    {
        PrintOnlyTypes(*tree, sequences);
    }

    PrintOnlyTypes(*forest.common, sequences);
    PrintOnlyTypes(*forest.trucks, sequences);

    PrintSequences(sequences);

    PrintFirstElemsOfSequences(sequences);
}


} // namespace b3d
} // namespace data
} // namespace resource
} // namespace d2_hack








int main(int argc, char* argv[])
{
    if (argc < 3)
    {
        std::cerr << "usage: " << argv[0] << " <dir> <id> [--skip_vector_data]" << std::endl;
        return 1;
    }

    std::string dir = argv[1];
    std::string id = argv[2];

    bool printVectorData = true;
    bool printBoundingSphere = true;
    bool printFaceInfo = true;
    bool printMeshInfo = true;
    bool optimize = true;
    bool transform = true;
    bool printOnlyTypes = false;
    bool printTrucks = false;

    for (int i = 3; i != argc; ++i)
    {
        if (argv[i] == std::string("--skip_vector_data"))
        {
            printVectorData = false;
        }
        else if (argv[i] == std::string("--skip_bounding_sphere"))
        {
            printBoundingSphere = false;
        }
        else if (argv[i] == std::string("--skip_face_info"))
        {
            printFaceInfo = false;
        }
        else if (argv[i] == std::string("--skip_mesh_info"))
        {
            printMeshInfo = false;
        }
        else if (argv[i] == std::string("--skip_optimization"))
        {
            optimize = false;
        }
        else if (argv[i] == std::string("--skip_transformation"))
        {
            transform = false;
        }
        else if (argv[i] == std::string("--print_only_types"))
        {
            printOnlyTypes = true;
        }
        else if (argv[i] == std::string("--print_trucks"))
        {
            printTrucks = true;
        }
        else
        {
            std::cerr << "Unknown argument: " << argv[i] << std::endl;
            return 1;
        }
    }
    
    using namespace d2_hack::resource::data::b3d;

    try
    {
        Ogre::LogManager logMgr;
        logMgr.createLog("default", true, false, true);

        if (!printOnlyTypes)
        {
            B3dRegistry registry
            {
                D2_ROOT_DIR,
                "",
                {
                }
            };

            if (!printTrucks)
            {
                registry.dir = dir;
                registry.entries.push_back(id);
            }

            B3dForest forest = ReadB3d(registry);

            if (transform)
            {
                transformation::Transform(forest);
            }
            if (optimize)
            {
                transformation::Optimize(forest);
            }

            if (!printTrucks)
            {
                for (const auto& tree : forest.forest)
                {
                    TracingVisitor visitor{ printBoundingSphere, true, printVectorData, printFaceInfo, printMeshInfo };
                    VisitTree(*tree, visitor);
                }
            }
            else
            {
                TracingVisitor visitor{ printBoundingSphere, true, printVectorData, printFaceInfo, printMeshInfo };
                VisitTree(*forest.trucks, visitor);
            }
        }
        else
        {
            PrintOnlyTypes(transform, optimize);
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << "fail: " << e.what() << std::endl;
    }

    return 0;
}
