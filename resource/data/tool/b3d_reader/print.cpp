#include "print.h"

#include <iostream>
#include <fstream>
#include <filesystem>

#include <d2_hack/resource/data/b3d_visitor.h>

#include <d2_hack/common/utils.h>

#include "options.h"

namespace d2_hack
{
namespace resource
{
namespace data
{
namespace b3d
{

using namespace common;

class TracingVisitor : public NodeVisitorInterface
{
public:
    TracingVisitor(bool printBoundingSphere,
                   bool newLineForVectorData,
                   bool printVectorData,
                   bool printFaceInfo,
                   bool printMeshInfo,
                   bool printOnlyNames,
                   bool print_resource_name_as_hex,
                   std::set<std::uint32_t>&& blockTypesToPrint,
                   std::ostream& stream)
        : m_printBoundingSphere(printBoundingSphere)
        , m_newLineForVectorData(newLineForVectorData)
        , m_printVectorData(printVectorData)
        , m_printFaceInfo(printFaceInfo)
        , m_printMeshInfo(printMeshInfo)
        , m_printOnlyNames(printOnlyNames)
        , m_print_resource_name_as_hex(print_resource_name_as_hex)
        , m_blockTypesToPrint(std::move(blockTypesToPrint))
        , m_stream(stream)
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
        return std::format("{}: ", BlockType::Name);
    }

    template <typename NodeType>
    void ProcessBlock(const NodeType& node)
    {
        GetStream() << GetBlockNamePrefix(node.GetBlockData()) << ToString(node.GetName()) << std::endl;
        GetStream() << "{" << std::endl;

        if (ShouldPrintBlockContent(node.GetType()))
        {
            GetStream(1) << "boundingSphere: " << ToString(node.GetBoundingSphere()) << "," << std::endl;

            PrintData(node.GetBlockData(), "", 1);
        }
    }

    virtual VisitResult Visit(NodeHierarchyBreaker& node, VisitMode /* visitMode */) override
    {
        ProcessBlock(node);

        return VisitResult::Continue;
    }

    virtual VisitResult Visit(NodeEventEntry& node, VisitMode /* visitMode */) override
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
    const bool m_printBoundingSphere;
    const bool m_newLineForVectorData;
    const bool m_printVectorData;
    const bool m_printFaceInfo;
    const bool m_printMeshInfo;
    const bool m_printOnlyNames;
    const bool m_print_resource_name_as_hex;
    const std::set<std::uint32_t> m_blockTypesToPrint;
    std::ostream& m_stream;

    size_t m_offset = 0;

    bool ShouldPrintBlockContent(std::uint32_t blockType) const
    {
        if (m_printOnlyNames)
        {
            return false;
        }

        if (!m_blockTypesToPrint.empty())
        {
            const auto pos = m_blockTypesToPrint.find(blockType);
            if (pos == m_blockTypesToPrint.end())
            {
                return false;
            }
        }

        return true;
    }

    std::string GetOffsetString(int adjustOffset = 0) const
    {
        std::string res;
        res.resize((m_offset + adjustOffset) * 4, ' ');

        return res;
    }

    std::ostream& GetStream(int adjustOffset = 0)
    {
        m_stream << GetOffsetString(adjustOffset);
        return m_stream;
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
        PrintData(meshInfo.indices, "indices", adjustOffset + 1, m_printMeshInfo);
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

    void PrintData(const block_data::EventEntry& /* block */, const char* /* name */, int /* adjustOffset */)
    {
    }

    void PrintData(const block_data::Empty0& block, const char* /* name */, int adjustOffset)
    {
        GetStream(adjustOffset) << "emptyData0: " << ToString(std::begin(block.emptyData0), std::end(block.emptyData0)) << "," << std::endl;
        GetStream(adjustOffset) << "unknown: " << block.unknown << "," << std::endl;
    }

    void PrintData(const block_data::SomeUintsWithBuffer& data, const char* name, int adjustOffset)
    {
        GetStream(adjustOffset) << "SomeUintsWithBuffer(" << name << ")" << std::endl;
        GetStream(adjustOffset) << "{" << std::endl;

        GetStream(adjustOffset + 1) << "type: " << ToString(data.type) << "," << std::endl;
        GetStream(adjustOffset + 1) << "unknown0: " << ToString(data.unknown0) << "," << std::endl;

        PrintData(data.unknown1, "unknown1", adjustOffset + 1, m_printVectorData);
        GetStream(adjustOffset) << "}" << std::endl;
    }

    void PrintData(const block_data::SimpleObjectConnector1& block, const char* /* name */, int adjustOffset)
    {
        GetStream(adjustOffset) << "space: " << ToString(block.space) << "," << std::endl;
        GetStream(adjustOffset) << "object: " << ToString(block.object) << std::endl;
    }

    void PrintData(const block_data::GroupUnknown2& block, const char* /* name */, int adjustOffset)
    {
        GetStream(adjustOffset) << "unknown0: " << ToString(block.unknown0) << std::endl;
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
        GetStream(adjustOffset) << "lodCenter: " << ToString(block.lodCenter) << "," << std::endl;
        GetStream(adjustOffset) << "distanceToCamera: " << block.distanceToCamera << std::endl;
    }

    void PrintData(const block_data::GroupUnknown12& block, const char* /* name */, int adjustOffset)
    {
        GetStream(adjustOffset) << "unknown0: " << ToString(block.unknown0) << "," << std::endl;
        PrintData(block.unknown1, "unknown1", adjustOffset);
    }

    void PrintData(const block_data::SimpleTrigger13& block, const char* /* name */, int adjustOffset)
    {
        PrintData(block.unknown0, "unknown0", adjustOffset);
    }

    void PrintData(const block_data::SimpleUnknown14& block, const char* /* name */, int adjustOffset)
    {
        GetStream(adjustOffset) << "unknown0: " << ToString(block.unknown0) << "," << std::endl;
        PrintData(block.unknown1, "unknown1", adjustOffset);
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
        PrintData(block.unknown0, "unknown0", adjustOffset);
        PrintData(block.maybeVertices, "maybeVertices", adjustOffset);
    }

    void PrintData(const block_data::GroupObjects21& block, const char* /* name */, int adjustOffset)
    {
        GetStream(adjustOffset) << "count: " << block.count << "," << std::endl;
        GetStream(adjustOffset) << "defaultValue: " << block.defaultValue << std::endl;
    }

    void PrintData(const block_data::SimpleVolumeCollision23& block, const char* /* name */, int adjustOffset)
    {
        PrintData(block.unknown0, "unknown0", adjustOffset);
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
        GetStream(adjustOffset) << "soundname: \"" << common::ResourceNameToStringView(block.soundName) << "\"," << std::endl;
        GetStream(adjustOffset) << "unknown3" << block.unknown3 << "\"," << std::endl;
        GetStream(adjustOffset) << "unknown4" << block.unknown4 << "\"," << std::endl;
        GetStream(adjustOffset) << "unknown5: {"
            << block.unknown5[0] << ", "
            << block.unknown5[1] << ", "
            << block.unknown5[2] << ", "
            << block.unknown5[3] << ", "
            << block.unknown5[4] << "}" << std::endl;
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
        GetStream(adjustOffset) << "count: " << ToString(block.count) << "," << std::endl;
        GetStream(adjustOffset) << "unknown0: " << ToString(block.unknown0) << "," << std::endl;
        GetStream(adjustOffset) << "unknown1: " << ToString(block.unknown1) << "," << std::endl;
        PrintData(block.unknown2, "unknown2", adjustOffset);
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
        GetStream(adjustOffset) << "unknown3: " << ToString(block.unknown3) << "," << std::endl;
        GetStream(adjustOffset) << "color: {"
            << block.color[0] << ", "
            << block.color[1] << ", "
            << block.color[2] << ", "
            << block.color[3] << ", "
            << block.color[4] << ", "
            << block.color[5] << ", "
            << block.color[6] << ", "
            << block.color[7] << ", "
            << block.color[8] << "}" << std::endl;
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
        PrintData(block.unknown0, "unknown0", adjustOffset);
    }

    std::string ToString(const std::string_view& data)
    {
        return std::format("\"{}\"", data);
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
        std::string res = std::format("\"{}\"", common::ResourceNameToStringView(resourceName));

        if (m_print_resource_name_as_hex)
        {
            res += ", ";
            res += ToString(resourceName.data(), resourceName.data() + resourceName.size());
        }

        return res;
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


void VisitNode(const B3dNodePtr& node, TracingVisitor& visitor, int level)
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
            VisitNode(std::static_pointer_cast<B3dNode>(child), visitor, level + 2);
        }

        visitor.SetLevel(level);
        visitor.EndNested();
    }
    visitor.EndNode();
}

void VisitTree(const B3dTree& tree, TracingVisitor& visitor)
{
    std::cout << "Processing " << tree.dir << "/" << tree.id << " ..." << std::endl;
    visitor.SetLevel(0);
    visitor.Visit(tree.materials);
    visitor.Visit(tree.transformations);
    visitor.Visit(tree.volumeCollisions);

    for (auto node : tree.rootNodes)
    {
        VisitNode(node, visitor, 0);
    }
}

} // namespace b3d
} // namespace data
} // namespace resource
} // namespace d2_hack


using namespace d2_hack::resource::data::b3d;


namespace options
{
namespace printing
{
static const char skip_vector_data[] = "skip_vector_data";
static const char skip_bounding_sphere[] = "skip_bounding_sphere";
static const char skip_face_info[] = "skip_face_info";
static const char skip_mesh_info[] = "skip_mesh_info";
static const char print_only_names[] = "print_only_names";
static const char block_types_to_print[] = "block_types_to_print";
static const char print_resource_name_as_hex[] = "print_resource_name_as_hex";

} // namespace printing
} // namespace options



boost::program_options::options_description get_print_options()
{
    boost::program_options::options_description print_options("Printing options");
    print_options.add_options()
        (options::printing::skip_bounding_sphere, "Skip bounding sphere")
        (options::printing::skip_vector_data, "Skip vector data")
        (options::printing::skip_face_info, "Skip face info")
        (options::printing::skip_mesh_info, "Skip mesh info")
        (options::printing::print_only_names, "Print only names")
        (options::printing::print_resource_name_as_hex, "Print resource name as hex")
        (options::printing::block_types_to_print, boost::program_options::value<std::vector<std::uint32_t>>()->multitoken(), "Print content only for given block types");

    return print_options;
}


static std::filesystem::path ConstructOutputPathName(const B3dTree& tree, const boost::program_options::variables_map& vm)
{
    std::filesystem::path dir = vm[options::generic::output_dir].as<std::string>() + "/" + tree.dir + "/" + tree.id;
    std::filesystem::create_directories(dir);

    using namespace options::generic;
    using namespace options::printing;
    
    std::string filename = tree.id;

    if (vm.contains(skip_transformation))
    {
        filename += "_notf";
    }
    if (vm.contains(skip_optimization))
    {
        filename += "_noopt";
    }
    if (vm.contains(skip_bounding_sphere))
    {
        filename += "_nobs";
    }
    if (vm.contains(skip_vector_data))
    {
        filename += "_novd";
    }
    if (vm.contains(skip_face_info))
    {
        filename += "_nofi";
    }
    if (vm.contains(skip_mesh_info))
    {
        filename += "_nomi";
    }
    if (vm.contains(print_only_names))
    {
        filename += "_onlynames";
    }

    return dir / (filename + ".txt");
}


static void PrintTree(const B3dTree& tree, const boost::program_options::variables_map& vm)
{
    const bool printBoundingSphere = (vm.count(options::printing::skip_bounding_sphere) > 0);
    const bool printVectorData = (vm.count(options::printing::skip_vector_data) == 0);
    const bool printFaceInfo = (vm.count(options::printing::skip_face_info) == 0);
    const bool printMeshInfo = (vm.count(options::printing::skip_mesh_info) == 0);
    const bool printOnlyNames = (vm.count(options::printing::print_only_names) > 0);
    const bool printResourceNameAsHex = (vm.count(options::printing::print_resource_name_as_hex) > 0);

    std::set<std::uint32_t> blockTypesToPrint;
    if (vm.count(options::printing::block_types_to_print) > 0)
    {
        const auto& tmp = vm[options::printing::block_types_to_print].as<std::vector<std::uint32_t>>();
        blockTypesToPrint.insert(tmp.begin(), tmp.end());
    }

    std::filesystem::path outputFile = ConstructOutputPathName(tree, vm);
    std::ofstream outputStream{ outputFile };

    TracingVisitor visitor{ printBoundingSphere, true, printVectorData, printFaceInfo, printMeshInfo, printOnlyNames, printResourceNameAsHex, std::move(blockTypesToPrint), outputStream };

    VisitTree(tree, visitor);
}

int print(const B3dForest& forest, const boost::program_options::variables_map& vm)
{
    const bool withTrucks = vm.contains(options::generic::with_trucks);
    const bool withCommon = vm.contains(options::generic::with_common);


    if (withTrucks)
    {
        PrintTree(*forest.trucks, vm);
    }
    if (withCommon)
    {
        PrintTree(*forest.common, vm);
    }

    for (const auto& tree : forest.forest)
    {
        PrintTree(*tree, vm);
    }

    return 0;
}
