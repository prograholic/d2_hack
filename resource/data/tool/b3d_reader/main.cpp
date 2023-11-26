#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <stdexcept>

#include <OgreLogManager.h>

#include <d2_hack/resource/data/b3d_reader.h>
#include <d2_hack/resource/data/b3d_tree.h>
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
        PrintVectorData(materials, "materials");
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
        GetStream() << "}" << std::endl;
    }

    template <typename BlockType>
    std::string GetBlockNamePrefix(const BlockType& /* block */)
    {
        return std::string(BlockType::Name) + ": ";
    }

    virtual void Visit(const std::string& name, block_data::Empty0& block, VisitMode /* visitMode */) override
    {
        GetStream() << GetBlockNamePrefix(block) << ToString(name) << std::endl;
        GetStream() << "{" << std::endl;
        GetStream(1) << "emptyData0: " << ToString(std::begin(block.emptyData0), std::end(block.emptyData0)) << "," << std::endl;
        GetStream(1) << "unknown: " << block.unknown << "," << std::endl;
        GetStream(1) << "emptyData1: " << ToString(std::begin(block.emptyData1), std::end(block.emptyData1)) << std::endl;
    }

    virtual void Visit(const std::string& name, block_data::GroupUnknown2& block, VisitMode /* visitMode */) override
    {
        GetStream() << GetBlockNamePrefix(block) << ToString(name) << std::endl;
        GetStream() << "{" << std::endl;
        GetStream(1) << "boundingSphere: " << ToString(block.boundingSphere) << "," << std::endl;
        GetStream(1) << "unknown0: " << block.unknown0 << "," << std::endl;
        GetStream(1) << "unknown1: " << block.unknown1 << "," << std::endl;
    }

    virtual void Visit(const std::string& name, block_data::GroupRoadInfraObjects4& block, VisitMode /* visitMode */) override
    {
        GetStream() << GetBlockNamePrefix(block) << ToString(name) << std::endl;
        GetStream() << "{" << std::endl;
        GetStream(1) << "boundingSphere: " << ToString(block.boundingSphere) << "," << std::endl;
        GetStream(1) << "name: " << ToString(block.name) << "," << std::endl;
        GetStream(1) << "data: " << ToString(block.data) << std::endl;
    }

    virtual void Visit(const std::string& name, block_data::GroupObjects5& block, VisitMode /* visitMode */) override
    {
        GetStream() << GetBlockNamePrefix(block) << ToString(name) << std::endl;
        GetStream() << "{" << std::endl;
        GetStream(1) << "boundingSphere: " << ToString(block.boundingSphere) << "," << std::endl;
        GetStream(1) << "name: " << ToString(block.name) << std::endl;
    }

    virtual void Visit(const std::string& name, block_data::GroupVertex7& block, VisitMode /* visitMode */) override
    {
        GetStream() << GetBlockNamePrefix(block) << ToString(name) << std::endl;
        GetStream() << "{" << std::endl;
        GetStream(1) << "boundingSphere: " << ToString(block.boundingSphere) << "," << std::endl;
        GetStream(1) << "name: " << ToString(block.name) << std::endl;
        PrintData(block.meshInfo, 1);
    }

    virtual void Visit(const std::string& name, block_data::SimpleFaces8& block, VisitMode /* visitMode */) override
    {
        GetStream() << GetBlockNamePrefix(block) << ToString(name) << std::endl;
        GetStream() << "{" << std::endl;
        GetStream(1) << "boundingSphere: " << ToString(block.boundingSphere) << std::endl;
        PrintVectorData(block.faces, "faces", 1, m_printFaceInfo);
    }

    virtual void Visit(const std::string& name, block_data::GroupTrigger9& block, VisitMode /* visitMode */) override
    {
        GetStream() << GetBlockNamePrefix(block) << ToString(name) << std::endl;
        GetStream() << "{" << std::endl;
        GetStream(1) << "boundingSphere: " << ToString(block.boundingSphere) << "," << std::endl;
        GetStream(1) << "unknown: " << block.unknown << "," << std::endl;
        GetStream(1) << "distanceToPlayer: " << block.distanceToPlayer << std::endl;
    }

    virtual void Visit(const std::string& name, block_data::GroupLodParameters10& block, VisitMode /* visitMode */) override
    {
        GetStream() << GetBlockNamePrefix(block) << ToString(name) << std::endl;
        GetStream() << "{" << std::endl;
        GetStream(1) << "boundingSphere: " << ToString(block.boundingSphere) << "," << std::endl;
        GetStream(1) << "unknown: " << ToString(block.unknown) << "," << std::endl;
        GetStream(1) << "distanceToPlayer: " << block.distanceToPlayer << std::endl;
    }

    virtual void Visit(const std::string& name, block_data::GroupUnknown12& block, VisitMode /* visitMode */) override
    {
        GetStream() << GetBlockNamePrefix(block) << ToString(name) << std::endl;
        GetStream() << "{" << std::endl;
        GetStream(1) << "boundingSphere: " << ToString(block.boundingSphere) << "," << std::endl;
        GetStream(1) << "unknown0: " << ToString(block.unknown0) << "," << std::endl;
        GetStream(1) << "unknown1: " << ToString(block.unknown1) << "," << std::endl;
        GetStream(1) << "unknown2: " << ToString(block.unknown2) << "," << std::endl;
        GetStream(1) << "unknown3: " << ToString(block.unknown3) << "," << std::endl;
        GetStream(1) << "unknown4: " << ToString(block.unknown4) << "," << std::endl;
        GetStream(1) << "unknown5: " << ToString(block.unknown5) << std::endl;
    }

    virtual void Visit(const std::string& name, block_data::SimpleTrigger13& block, VisitMode /* visitMode */) override
    {
        GetStream() << GetBlockNamePrefix(block) << ToString(name) << std::endl;
        GetStream() << "{" << std::endl;
        GetStream(1) << "boundingSphere: " << ToString(block.boundingSphere) << "," << std::endl;
        GetStream(1) << "unknown0: " << block.unknown0 << "," << std::endl;
        GetStream(1) << "unknown1: " << block.unknown1 << std::endl;
        PrintVectorData(block.unknown2, "unknown2", 1);
    }

    virtual void Visit(const std::string& name, block_data::SimpleUnknown14& block, VisitMode /* visitMode */) override
    {
        GetStream() << GetBlockNamePrefix(block) << ToString(name) << std::endl;
        GetStream() << "{" << std::endl;
        GetStream(1) << "boundingSphere: " << ToString(block.boundingSphere) << "," << std::endl;
        GetStream(1) << "unknown0: " << block.unknown0 << "," << std::endl;
        GetStream(1) << "unknown1: " << block.unknown1 << "," << std::endl;
        GetStream(1) << "unknown2: " << block.unknown2 << "," << std::endl;
        GetStream(1) << "unknown3: " << block.unknown3 << "," << std::endl;
        GetStream(1) << "unknown4: " << block.unknown4 << "," << std::endl;
        GetStream(1) << "unknown5: " << block.unknown5 << "," << std::endl;
        GetStream(1) << "unknown6: " << block.unknown6 << std::endl;
    }

    virtual void Visit(const std::string& name, block_data::SimpleObjectConnector18& block, VisitMode /* visitMode */) override
    {
        GetStream() << GetBlockNamePrefix(block) << ToString(name) << std::endl;
        GetStream() << "{" << std::endl;
        GetStream(1) << "boundingSphere: " << ToString(block.boundingSphere) << "," << std::endl;
        GetStream(1) << "space: " << ToString(block.space) << "," << std::endl;
        GetStream(1) << "object: " << ToString(block.object) << std::endl;
    }

    virtual void Visit(const std::string& name, block_data::GroupObjects19& block, VisitMode /* visitMode */) override
    {
        GetStream() << GetBlockNamePrefix(block) << ToString(name) << std::endl;
        GetStream() << "{" << std::endl;
    }

    virtual void Visit(const std::string& name, block_data::SimpleFlatCollision20& block, VisitMode /* visitMode */) override
    {
        GetStream() << GetBlockNamePrefix(block) << ToString(name) << std::endl;
        GetStream() << "{" << std::endl;
        GetStream(1) << "boundingSphere: " << ToString(block.boundingSphere) << "," << std::endl;
        GetStream(1) << "unknown0: " << block.unknown0 << "," << std::endl;
        GetStream(1) << "unknown1: " << block.unknown1 << std::endl;
        PrintVectorData(block.unknown2, "unknown2", 1);
        PrintVectorData(block.unknown3, "unknown3", 1);
    }

    virtual void Visit(const std::string& name, block_data::GroupObjects21& block, VisitMode /* visitMode */) override
    {
        GetStream() << GetBlockNamePrefix(block) << ToString(name) << std::endl;
        GetStream() << "{" << std::endl;
        GetStream(1) << "boundingSphere: " << ToString(block.boundingSphere) << "," << std::endl;
        GetStream(1) << "count: " << block.count << "," << std::endl;
        GetStream(1) << "unknown: " << block.unknown << std::endl;
    }

    virtual void Visit(const std::string& name, block_data::SimpleVolumeCollision23& block, VisitMode /* visitMode */) override
    {
        GetStream() << GetBlockNamePrefix(block) << ToString(name) << std::endl;
        GetStream() << "{" << std::endl;
        GetStream(1) << "unknown0: " << block.unknown0 << "," << std::endl;
        GetStream(1) << "surfaceType: " << block.surfaceType << "," << std::endl;
        PrintVectorData(block.unknown1, "unknown1", 1);
        PrintVectorData(block.polygons, "polygons", 1);
    }

    virtual void Visit(const std::string& name, block_data::GroupTransformMatrix24& block, VisitMode /* visitMode */) override
    {
        GetStream() << GetBlockNamePrefix(block) << ToString(name) << std::endl;
        GetStream() << "{" << std::endl;
        GetStream(1) << "x: " << block.x << "," << std::endl;
        GetStream(1) << "y: " << block.y << "," << std::endl;
        GetStream(1) << "z: " << block.z << "," << std::endl;
        GetStream(1) << "position: " << block.position << "," << std::endl;
        GetStream(1) << "unknown: " << block.unknown << std::endl;
    }

    virtual void Visit(const std::string& name, block_data::SimpleUnknown25& block, VisitMode /* visitMode */) override
    {
        GetStream() << GetBlockNamePrefix(block) << ToString(name) << std::endl;
        GetStream() << "{" << std::endl;
        GetStream(1) << "unknown0: " << block.unknown0 << "," << std::endl;
        GetStream(1) << "unknown1: " << block.unknown1 << "," << std::endl;
        GetStream(1) << "unknown2: " << block.unknown2 << "," << std::endl;
        GetStream(1) << "soundname: " << common::ResourceNameToString(block.soundName) << "," << std::endl;
        GetStream(1) << "unknown3: {"
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

    virtual void Visit(const std::string& name, block_data::SimpleFaces28& block, VisitMode /* visitMode */) override
    {
        GetStream() << GetBlockNamePrefix(block) << ToString(name) << std::endl;
        GetStream() << "{" << std::endl;
        GetStream(1) << "boundingSphere: " << ToString(block.boundingSphere) << "," << std::endl;
        GetStream(1) << "unknown: " << ToString(block.unknown) << std::endl;
        PrintVectorData(block.faces, "faces", 1);
    }

    virtual void Visit(const std::string& name, block_data::GroupUnknown29& block, VisitMode /* visitMode */) override
    {
        GetStream() << GetBlockNamePrefix(block) << ToString(name) << std::endl;
        GetStream() << "{" << std::endl;
        GetStream(1) << "boundingSphere: " << ToString(block.boundingSphere) << "," << std::endl;
        GetStream(1) << "type: " << ToString(block.type) << "," << std::endl;
        GetStream(1) << "unknown0: " << ToString(block.unknown0) << "," << std::endl;
        GetStream(1) << "unknown1: "
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

    virtual void Visit(const std::string& name, block_data::SimplePortal30& block, VisitMode /* visitMode */) override
    {
        GetStream() << GetBlockNamePrefix(block) << ToString(name) << std::endl;
        GetStream() << "{" << std::endl;
        GetStream(1) << "boundingSphere: " << ToString(block.boundingSphere) << "," << std::endl;
        GetStream(1) << "connectedRoom: " << ToString(block.connectedRoom) << "," << std::endl;
        GetStream(1) << "leftDown: " << ToString(block.leftDown) << "," << std::endl;
        GetStream(1) << "upRight: " << ToString(block.upRight) << std::endl;
    }

    virtual void Visit(const std::string& name, block_data::GroupLightingObjects33& block, VisitMode /* visitMode */) override
    {
        GetStream() << GetBlockNamePrefix(block) << ToString(name) << std::endl;
        GetStream() << "{" << std::endl;
        GetStream(1) << "boundingSphere: " << ToString(block.boundingSphere) << "," << std::endl;
        GetStream(1) << "unknown0: " << block.unknown0 << "," << std::endl;
        GetStream(1) << "unknown1: " << block.unknown1 << "," << std::endl;
        GetStream(1) << "unknown2: " << block.unknown2 << "," << std::endl;
        GetStream(1) << "position: " << ToString(block.position) << "," << std::endl;
        GetStream(1) << "color: {"
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

    virtual void Visit(const std::string& name, block_data::SimpleUnknown34& block, VisitMode /* visitMode */) override
    {
        GetStream() << GetBlockNamePrefix(block) << ToString(name) << std::endl;
        GetStream() << "{" << std::endl;
        GetStream(1) << "boundingSphere: " << ToString(block.boundingSphere) << "," << std::endl;
        GetStream(1) << "unknown0: " << block.unknown0 << "," << std::endl;
        PrintVectorData(block.data, "data", 1);
    }

    virtual void Visit(const std::string& name, block_data::SimpleFaces35& block, VisitMode /* visitMode */) override
    {
        GetStream() << GetBlockNamePrefix(block) << ToString(name) << std::endl;
        GetStream() << "{" << std::endl;
        GetStream(1) << "boundingSphere: " << ToString(block.boundingSphere) << "," << std::endl;
        GetStream(1) << "type: " << block.type << "," << std::endl;
        GetStream(1) << "materialIndex: " << block.materialIndex << std::endl;
        PrintVectorData(block.faces, "faces", 1, m_printFaceInfo);
    }

    virtual void Visit(const std::string& name, block_data::GroupVertexData37& block, VisitMode /* visitMode */) override
    {
        GetStream() << GetBlockNamePrefix(block) << ToString(name) << std::endl;
        GetStream() << "{" << std::endl;
        GetStream(1) << "boundingSphere: " << ToString(block.boundingSphere) << "," << std::endl;
        GetStream(1) << "name: " << ToString(block.name) << "," << std::endl;
        GetStream(1) << "type: " << block.type << std::endl;
        PrintData(block.meshInfo, 1);
        PrintVectorData(block.unknown258Or515, "unknown258Or515", 1);
        PrintVectorData(block.unknown514, "unknown514", 1);
    }

    virtual void Visit(const std::string& name, block_data::SimpleGeneratedObjects40& block, VisitMode /* visitMode */) override
    {
        GetStream() << GetBlockNamePrefix(block) << ToString(name) << std::endl;
        GetStream() << "{" << std::endl;
        GetStream(1) << "boundingSphere: " << ToString(block.boundingSphere) << "," << std::endl;
        GetStream(1) << "empty: " << ToString(block.empty) << "," << std::endl;
        GetStream(1) << "name: " << ToString(block.name) << "," << std::endl;
        GetStream(1) << "type: " << block.type << "," << std::endl;
        GetStream(1) << "unknown0: " << block.unknown0 << std::endl;
        PrintVectorData(block.unknown1, "unknown1", 1);
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

    void PrintData(const common::ResourceName& name, int adjustOffset)
    {
        GetStream(adjustOffset) << ToString(name) << std::endl;
    }

    void PrintData(const common::SimpleMeshInfo& meshInfo, int adjustOffset)
    {
        GetStream(adjustOffset) << "SimpleMeshInfo" << std::endl;
        GetStream(adjustOffset) << "{" << std::endl;

        PrintVectorData(meshInfo.positions, "positions", adjustOffset + 1, m_printMeshInfo);
        PrintVectorData(meshInfo.texCoords, "texCoords", adjustOffset + 1, m_printMeshInfo);
        PrintVectorData(meshInfo.normals, "normals", adjustOffset + 1, m_printMeshInfo);
        GetStream(adjustOffset) << "}" << std::endl;
    }

    void PrintData(const block_data::Face8& data, int adjustOffset)
    {
        GetStream(adjustOffset) << "Face8" << std::endl;
        GetStream(adjustOffset) << "{" << std::endl;

        GetStream(adjustOffset + 1) << "type: " << data.type << std::endl;
        GetStream(adjustOffset + 1) << "unknown0: " << data.unknown0 << std::endl;
        GetStream(adjustOffset + 1) << "unknown1: " << data.unknown1 << std::endl;
        GetStream(adjustOffset + 1) << "materialIndex: " << data.materialIndex << std::endl;
        PrintData(data.meshInfo, adjustOffset + 1);

        GetStream(adjustOffset) << "}" << std::endl;
    }

    void PrintData(Ogre::Real data, int adjustOffset)
    {
        GetStream(adjustOffset) << data << std::endl;
    }

    void PrintData(std::uint32_t data, int adjustOffset)
    {
        GetStream(adjustOffset) << data << std::endl;
    }

    void PrintData(const Ogre::Vector3& data, int adjustOffset)
    {
        GetStream(adjustOffset) << data << std::endl;
    }

    void PrintData(const Ogre::Vector2& data, int adjustOffset)
    {
        GetStream(adjustOffset) << data << std::endl;
    }

    void PrintData(const block_data::Face28& data, int adjustOffset)
    {
        GetStream(adjustOffset) << "Face28" << std::endl;
        GetStream(adjustOffset) << "{" << std::endl;
        if (m_printFaceInfo)
        {
            GetStream(adjustOffset + 1) << "type: " << data.type << std::endl;
            GetStream(adjustOffset + 1) << "unknown0: " << data.unknown0 << std::endl;
            GetStream(adjustOffset + 1) << "unknown1: " << data.unknown1 << std::endl;
            GetStream(adjustOffset + 1) << "materialIndex: " << data.materialIndex << std::endl;
            PrintData(data.meshInfo, adjustOffset + 1);
            PrintVectorData(data.unknown, "unknown", adjustOffset + 1);
        }
        else
        {
            GetStream(adjustOffset + 1) << "//..." << std::endl;
        }
        GetStream(adjustOffset) << "}" << std::endl;
    }

    void PrintData(const block_data::Face28::Unknown& data, int adjustOffset)
    {
        if (m_printFaceInfo)
        {
            GetStream(adjustOffset) << "Face28::Unknown" << std::endl;
            GetStream(adjustOffset) << "{" << std::endl;
            GetStream(adjustOffset + 1) << "unknown: " << data.unknown << std::endl;
            GetStream(adjustOffset) << "}" << std::endl;
        }
    }
    void PrintData(const block_data::Face35& data, int adjustOffset)
    {
        GetStream(adjustOffset) << "Face35" << std::endl;
        GetStream(adjustOffset) << "{" << std::endl;

        if (m_printFaceInfo)
        {
            GetStream(adjustOffset + 1) << "type: " << data.type << std::endl;
            GetStream(adjustOffset + 1) << "unknown0: " << data.unknown0 << std::endl;
            GetStream(adjustOffset + 1) << "unknown1: " << data.unknown1 << std::endl;
            GetStream(adjustOffset + 1) << "materialIndex: " << data.materialIndex << std::endl;
            PrintData(data.meshInfo, adjustOffset + 1);
            PrintVectorData(data.unknown49, "unknown49", adjustOffset + 1);
        }
        else
        {
            GetStream(adjustOffset + 1) << "//..." << std::endl;
        }
        GetStream(adjustOffset) << "}" << std::endl;
    }

    void PrintData(const block_data::GroupVertexData37::Unknown514& data, int adjustOffset)
    {
        GetStream(adjustOffset) << "GroupVertexData37::Unknown514" << std::endl;
        GetStream(adjustOffset) << "{" << std::endl;
        GetStream(adjustOffset + 1) << "unknown: " << data.unknown << std::endl;
        GetStream(adjustOffset) << "}" << std::endl;
    }

    void PrintData(const block_data::GroupVertexData37::Unknown258Or515& data, int adjustOffset)
    {
        GetStream(adjustOffset) << "GroupVertexData37::Unknown258Or515" << std::endl;
        GetStream(adjustOffset) << "{" << std::endl;
        GetStream(adjustOffset + 1) << "unknown: " << ToString(data.unknown) << std::endl;
        GetStream(adjustOffset) << "}" << std::endl;
    }

    void PrintData(const block_data::Face35::Unknown49& data, int adjustOffset)
    {
        GetStream(adjustOffset) << "Face35::Unknown49" << std::endl;
        GetStream(adjustOffset) << "{" << std::endl;
        GetStream(adjustOffset + 1) << "unknown: " << ToString(data.unknown) << "," << std::endl;
        GetStream(adjustOffset) << "}" << std::endl;
    }

    void PrintData(const block_data::SimpleUnknown34::Unknown& data, int adjustOffset)
    {
        GetStream(adjustOffset) << "SimpleUnknown34::Unknown" << std::endl;
        GetStream(adjustOffset) << "{" << std::endl;
        GetStream(adjustOffset + 1) << "unknown0: " << ToString(data.unknown0) << "," << std::endl;
        GetStream(adjustOffset + 1) << "unknown1: " << ToString(data.unknown1) << "," << std::endl;
        GetStream(adjustOffset + 1) << "unknown2: " << ToString(data.unknown2) << "," << std::endl;
        GetStream(adjustOffset + 1) << "unknown3: " << ToString(data.unknown3) << "," << std::endl;
        GetStream(adjustOffset) << "}" << std::endl;
    }

    void PrintData(const common::PositionList& data, int adjustOffset)
    {
        PrintVectorData(data, "data", adjustOffset + 1);
    }

    void PrintData(const block_data::Face8::Unknown177& data, int adjustOffset)
    {
        GetStream(adjustOffset) << "Face8::Unknown177" << std::endl;
        GetStream(adjustOffset) << "{" << std::endl;
        GetStream(adjustOffset + 1) << "unknown: " << ToString(data.unknown) << "," << std::endl;
        GetStream(adjustOffset) << "}" << std::endl;
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
    void PrintVectorData(const std::vector<T, A>& data, const char* name, int adjustOffset = 0, bool printVectorData = true)
    {
        GetStream(adjustOffset) << name << "(" << data.size() << ")" << std::endl;
        GetStream(adjustOffset) << "{" << std::endl;

        if (m_printVectorData && printVectorData)
        {
            for (const auto& item : data)
            {
                PrintData(item, adjustOffset + 1);
            }
        }
        else
        {
            GetStream(adjustOffset + 1) << "//..." << std::endl;
        }
        GetStream(adjustOffset) << "}" << std::endl;
    }
};


void VisitNode(const NodePtr& node, TracingVisitor& visitor, int level)
{
    visitor.SetLevel(level);
    node->Visit(visitor, VisitMode::PreOrder);
    visitor.BeginNested();

    const auto& childNodes = node->GetChildNodeList();
    for (auto child : childNodes)
    {
        VisitNode(child, visitor, level + 2);
    }

    visitor.SetLevel(level);
    visitor.EndNested();
}

void VisitTree(const B3dTree& tree, TracingVisitor& visitor)
{
    visitor.SetLevel(0);
    visitor.Visit(tree.materials);

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

void PrintOnlyTypes(std::set< std::vector<std::uint32_t>>& sequences, const B3dTree& tree)
{
    for (auto node : tree.rootNodes)
    {
        PrintOnlyTypes(node, {node->GetType()}, sequences);
    }
}

void PrintOnlyTypes(const std::string& baseDir, const std::string& subDir, const std::string& prefix, std::set< std::vector<std::uint32_t>>& sequences)
{
    std::string fileName = baseDir + "/" + subDir + "/" + prefix + ".b3d";
    std::cout << "processing " << prefix << " ..." << std::endl;

    std::ifstream inputFile(fileName.c_str(), std::ios_base::binary);
    if (!inputFile)
    {
        throw std::runtime_error("failed to open file [" + fileName + "]");
    }

    B3dReader reader;

    Ogre::FileStreamDataStream dataStream(&inputFile, false);

    B3dTree tree = reader.Read(dataStream);

    optimization::Optimize(tree);

    PrintOnlyTypes(sequences, tree);
}


void PrintOnlyTypes(const std::string& dir)
{
    std::set< std::vector<std::uint32_t>> sequences;

    PrintOnlyTypes(dir, "ENV", "aa", sequences);
    PrintOnlyTypes(dir, "ENV", "ab", sequences);
    PrintOnlyTypes(dir, "ENV", "ac", sequences);
    PrintOnlyTypes(dir, "ENV", "ad", sequences);
    PrintOnlyTypes(dir, "ENV", "ae", sequences);
    PrintOnlyTypes(dir, "ENV", "af", sequences);
    PrintOnlyTypes(dir, "ENV", "ag", sequences);
    PrintOnlyTypes(dir, "ENV", "ah", sequences);
    PrintOnlyTypes(dir, "ENV", "aj", sequences);
    PrintOnlyTypes(dir, "ENV", "ak", sequences);
    PrintOnlyTypes(dir, "ENV", "al", sequences);
    PrintOnlyTypes(dir, "ENV", "am", sequences);
    PrintOnlyTypes(dir, "ENV", "ap", sequences);
    PrintOnlyTypes(dir, "ENV", "aq", sequences);
    PrintOnlyTypes(dir, "ENV", "ar", sequences);
    PrintOnlyTypes(dir, "ENV", "as", sequences);
    PrintOnlyTypes(dir, "ENV", "at", sequences);
    PrintOnlyTypes(dir, "ENV", "au", sequences);
    PrintOnlyTypes(dir, "ENV", "av", sequences);
    PrintOnlyTypes(dir, "ENV", "aw", sequences);
    PrintOnlyTypes(dir, "ENV", "ax", sequences);
    PrintOnlyTypes(dir, "ENV", "ba", sequences);
    PrintOnlyTypes(dir, "ENV", "bb", sequences);
    PrintOnlyTypes(dir, "ENV", "bc", sequences);
    PrintOnlyTypes(dir, "ENV", "bd", sequences);
    PrintOnlyTypes(dir, "ENV", "be", sequences);
    PrintOnlyTypes(dir, "ENV", "bf", sequences);
    PrintOnlyTypes(dir, "ENV", "bg", sequences);
    PrintOnlyTypes(dir, "ENV", "ca", sequences);
    PrintOnlyTypes(dir, "ENV", "cb", sequences);
    PrintOnlyTypes(dir, "ENV", "cc", sequences);
    PrintOnlyTypes(dir, "ENV", "ce", sequences);
    PrintOnlyTypes(dir, "ENV", "cf", sequences);
    PrintOnlyTypes(dir, "ENV", "ch", sequences);
    PrintOnlyTypes(dir, "ENV", "da", sequences);
    PrintOnlyTypes(dir, "ENV", "db", sequences);
    PrintOnlyTypes(dir, "ENV", "dc", sequences);
    PrintOnlyTypes(dir, "ENV", "dq", sequences);
    PrintOnlyTypes(dir, "ENV", "dr", sequences);

    PrintOnlyTypes(dir, "COMMON", "trucks", sequences);


    PrintSequences(sequences);
}





} // namespace b3d
} // namespace data
} // namespace resource
} // namespace d2_hack








int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cerr << "usage: " << argv[0] << " <input_file> [--skip_vector_data]" << std::endl;
        return 1;
    }

    std::string fileName = argv[1];

    bool printVectorData = true;
    bool printBoundingSphere = true;
    bool printFaceInfo = true;
    bool printMeshInfo = true;
    bool optimize = true;
    bool printOnlyTypes = false;
    for (int i = 2; i != argc; ++i)
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
        else if (argv[i] == std::string("--print_only_types"))
        {
            printOnlyTypes = true;
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
            std::ifstream inputFile(fileName.c_str(), std::ios_base::binary);
            if (!inputFile)
            {
                std::cerr << "failed to open file [" << fileName << "]" << std::endl;
                return 2;
            }

            B3dReader reader;

            Ogre::FileStreamDataStream dataStream(&inputFile, false);

            B3dTree tree = reader.Read(dataStream);
            if (optimize)
            {
                optimization::Optimize(tree);
            }

            TracingVisitor visitor{printBoundingSphere, true, printVectorData, printFaceInfo, printMeshInfo};
            VisitTree(tree, visitor);
        }
        else
        {
            PrintOnlyTypes(fileName);
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << "fail: " << e.what() << std::endl;
    }

    return 0;
}
