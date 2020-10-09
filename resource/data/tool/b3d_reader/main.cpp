#include <iostream>
#include <iomanip>
#include <fstream>
#include <strstream>
#include <string>
#include <stdexcept>

#include <d2_hack/resource/data/b3d.h>
#include <d2_hack/resource/data/b3d_reader.h>

namespace d2_hack
{
namespace resource
{
namespace data
{
namespace b3d
{

class TracingListener : public B3dListenerInterface
{
public:
    TracingListener(bool printBoundingSphere, bool newLineForVectorData, bool printVectorData, bool printMeshInfo)
        : m_printBoundingSphere(printBoundingSphere)
        , m_newLineForVectorData(newLineForVectorData)
        , m_printVectorData(printVectorData)
        , m_printMeshInfo(printMeshInfo)
    {
    }

    virtual void OnBlockBegin(const block_data::BlockHeader& blockHeader) override
    {
        m_offset += 1;
        GetStream() << "name: " << Print(blockHeader.name) << std::endl;
        GetStream() << "type: " << blockHeader.type << std::endl;
    }

    virtual void OnBlockEnd(const block_data::BlockHeader& /* blockHeader */) override
    {
        m_offset -= 1;
        GetStream() << std::endl;
    }

    virtual void OnNestedBlockBegin(std::uint32_t /* nestedBlockNumber */) override
    {
        GetStream() << "nested: {" << std::endl;
    }

    virtual void OnNestedBlockEnd(std::uint32_t /* nestedBlockNumber */) override
    {
        GetStream() << "} //nested" << std::endl;
    }

    virtual void OnMaterials(Materials&& materials) override
    {
        PrintVectorData(materials, "materials");
    }

    virtual void OnBlock(const block_data::Empty0& block) override
    {
        GetStream() << "Empty0 {"
            "emptyData0: " << Print(std::begin(block.emptyData0), std::end(block.emptyData0)) << ", "
            "unknown: " << block.unknown << ", "
            "emptyData1: " << Print(std::begin(block.emptyData1), std::end(block.emptyData1)) <<
            "}" << std::endl;
    }

    virtual void OnBlock(const block_data::GroupRoadInfraObjects4& block) override
    {
        GetStream() << "GroupRoadInfraObjects4 {"
            "boundingSphere: " << Print(block.boundingSphere) << ", "
            "name: " << Print(block.name) << ", "
            "data: " << Print(block.data) << "}" << std::endl;
    }

    virtual void OnBlock(const block_data::GroupObjects5& block) override
    {
        GetStream() << "GroupObjects5 {"
            "boundingSphere: " << Print(block.boundingSphere) << ", "
            "name: " << Print(block.name) << "}" << std::endl;
    }

    virtual void OnBlock(const block_data::GroupVertex7& block) override
    {
        GetStream() << "GroupVertex7 {"
            "boundingSphere: " << Print(block.boundingSphere) << ", "
            "name: " << Print(block.name) << "}" << std::endl;
    }

    virtual void OnBlock(const block_data::SimpleFaces8& block) override
    {
        GetStream() << "SimpleFaces8 {"
            "boundingSphere: " << Print(block.boundingSphere) << "}" << std::endl;
    }

    virtual void OnBlock(const block_data::GroupTrigger9& block) override
    {
        GetStream() << "GroupTrigger9 {"
            "boundingSphere: " << Print(block.boundingSphere) << ", "
            "unknown: " << block.unknown << ", "
            "distanceToPlayer: " << block.distanceToPlayer << "}" << std::endl;
    }

    virtual void OnBlock(const block_data::GroupLodParameters10& block) override
    {
        GetStream() << "GroupLodParameters10 {"
            "boundingSphere: " << Print(block.boundingSphere) << ", "
            "unknown: " << Print(block.unknown) << ", "
            "distanceToPlayer: " << block.distanceToPlayer <<
            "}" << std::endl;
    }

    virtual void OnBlock(const block_data::GroupUnknown12& block) override
    {
        GetStream() << "GroupUnknown12 {"
            "boundingSphere: " << Print(block.boundingSphere) << ", "
            "unknown: " << Print(block.unknown0) << ", "
            "unknown: " << Print(block.unknown1) << ", "
            "unknown: " << Print(block.unknown2) << ", "
            "unknown: " << Print(block.unknown3) << ", "
            "unknown: " << Print(block.unknown4) << ", "
            "unknown: " << Print(block.unknown5) << "}" << std::endl;
    }

    virtual void OnBlock(const block_data::SimpleTrigger13& block) override
    {
        GetStream() << "SimpleTrigger13 {"
            "boundingSphere: " << Print(block.boundingSphere) << ", "
            "unknown0: " << block.unknown0 << ", "
            "unknown1: " << block.unknown1 << "}" << std::endl;
    }

    virtual void OnBlock(const block_data::SimpleUnknown14& block) override
    {
        GetStream() << "SimpleUnknown14 {"
            "boundingSphere: " << Print(block.boundingSphere) << ", "
            "unknown0: " << block.unknown0 << ", "
            "unknown0: " << block.unknown1 << ", "
            "unknown0: " << block.unknown2 << ", "
            "unknown0: " << block.unknown3 << ", "
            "unknown0: " << block.unknown4 << ", "
            "unknown0: " << block.unknown5 << ", "
            "unknown1: " << block.unknown6 << "}" << std::endl;
    }

    virtual void OnBlock(const block_data::SimpleObjectConnector18& block) override
    {
        GetStream() << "SimpleObjectConnector18 {"
            "boundingSphere: " << Print(block.boundingSphere) << ", "
            "space: " << Print(block.space) << ", "
            "object: " << Print(block.object) << "}" << std::endl;
    }

    virtual void OnBlock(const block_data::GroupObjects19& /* block */) override
    {
        GetStream() << "GroupObjects19 {}" << std::endl;
    }

    virtual void OnBlock(const block_data::SimpleFlatCollision20& block) override
    {
        GetStream() << "SimpleFlatCollision20 {"
            "boundingSphere: " << Print(block.boundingSphere) << ", "
            "unknown0: " << block.unknown0 << ", "
            "unknown1: " << block.unknown1 << "}" << std::endl;
    }

    virtual void OnBlock(const block_data::GroupObjects21& block) override
    {
        GetStream() << "GroupObjects21 {"
            "boundingSphere: " << Print(block.boundingSphere) << ", "
            "count: " << block.count << ", "
            "unknown: " << block.unknown <<
            "}" << std::endl;
    }

    virtual void OnBlock(const block_data::SimpleVolumeCollision23& block) override
    {
        GetStream() << "SimpleVolumeCollision23 {"
            "unknown0" << block.unknown0 << ", "
            "unknown1" << block.unknown1 << ", "
            "type" << block.type << ", "
            "unknown2" << block.unknown2 << ", "
            "unknown3" << block.unknown3 << ", "
            "unknown4" << block.unknown4 << "}" << std::endl;
    }

    virtual void OnBlock(const block_data::GroupTransformMatrix24& block) override
    {
        GetStream() << "GroupTransformMatrix24 {"
            "x: " << block.x << ", "
            "y: " << block.y << ", "
            "z: " << block.z << ", "
            "position: " << block.position << ", "
            "unknown: " << block.unknown <<
            "}" << std::endl;
    }

    virtual void OnBlock(const block_data::SimpleFaces28& block) override
    {
        GetStream() << "SimpleFaces28 {"
            "boundingSphere: " << Print(block.boundingSphere) << ", "
            "unknown: " << Print(block.unknown) << "}" << std::endl;
    }

    virtual void OnBlock(const block_data::GroupUnknown29& block) override
    {
        GetStream() << "GroupUnknown29 {"
            "boundingSphere: " << Print(block.boundingSphere) << ", "
            "type: " << Print(block.type) << ", "
            "unknown0: " << Print(block.unknown0) << ", "
            "unknown1: "
            "{" << Print(block.unknown1[0]) << ", "
            "" << Print(block.unknown1[1]) << ", "
            "" << Print(block.unknown1[2]) << ", "
            "" << Print(block.unknown1[3]) << ", "
            "" << Print(block.unknown1[4]) << ", "
            "" << Print(block.unknown1[5]) << ", "
            "" << Print(block.unknown1[6]) << ", "
            "" << Print(block.unknown1[7]) << ", "
            "}"
            "}" << std::endl;
    }

    virtual void OnBlock(const block_data::SimplePortal30& block) override
    {
        GetStream() << "SimplePortal30 {"
            "boundingSphere: " << Print(block.boundingSphere) << ", "
            "connectedRoom: " << Print(block.connectedRoom) << ", "
            "leftDown: " << Print(block.leftDown) << ", "
            "upRight: " << Print(block.upRight) << "}" << std::endl;
    }

    virtual void OnBlock(const block_data::GroupLightingObjects33& block) override
    {
        GetStream() << "GroupLightingObjects33 {"
            "boundingSphere: " << Print(block.boundingSphere) << ", "
            "unknown0: " << block.unknown0 << ", "
            "unknown1: " << block.unknown1 << ", "
            "unknown2: " << block.unknown2 << ", "
            "position: " << Print(block.position) << ", "
            "color: {"
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
            << block.color[11] << "}"
            "}" << std::endl;
    }

    virtual void OnBlock(const block_data::SimpleFaceData35& block) override
    {
        GetStream() << "SimpleFaceData35{"
            "boundingSphere: " << Print(block.boundingSphere) << ", "
            "type: " << block.type << ", "
            "materialIndex: " << block.materialIndex << "}" << std::endl;
    }

    virtual void OnBlock(const block_data::GroupVertexData37& block) override
    {
        GetStream() << "GroupVertexData37 {"
            "boundingSphere: " << Print(block.boundingSphere) << ", "
            "name: " << Print(block.name) << ", "
            "type: " << block.type << "}" << std::endl;
    }

    virtual void OnBlock(const block_data::SimpleGeneratedObjects40& block) override
    {
        GetStream() << "SimpleGeneratedObjects40 {"
            "boundingSphere: " << Print(block.boundingSphere) << ", "
            "empty: " << Print(block.empty) << ", "
            "name: " << Print(block.name) << ", "
            "type: " << block.type << ", "
            "unknown0: " << block.unknown0 << "}" << std::endl;
    }

    virtual void OnData(common::PositionWithTexCoordList&& data) override
    {
        PrintVectorData(data, "PositionWithTexCoordList");
    }

    virtual void OnData(block_data::Face8&& data) override
    {
        if (m_printMeshInfo)
        {
            GetStream() << "Face8 {" <<
                "type: " << data.type << ", "
                "unknown0: " << data.unknown0 << ", "
                "unknown1: " << data.unknown1 << ", "
                "materialIndex: " << data.materialIndex << "}" << std::endl;
        }
    }

    virtual void OnData(common::IndexList&& data) override
    {
        if (m_printMeshInfo)
        {
            PrintVectorData(data, "IndexList");
        }
    }

    virtual void OnData(common::IndexWithPositionList&& data) override
    {
        if (m_printMeshInfo)
        {
            PrintVectorData(data, "IndexWithPositionList");
        }
        
    }

    virtual void OnData(common::IndexWithPositionTexCoordList&& data) override
    {
        PrintVectorData(data, "IndexWithPositionTexCoordList");
    }

    virtual void OnData(common::IndexWithTexCoordList&& data) override
    {
        PrintVectorData(data, "IndexWithTexCoordList");
    }

    virtual void OnData(common::PositionList&& data) override
    {
        PrintVectorData(data, "PositionList");
    }

    virtual void OnData(block_data::Face28Entry&& data) override
    {
        GetStream() << "Face28Entry {" <<
            "type: " << data.type << ", "
            "unknown0: " << data.unknown0 << ", "
            "unknown1: " << data.unknown1 << ", "
            "materialIndex: " << data.materialIndex << "}" << std::endl;
    }

    virtual void OnData(std::vector<block_data::Face28Entry::Unknown>&& data) override
    {
        PrintVectorData(data, "std::vector<block_data::Face28Entry::Unknown>");
    }

    virtual void OnData(block_data::Mesh35&& data) override
    {
        if (m_printMeshInfo)
        {
            auto& stream = GetStream();

            stream << "Mesh35{" <<
                "type: " << data.type << ", "
                "unknown0: " << data.unknown0 << ", "
                "unknown1: " << data.unknown1 << ", "
                "materialIndex: " << data.materialIndex << "}" << std::endl;
        }
    }

    virtual void OnData(std::vector<block_data::Mesh35::Unknown49>&& data) override
    {
        if (m_printMeshInfo)
        {
            PrintVectorData(data, "std::vector<block_data::Mesh35::Unknown49>");
        }
    }

    virtual void OnData(std::vector<block_data::GroupVertexData37::Unknown514>&& data) override
    {
        PrintVectorData(data, "std::vector<block_data::GroupVertexData37::Unknown514>");
    }

    virtual void OnData(std::vector<block_data::GroupVertexData37::Unknown258>&& data)
    {
        PrintVectorData(data, "std::vector<block_data::GroupVertexData37::Unknown258>");
    }

    virtual void OnData(common::PositionWithNormalList&& data) override
    {
        PrintVectorData(data, "PositionWithNormal");
    }

    virtual void OnData(common::PositionWithTexCoordNormalList&& data) override
    {
        PrintVectorData(data, "PositionWithTexCoordNormalList");
    }

    virtual void OnData(common::CollisionPositionList&& data) override
    {
        PrintVectorData(data.data, "CollisionPositionList");
    }

    virtual void OnData(common::CollisionUnknownList&& data) override
    {
        PrintVectorData(data.data, "CollisionUnknownList");
    }

    virtual void OnData(common::TriggerInfoList&& data) override
    {
        PrintVectorData(data.data, "TriggerInfoList");
    }

    virtual void OnData(common::GeneratedObjectInfo&& data) override
    {
        PrintVectorData(data.data, "GeneratedObjectInfo");
    }

private:
    bool m_printBoundingSphere;
    bool m_newLineForVectorData;
    bool m_printVectorData;
    bool m_printMeshInfo;
    size_t m_offset = 0;

    std::string GetOffsetString(int adjustOffset = 0) const
    {
        std::string res;
        res.resize((m_offset + adjustOffset) * 4, ' ');

        return res;
    }


    private:
        std::ostream& GetStream(std::ostream& ostream = std::cout, int adjustOffset = 0)
        {
            ostream << GetOffsetString(adjustOffset);
            return ostream;
        }

        std::string Print(const uint8_t* first, const uint8_t* last)
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

        std::string Print(const Ogre::Vector3& vector)
        {
            std::ostringstream ostream;
            ostream << vector;

            return ostream.str();
        }

        std::string Print(const Ogre::Vector2& vector)
        {
            std::ostringstream ostream;
            ostream << vector;

            return ostream.str();
        }

        std::string Print(const common::BoundingSphere& boundingSphere)
        {
            std::ostringstream ostream;
            ostream << "BoundingSphere {";

            if (m_printBoundingSphere)
            {
                ostream <<
                    "center: " << Print(boundingSphere.center) << ", "
                    "radius: " << boundingSphere.radius;
            }
            ostream << "}";

            return ostream.str();
        }

        std::string Print(const common::ResourceName& resourceName)
        {
            const char* begin = reinterpret_cast<const char*>(resourceName.data());

            return "\"" + std::string(begin, begin + strnlen(begin, resourceName.size())) + "\"";
        }

        std::string Print(const common::PositionWithTexCoordNormal& positionWithTexCoordNormal)
        {
            std::ostringstream ostream;
            ostream << "PositionWithTexCoordNormal {" <<
                "position: " << Print(positionWithTexCoordNormal.position) << ", "
                "texCoord: " << Print(positionWithTexCoordNormal.texCoord) << ", "
                "normal: " << Print(positionWithTexCoordNormal.normal) << "}";

            return ostream.str();
        }

        std::string Print(const common::PositionWithNormal& positionWithNormal)
        {
            std::ostringstream ostream;
            ostream << "PositionWithNormal {" <<
                "position: " << Print(positionWithNormal.position) << ", "
                "normal: " << Print(positionWithNormal.normal) << "}";

            return ostream.str();
        }

        std::string Print(const common::PositionWithTexCoord& positionWithTexCoord)
        {
            std::ostringstream ostream;
            ostream << "PositionWithTexCoord {" <<
                "position: " << Print(positionWithTexCoord.position) << ", "
                "texCoord: " << Print(positionWithTexCoord.texCoord) << "}";

            return ostream.str();
        }

        std::string Print(const common::IndexWithPosition& indexWithPosition)
        {
            std::ostringstream ostream;
            ostream << "IndexWithPosition {" <<
                "index: " << indexWithPosition.index << ", "
                "position: " << Print(indexWithPosition.position) << "}";

            return ostream.str();
        }

        std::string Print(const common::IndexWithTexCoord& indexWithTexCoord)
        {
            std::ostringstream ostream;
            ostream << "IndexWithTexCoord {" <<
                "index: " << indexWithTexCoord.index << ", "
                "texCoord: " << Print(indexWithTexCoord.texCoord) << "}";

            return ostream.str();
        }

        std::string Print(const common::IndexWithPositionTexCoord& indexWithPositionTexCoord)
        {
            std::ostringstream ostream;
            ostream << "IPT{" <<
                "index: " << indexWithPositionTexCoord.index << ", "
                "position: " << Print(indexWithPositionTexCoord.position) << ", "
                "texCoord: " << Print(indexWithPositionTexCoord.texCoord) << "}";

            return ostream.str();
        }

        std::string Print(common::Index index)
        {
            return std::to_string(index);
        }

        std::string Print(Ogre::Real value)
        {
            return std::to_string(value);
        }

        std::string Print(const block_data::Face28Entry::Unknown& data)
        {
            std::ostringstream ostream;
            ostream << "Unknown {"
                "unknown0:" << data.unknown0 << ", "
                "unknown1:" << data.unknown1 << ", "
                "texCoord:" << Print(data.texCoord) << "}";

            return ostream.str();
        }

        std::string Print(const block_data::Mesh35::Unknown49& data)
        {
            std::ostringstream ostream;
            ostream << "Unknown {"
                "index:" << Print(data.index) << ", "
                "unknown:" << Print(data.unknown) << "}";

            return ostream.str();
        }

        std::string Print(const block_data::GroupVertexData37::Unknown514& data)
        {
            std::ostringstream ostream;
            ostream << "Unknown {"
                "position:" << Print(data.position) << ", "
                "unknown1:" << Print(data.unknown1) << ", "
                "unknown2:" << Print(data.unknown2) << ", "
                "unknown3:" << Print(data.unknown3) << "}";

            return ostream.str();
        }

        std::string Print(const block_data::GroupVertexData37::Unknown258& data)
        {
            std::ostringstream ostream;
            ostream << "Unknown {"
                "position:" << Print(data.position) << ", "
                "unknown1:" << Print(data.unknown1) << ", "
                "unknown2:" << Print(data.unknown2) << ", "
                "unknown3:" << Print(data.unknown3) << "}";

            return ostream.str();
        }

        template <typename T, typename A>
        void PrintVectorData(const std::vector<T, A>& data, const char* name)
        {
            auto& stream = GetStream();

            stream << name << "(" << data.size() << ")";

            if (m_printVectorData)
            {
                stream << " {";

                for (const auto& item : data)
                {
                    (m_newLineForVectorData ? GetStream(stream << std::endl, 1) : stream) << Print(item) << ", ";
                }

                (m_newLineForVectorData ? GetStream(stream << std::endl) : stream) << "}" << std::endl;
            }
            else
            {
                stream << std::endl;
            }
        }
};

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
    bool printBoundingSphere = false;
    bool printMeshInfo = true;
    for (int i = 2; i != argc; ++i)
    {
        if (argv[i] == std::string("--skip_vector_data"))
        {
            printVectorData = false;
        }
        else if (argv[i] == std::string("--print_bounding_sphere"))
        {
            printBoundingSphere = true;
        }
        else if (argv[i] == std::string("--skip_mesh_info"))
        {
            printMeshInfo = false;
        }
    }
    
    std::ifstream inputFile(fileName.c_str(), std::ios_base::binary);
    if (!inputFile)
    {
        std::cerr << "failed to open file [" << fileName << "]" << std::endl;
        return 2;
    }

    using namespace d2_hack::resource::data::b3d;

    try
    {
        TracingListener listener{printBoundingSphere, true, printVectorData, printMeshInfo};
        B3dReader reader;

        Ogre::FileStreamDataStream dataStream(&inputFile, false);

        reader.Read(dataStream, listener);
    }
    catch (const std::exception& e)
    {
        std::cerr << "fail: " << e.what() << std::endl;
    }

    return 0;
}
