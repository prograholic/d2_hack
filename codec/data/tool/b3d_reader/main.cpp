#include <iostream>
#include <iomanip>
#include <fstream>
#include <strstream>
#include <string>
#include <stdexcept>

#include <boost/variant/get.hpp>

#include <d2_hack/codec/data/b3d.h>
#include <d2_hack/codec/data/b3d_reader.h>

namespace d2_hack
{
namespace codec
{
namespace data
{
namespace b3d
{

class TracingVisitor : public B3dVisitorInterface
{
public:
    TracingVisitor(bool printBoundingSphere, bool newLineForMesh35, bool newLineForVertex)
        : m_printBoundingSphere(printBoundingSphere)
        , m_newLineForMesh35(newLineForMesh35)
        , m_newLineForVertex(newLineForVertex)
    {
    }

    bool m_printBoundingSphere;
    bool m_newLineForMesh35;
    bool m_newLineForVertex;

    virtual void VisitBlockSeparator(std::uint32_t blockSeparator) override
    {
        if (blockSeparator == 333)
        {
            //GetStream() << "{" << std::endl;
        }
        else if (blockSeparator == 555)
        {
            //GetStream() << "}" << std::endl;
        }
    }

    virtual void VisitBlockHeader(const common::ResourceName& name, std::uint32_t type) override
    {
        m_offset += 1;
        GetStream() << "name: " << Print(name) << std::endl;
        GetStream() << "type: " << type << std::endl;
    }

    virtual void VisitBlock(const common::ResourceName& /* name */, std::uint32_t /* type */) override
    {
        m_offset -= 1;
        GetStream() << std::endl;
    }

    virtual void VisitNestedBlockHeader(std::uint32_t nestedBlockCount) override
    {
        GetStream() << "nested: " << nestedBlockCount << std::endl;
    }

    virtual std::ostream* GetDebugOutStream()  override
    {
        return &std::cout;
    }

    std::ostream& GetStream(std::ostream& ostream = std::cout, int adjustOffset = 0)
    {
        ostream << GetOffsetString(adjustOffset);
        return ostream;
    }

    std::string Print(const uint8_t* first, const uint8_t* last)
    {
        std::ostringstream ostream;
        ostream << "{";
        for ( ; first != last; ++first)
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

    virtual void VisitBlockData(const block_data::Empty0& blockData) override
    {
        GetStream() << "Empty0 {"
            "emptyData0: " << Print(std::begin(blockData.emptyData0), std::end(blockData.emptyData0)) << ", "
            "unknown: " << blockData.unknown << ", "
            "emptyData1: " << Print(std::begin(blockData.emptyData1), std::end(blockData.emptyData1)) << 
            "}" << std::endl;
    }

    virtual void VisitBlockData(const block_data::GroupRoadInfraObjects4& /* blockData */) override
    {
        throw std::runtime_error("GroupRoadInfraObjects4");
    }

    virtual void VisitBlockData(const block_data::GroupObjects5& blockData) override
    {
        GetStream() << "GroupObjects5 {"
            "boundingSphere: " << Print(blockData.boundingSphere) << ", "
            "name: " << Print(blockData.name) << "}" << std::endl;
    }

    virtual void VisitBlockData(const block_data::GroupVertex7& blockData) override
    {
        GetStream() << "GroupVertex7 {"
            "boundingSphere: " << Print(blockData.boundingSphere) << ", "
            "name: " << Print(blockData.name) << ", "
            "vertices(" << blockData.vertices.size() << "): {";

        for (const auto& vertex : blockData.vertices)
        {
            (m_newLineForVertex ? GetStream(std::cout << std::endl, 1) : std::cout) << Print(vertex) << ", ";
        }

        (m_newLineForVertex ? GetStream(std::cout << std::endl) : std::cout) << "}" << std::endl;
    }

    std::string Print(const block_data::Face8::FaceData& faceData, std::uint32_t type)
    {
        std::ostringstream ostream;

        switch (type)
        {
        case block_data::Face8::UnknownType0:
        case block_data::Face8::UnknownType1:
        case block_data::Face8::UnknownType16:
        case block_data::Face8::FaceIndexType129:
        case block_data::Face8::UnknownType144:
            ostream << "index: " << boost::get<common::Index>(faceData);
            break;

        case block_data::Face8::FaceIndexType3:
            ostream << Print(boost::get<common::IndexWithTexCoord>(faceData));
            break;

        case block_data::Face8::FaceIndexType48:
        case block_data::Face8::FaceIndexType176:
            ostream << Print(boost::get<common::IndexWithPosition>(faceData));
            break;

        case block_data::Face8::FaceIndexType50:
        case block_data::Face8::FaceIndexType178:
            ostream << Print(boost::get<common::IndexWithPositionTexCoord>(faceData));
            break;

        default:
            throw std::runtime_error("Print(Face8::FaceData): unknown type: " + std::to_string(type));

        }

        return ostream.str();
    }

    std::string Print(const block_data::Face8& face)
    {
        std::ostringstream ostream;
        ostream << "Face8 {" <<
            "type: " << face.type << ", "
            "unknown0: " << face.unknown0 << ", "
            "unknown1: " << face.unknown1 << ", "
            "unknown2: " << face.unknown2 << ", "
            "faceDataList(" << face.faceDataList.size() << "): {";

        for (const auto& faceData : face.faceDataList)
        {
            ostream << Print(faceData, face.type) << ", ";
        }

        ostream << "}";

        return ostream.str();
    }

    virtual void VisitBlockData(const block_data::SimpleFaces8& blockData) override
    {
        GetStream() << "SimpleFaces8 {"
            "boundingSphere: " << Print(blockData.boundingSphere) << ", "
            "faces(" << blockData.faces.size() << "): {";

        for (const auto& face : blockData.faces)
        {
            std::cout << Print(face) << ", ";
        }

        std::cout << "}" << std::endl;
    }

    virtual void VisitBlockData(const block_data::GroupTrigger9& blockData) override
    {
        GetStream() << "GroupTrigger9 {"
            "boundingSphere: " << Print(blockData.boundingSphere) << ", "
            "unknown: " << blockData.unknown << ", "
            "distanceToPlayer: " << blockData.distanceToPlayer << "}" << std::endl;
    }

    virtual void VisitBlockData(const block_data::GroupLodParameters10& blockData) override
    {
        GetStream() << "GroupLodParameters10 {"
            "boundingSphere: " << Print(blockData.boundingSphere) << ", "
            "unknown: " << Print(blockData.unknown) << ", "
            "distanceToPlayer: " << blockData.distanceToPlayer << 
            "}" << std::endl;
    }

    virtual void VisitBlockData(const block_data::GroupUnknown12& /* blockData */) override
    {
        throw std::runtime_error("GroupUnknown12");
    }

    virtual void VisitBlockData(const block_data::SimpleTrigger13& blockData) override
    {
        GetStream() << "SimpleTrigger13 {"
            "boundingSphere: " << Print(blockData.boundingSphere) << ", "
            "unknown0: " << blockData.unknown0 << ", "
            "unknown1: " << blockData.unknown1 << ", "
            "unknown2(" << blockData.unknown2.size() << "): {";

        for (const auto& u : blockData.unknown2)
        {
            std::cout << u << ", ";
        }

        std::cout << "}" << std::endl;
    }

    virtual void VisitBlockData(const block_data::SimpleObjectConnector18& blockData) override
    {
        GetStream() << "SimpleObjectConnector18 {"
            "boundingSphere: " << Print(blockData.boundingSphere) << ", "
            "space: " << Print(blockData.space) << ", "
            "object: " << Print(blockData.object) << "}" << std::endl;
    }

    virtual void VisitBlockData(const block_data::GroupObjects19& /* blockData */) override
    {
        GetStream() << "GroupObjects19 {}" << std::endl;
    }

    virtual void VisitBlockData(const block_data::SimpleFlatCollision20& blockData) override
    {
        GetStream() << "SimpleFlatCollision20 {"
            "boundingSphere: " << Print(blockData.boundingSphere) << ", "
            "unknown0: " << blockData.unknown0 << ", "
            "unknown1: " << blockData.unknown1 << ", "
            "a(" << blockData.a.size() << "): {";
        
        for (const auto& a : blockData.a)
        {
            std::cout << Print(a) << ", ";
        }
        
        std::cout << "}"
            "b(" << blockData.b.size() << "): {";
        
        for (const auto& a : blockData.a)
        {
            std::cout << a << ", ";
        }

        std::cout << "}" << std::endl;
    }

    virtual void VisitBlockData(const block_data::GroupObjects21& blockData) override
    {
        GetStream() << "GroupObjects21 {"
            "boundingSphere: " << Print(blockData.boundingSphere) << ", "
            "count: " << blockData.count << ", "
            "unknown: " << blockData.unknown << 
            "}" << std::endl;
    }

    std::string Print(const block_data::SimpleVolumeCollision23::Unknown0& unknown0)
    {
        std::ostringstream ostream;
        ostream << "unknown0(" << unknown0.unknown.size() << "): {";

        for (const auto& u : unknown0.unknown)
        {
            ostream << Print(u) << ", ";
        }

        ostream << "}";

        return ostream.str();
    }

    std::string Print(const block_data::SimpleVolumeCollision23::CollisionEntry& collisionEntry, std::uint32_t type)
    {
        std::ostringstream ostream;

        switch (type)
        {
        case block_data::SimpleVolumeCollision23::UnknownType0:
            ostream << Print(boost::get<block_data::SimpleVolumeCollision23::Unknown0>(collisionEntry));
            break;

        default:
            throw std::runtime_error("Print(SimpleVolumeCollision23::CollisionEntry): unknown type: " + std::to_string(type));

        }

        return ostream.str();
    }

    virtual void VisitBlockData(const block_data::SimpleVolumeCollision23& blockData) override
    {
        GetStream() << "SimpleVolumeCollision23 {"
            "unknown0: " << blockData.unknown0 << ", "
            "unknown1: " << blockData.unknown1 << ", "
            "type: " << blockData.type << ", "
            "unknown2: " << blockData.unknown2 << ", "
            "unknown3: " << blockData.unknown3 << ", "
            "unknown4: " << blockData.unknown4 << ", "
            "collisionEntries(" << blockData.collisionEntries.size() << "): {";

        for (const auto& collisionEntry : blockData.collisionEntries)
        {
            std::cout << Print(collisionEntry, blockData.type) << ", ";
        }

        std::cout << "}" << std::endl;
    }

    virtual void VisitBlockData(const block_data::GroupTransformMatrix24& blockData) override
    {
        GetStream() << "GroupTransformMatrix24 {"
            "x: " << Print(blockData.x) << ", "
            "y: " << Print(blockData.y) << ", "
            "z: " << Print(blockData.z) << ", "
            "position: " << Print(blockData.position) << ", "
            "unknown: " << blockData.unknown << 
            "}" << std::endl;
    }

    std::string Print(const block_data::Face28Entry::FaceData& face, std::uint32_t type)
    {
        std::ostringstream ostream;

        switch (type)
        {
        case block_data::Face28Entry::Unknown2:
            {
                block_data::Face28Entry::Unknown u = boost::get<block_data::Face28Entry::Unknown>(face);
                ostream << "Unknown {"
                    "unknown0:" << u.unknown0 << ", "
                    "unknown1:" << u.unknown1 << ", "
                    "unknown2:" << u.unknown2 << ", "
                    "unknown3:" << u.unknown3 << "}";
            }
            break;

        default:
            throw std::runtime_error("Print(Face28Entry::Unknown): unknown type: " + std::to_string(type));

        }

        return ostream.str();
    }

    std::string Print(const block_data::Face28Entry& faceEntry)
    {
        std::ostringstream ostream;
        ostream << "Face28Entry {" <<
            "type: " << faceEntry.type << ", "
            "unknown0: " << faceEntry.unknown0 << ", "
            "unknown1: " << faceEntry.unknown1 << ", "
            "unknown2: " << faceEntry.unknown2 << ", "
            "faceData(" << faceEntry.faceData.size() << "): {";

        for (const auto& face : faceEntry.faceData)
        {
            ostream << Print(face, faceEntry.type) << ", ";
        }

        ostream << "}";

        return ostream.str();
    }

    virtual void VisitBlockData(const block_data::SimpleFaces28& blockData) override
    {
        GetStream() << "SimpleFaces28 {"
            "boundingSphere: " << Print(blockData.boundingSphere) << ", "
            "unknown: " << Print(blockData.unknown) << ", "
            "facesEntries(" << blockData.facesEntries.size() << "): {";

        for (const auto& facesEntry : blockData.facesEntries)
        {
            std::cout << Print(facesEntry) << ", ";
        }

        std::cout << "}" << std::endl;
    }

    virtual void VisitBlockData(const block_data::SimplePortal30& blockData) override
    {
        GetStream() << "SimplePortal30 {"
            "boundingSphere: " << Print(blockData.boundingSphere) << ", "
            "connectedRoom: " << Print(blockData.connectedRoom) << ", "
            "leftDown: " << Print(blockData.leftDown) << ", "
            "upRight: " << Print(blockData.upRight) << "}" << std::endl;
    }

    virtual void VisitBlockData(const block_data::GroupLightingObjects33& blockData) override
    {
        GetStream() << "GroupLightingObjects33 {"
            "boundingSphere: " << Print(blockData.boundingSphere) << ", "
            "unknown0: " << blockData.unknown0 << ", "
            "unknown1: " << blockData.unknown1 << ", "
            "unknown2: " << blockData.unknown2 << ", "
            "position: " << Print(blockData.position) << ", "
            "color: {"
            << blockData.color[0] << ", "
            << blockData.color[1] << ", "
            << blockData.color[2] << ", "
            << blockData.color[3] << ", "
            << blockData.color[4] << ", "
            << blockData.color[5] << ", "
            << blockData.color[6] << ", "
            << blockData.color[7] << ", "
            << blockData.color[8] << ", "
            << blockData.color[9] << ", "
            << blockData.color[10] << ", "
            << blockData.color[11] << "}"
            "}" << std::endl;
    }

    std::string Print(const block_data::Mesh35::MeshData& meshData, std::uint32_t type)
    {
        std::ostringstream ostream;

        switch (type)
        {
        case block_data::Mesh35::Indices0:
        case block_data::Mesh35::Indices1:
        case block_data::Mesh35::Indices3:
        case block_data::Mesh35::Indices16:
            ostream << "index: " << boost::get<common::Index>(meshData);
        break;

        case block_data::Mesh35::UnknownType50:
            ostream << Print(boost::get<common::IndexWithPositionTexCoord>(meshData));
            break;

        case block_data::Mesh35::UnknownType48:
            ostream << Print(boost::get<common::IndexWithPosition>(meshData));
            break;

        default:
            throw std::runtime_error("Print(Mesh35::MeshData): unknown type: " + std::to_string(type));
            
        }

        return ostream.str();
    }

    std::string Print(const block_data::Mesh35& mesh)
    {
        std::ostringstream ostream;

        ostream << "Mesh35{" <<
            "type: " << mesh.type << ", "
            "unknown0: " << mesh.unknown0 << ", "
            "unknown1: " << mesh.unknown1 << ", "
            "materialIndex: " << mesh.materialIndex << ", "
            "meshDataList(" << mesh.meshDataList.size() << "): {";

        for (const auto& meshData : mesh.meshDataList)
        {
            ostream << Print(meshData, mesh.type) << ", ";
        }

        ostream << "}";
        ostream << "}";

        return ostream.str();
    }

    virtual void VisitBlockData(const block_data::SimpleFaceData35& blockData) override
    {
        GetStream() << "SimpleFaceData35{"
            "boundingSphere: " << Print(blockData.boundingSphere) << ", "
            "type: " << blockData.type << ", "
            "meshIndex: " << blockData.meshIndex << ", "
            "meshList(" << blockData.meshList.size() << "): {";
            
        for (const auto& mesh : blockData.meshList)
        {
            (m_newLineForMesh35 ? GetStream(std::cout << std::endl, 1) : std::cout) << Print(mesh) << ", ";
        }

        (m_newLineForMesh35 ? GetStream(std::cout << std::endl) : std::cout) << "}" << std::endl;
    }

    std::string Print(const block_data::GroupVertexData37::VertexData& vertexData, std::uint32_t type)
    {
        std::ostringstream ostream;
        switch (type)
        {
        case block_data::GroupVertexData37::Vertex2:
            ostream << Print(boost::get<common::PositionWithTexCoordNormal>(vertexData));
            break;

        case block_data::GroupVertexData37::Vertex3:
            ostream << Print(boost::get<common::PositionWithNormal>(vertexData));
            break;

        case block_data::GroupVertexData37::UnknownType514:
            {
                block_data::GroupVertexData37::Unknown514 u = boost::get<block_data::GroupVertexData37::Unknown514>(vertexData);
                ostream << "Unknown {"
                    "unknown0:" << Print(u.unknown0) << ", "
                    "unknown1:" << Print(u.unknown1) << ", "
                    "unknown2:" << Print(u.unknown2) << ", "
                    "unknown3:" << Print(u.unknown3) << "}";
            }
            break;

        default:
            throw std::runtime_error("Print(GroupVertexData37::VertexData): unknown type: " + std::to_string(type));
        }

        return ostream.str();
    }

    virtual void VisitBlockData(const block_data::GroupVertexData37& blockData) override
    {
        GetStream() << "GroupVertexData37 {"
            "boundingSphere: " << Print(blockData.boundingSphere) << ", "
            "name: " << Print(blockData.name) << ", "
            "type: " << blockData.type << ", "
            "vertexDataList(" << blockData.vertexDataList.size() << "): {";

        for (const auto& vertexData : blockData.vertexDataList)
        {
            (m_newLineForVertex ? GetStream(std::cout << std::endl, 1) : std::cout) << Print(vertexData, blockData.type) << ", ";
        }

        (m_newLineForVertex ? GetStream(std::cout << std::endl) : std::cout) << "}" << std::endl;
    }

    virtual void VisitBlockData(const block_data::SimpleGeneratedObjects40& blockData) override
    {
        GetStream() << "SimpleGeneratedObjects40 {"
            "boundingSphere: " << Print(blockData.boundingSphere) << ", "
            "empty: " << Print(blockData.empty) << ", "
            "name: " << Print(blockData.name) << ", "
            "type: " << blockData.type << ", "
            "unknown0: " << blockData.unknown0 << ", "
            "unknown1(" << blockData.unknown1.size() << "): {";

        for (const auto& u1 : blockData.unknown1)
        {
            std::cout << u1 << ", ";
        }

        std::cout << "}" << std::endl;
    }

private:
    size_t m_offset = 0;

    std::string GetOffsetString(int adjustOffset = 0) const
    {
        std::string res;
        res.resize((m_offset + adjustOffset) * 4, ' ');

        return res;
    }
};

} // namespace b3d
} // namespace data
} // namespace codec
} // namespace d2_hack








int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        std::cerr << "usage: " << argv[0] << " <input_file>" << std::endl;
        return 1;
    }

    std::string fileName = argv[1];

    std::ifstream inputFile(fileName.c_str(), std::ios_base::binary);
    if (!inputFile)
    {
        std::cerr << "failed to open file [" << fileName << "]" << std::endl;
        return 2;
    }

    using namespace d2_hack::codec::data::b3d;

    try
    {
        TracingVisitor visitor{false, true, true};
        B3dReader reader;
        B3d data;
        reader.Read(inputFile, data, &visitor);
    }
    catch (const std::exception& e)
    {
        std::cerr << "fail: " << e.what() << std::endl;
    }



    return 0;
}
