#include "b3d_visitor.h"

#include <d2_hack/common/utils.h>

//#define B3D_NOT_IMPLEMENTED() D2_HACK_LOG("") << __FUNCSIG__ << ": NOT IMPLEMENTED"
#define B3D_NOT_IMPLEMENTED()


namespace d2_hack
{
namespace app
{

using namespace resource::data::b3d;

static const SubMeshInfo InvalidSubMeshInfo{ false, 0, Ogre::RenderOperation::OT_POINT_LIST };

static SubMeshInfo GetFace8Mapping(std::uint32_t type, std::uint32_t materialIndex)
{
    switch (type)
    {
    case block_data::Face8::UnknownType0:
        return { true, materialIndex, Ogre::RenderOperation::OT_TRIANGLE_FAN };

    case block_data::Face8::UnknownType1:
        return { true, materialIndex, Ogre::RenderOperation::OT_TRIANGLE_STRIP };

    case block_data::Face8::FaceIndexType2:
        return { true, materialIndex, Ogre::RenderOperation::OT_TRIANGLE_STRIP };

    case block_data::Face8::FaceIndexType3:
        return { true, materialIndex, Ogre::RenderOperation::OT_TRIANGLE_STRIP };

    case block_data::Face8::UnknownType16:
        return { true, materialIndex, Ogre::RenderOperation::OT_TRIANGLE_STRIP };

    case block_data::Face8::FaceIndexType48:
        return { false, materialIndex, Ogre::RenderOperation::OT_TRIANGLE_STRIP };

    case block_data::Face8::FaceIndexType50:
        return { false, materialIndex, Ogre::RenderOperation::OT_TRIANGLE_LIST };

    case block_data::Face8::FaceIndexType128:
        return { true, materialIndex, Ogre::RenderOperation::OT_TRIANGLE_STRIP };

    case block_data::Face8::FaceIndexType129:
        return { true, materialIndex, Ogre::RenderOperation::OT_TRIANGLE_STRIP };

    case block_data::Face8::UnknownType144:
        return { true, materialIndex, Ogre::RenderOperation::OT_TRIANGLE_STRIP };

    case block_data::Face8::FaceIndexType176:
        return { false, materialIndex, Ogre::RenderOperation::OT_TRIANGLE_STRIP };

    case block_data::Face8::FaceIndexType178:
        return { false, materialIndex, Ogre::RenderOperation::OT_TRIANGLE_STRIP };

    default:
        assert(0 && "not implemented");
        return InvalidSubMeshInfo;
    };
}

static SubMeshInfo GetFace28Mapping(std::uint32_t type, std::uint32_t materialIndex)
{
    switch (type)
    {
    case block_data::Face28Entry::Unknown2:
        return { true, materialIndex, Ogre::RenderOperation::OT_TRIANGLE_STRIP };

    default:
        assert(0 && "not implemented");
        return InvalidSubMeshInfo;
    }
}

static SubMeshInfo GetFace35Mapping(std::uint32_t blockType, std::uint32_t dataType, std::uint32_t materialIndex)
{
    if (blockType == block_data::SimpleFaceData35::IndicesOnly3)
    {
        switch (dataType)
        {
        case block_data::Mesh35::Indices0:
            return { true, materialIndex, Ogre::RenderOperation::OT_TRIANGLE_LIST };

        case block_data::Mesh35::Indices1:
            return { true, materialIndex, Ogre::RenderOperation::OT_TRIANGLE_LIST };

        case block_data::Mesh35::Indices3:
            return { true, materialIndex, Ogre::RenderOperation::OT_TRIANGLE_LIST };

        case block_data::Mesh35::Indices16:
            return { true, materialIndex, Ogre::RenderOperation::OT_TRIANGLE_LIST };

        case block_data::Mesh35::Indices17:
            return { true, materialIndex, Ogre::RenderOperation::OT_TRIANGLE_LIST };

        default:
            assert(0 && "not implemented");
            return InvalidSubMeshInfo;
        };
    }
    else if (blockType == block_data::SimpleFaceData35::Unknown2)
    {
        switch (dataType)
        {
        case block_data::Mesh35::Indices1:
            return { true, materialIndex, Ogre::RenderOperation::OT_TRIANGLE_LIST };

        case block_data::Mesh35::UnknownType3:
            return { false, materialIndex, Ogre::RenderOperation::OT_TRIANGLE_LIST };

        case block_data::Mesh35::UnknownType49:
            return { true, materialIndex, Ogre::RenderOperation::OT_TRIANGLE_LIST };

        case block_data::Mesh35::UnknownType51:
            return { false, materialIndex, Ogre::RenderOperation::OT_TRIANGLE_LIST };

        default:
            assert(0 && "not implemented");
            return InvalidSubMeshInfo;
        };
    }
    else if (blockType == block_data::SimpleFaceData35::Unknown1)
    {
        switch (dataType)
        {
        case block_data::Mesh35::Indices0:
            return { true, materialIndex, Ogre::RenderOperation::OT_TRIANGLE_LIST };

        case block_data::Mesh35::Unknown2:
            return { false, materialIndex, Ogre::RenderOperation::OT_TRIANGLE_LIST };

        case block_data::Mesh35::UnknownType48:
            return { false, materialIndex, Ogre::RenderOperation::OT_TRIANGLE_LIST };

        case block_data::Mesh35::UnknownType50:
            return { false, materialIndex, Ogre::RenderOperation::OT_TRIANGLE_LIST };

        default:
            assert(0 && "not implemented");
            return InvalidSubMeshInfo;
        };
    }
    else
    {
        assert(0 && "not implemented");
        return InvalidSubMeshInfo;
    }
}


B3dTreeVisitor::B3dTreeVisitor(const char* b3dId,
                               const std::string& b3dName,
                               Ogre::SceneManager* sceneManager,
                               Ogre::SceneNode* rootNode,
                               Ogre::MeshManager* meshManager,
                               const common::Materials& materials)
    : B3dSceneBuilder(b3dId, b3dName, sceneManager, rootNode, meshManager, materials)
{
}

void B3dTreeVisitor::Visit(const std::string& /* name */, const block_data::Empty0& /* block */, VisitMode /* visitMode */)
{
    // no need to implement
}

void B3dTreeVisitor::Visit(const std::string& name, const block_data::GroupRoadInfraObjects4& /* block */, VisitMode visitMode)
{
    ProcessSceneNode(name, visitMode);
}

void B3dTreeVisitor::Visit(const std::string& name, const block_data::GroupObjects5& /* block */, VisitMode visitMode)
{
    ProcessSceneNode(name, visitMode);
}

void B3dTreeVisitor::Visit(const std::string& name, const block_data::GroupVertex7& block, VisitMode visitMode)
{
    Ogre::MeshPtr mesh = ProcessMesh(name, visitMode);
    if (visitMode == VisitMode::PreOrder)
    {
        AddVertexData(mesh, block.vertices);
    }
}

void B3dTreeVisitor::Visit(const std::string& /* name */, const block_data::SimpleFaces8& block, VisitMode visitMode)
{
    if (visitMode == VisitMode::PreOrder)
    {
        for (const auto& data : block.faces)
        {
            Ogre::SubMesh* subMesh = CreateSubMesh(GetFace8Mapping(data.type, data.materialIndex));

            auto visitor = [this, subMesh](auto&& items)
            {
                SetSubMeshData(subMesh, items);
            };
            std::visit(visitor, data.data);
        }
    }
}

void B3dTreeVisitor::Visit(const std::string& /* name */, const block_data::GroupTrigger9& /* block */, VisitMode /* visitMode */)
{
    B3D_NOT_IMPLEMENTED();
}

void B3dTreeVisitor::Visit(const std::string& /* name */, const block_data::GroupLodParameters10& block, VisitMode visitMode)
{
    ProcessLod(block, visitMode);
}

void B3dTreeVisitor::Visit(const std::string& /* name */, const block_data::GroupUnknown12& /* block */, VisitMode /* visitMode */)
{
    B3D_NOT_IMPLEMENTED();
}

void B3dTreeVisitor::Visit(const std::string& /* name */, const block_data::SimpleTrigger13& /* block */, VisitMode /* visitMode */)
{
    B3D_NOT_IMPLEMENTED();
}

void B3dTreeVisitor::Visit(const std::string& /* name */, const block_data::SimpleUnknown14& /* block */, VisitMode /* visitMode */)
{
    B3D_NOT_IMPLEMENTED();
}

void B3dTreeVisitor::Visit(const std::string& /* name */, const block_data::SimpleObjectConnector18& block, VisitMode visitMode)
{
    ProcessObjectConnector(block, visitMode);
}

void B3dTreeVisitor::Visit(const std::string& name, const block_data::GroupObjects19& /* block */, VisitMode visitMode)
{
    ProcessSceneNode(name, visitMode);
}

void B3dTreeVisitor::Visit(const std::string& /* name */, const block_data::SimpleFlatCollision20& /* block */, VisitMode /* visitMode */)
{
    B3D_NOT_IMPLEMENTED();
}

void B3dTreeVisitor::Visit(const std::string& name, const block_data::GroupObjects21& /* block */, VisitMode visitMode)
{
    ProcessSceneNode(name, visitMode);
}

void B3dTreeVisitor::Visit(const std::string& /* name */, const block_data::SimpleVolumeCollision23& /* block */, VisitMode /* visitMode */)
{
    B3D_NOT_IMPLEMENTED();
}

void B3dTreeVisitor::Visit(const std::string& name, const block_data::GroupTransformMatrix24& block, VisitMode visitMode)
{
    ProcessTransformQueue(name, block, visitMode);
}

void B3dTreeVisitor::Visit(const std::string& /* name */, const block_data::SimpleFaces28& block, VisitMode visitMode)
{
    if (visitMode == VisitMode::PreOrder)
    {
        for (const auto& data : block.facesEntries)
        {
            Ogre::SubMesh* subMesh = CreateSubMesh(GetFace28Mapping(data.type, data.materialIndex));
            SetSubMeshData(subMesh, data);
        }
    }
}

void B3dTreeVisitor::Visit(const std::string& /* name */, const block_data::GroupUnknown29& /* block */, VisitMode /* visitMode */)
{
    B3D_NOT_IMPLEMENTED();
}

void B3dTreeVisitor::Visit(const std::string& /* name */, const block_data::SimplePortal30& /* block */, VisitMode /* visitMode */)
{
    B3D_NOT_IMPLEMENTED();
}

void B3dTreeVisitor::Visit(const std::string& name, const block_data::GroupLightingObjects33& block, VisitMode visitMode)
{
    ProcessLight(name, block, visitMode);
}

void B3dTreeVisitor::Visit(const std::string& /* name */, const block_data::SimpleFaceData35& block, VisitMode visitMode)
{
    if (visitMode == VisitMode::PreOrder)
    {
        for (const auto& data : block.meshList)
        {
            Ogre::SubMesh* subMesh = CreateSubMesh(GetFace35Mapping(block.type, data.type, data.materialIndex));

            auto visitor = [this, subMesh](auto&& items)
            {
                SetSubMeshData(subMesh, items);
            };
            std::visit(visitor, data.data);
        }
    }
}

void B3dTreeVisitor::Visit(const std::string& name, const block_data::GroupVertexData37& block, VisitMode visitMode)
{
    Ogre::MeshPtr mesh = ProcessMesh(name, visitMode);
    if (visitMode == VisitMode::PreOrder)
    {
        auto visitor = [this, mesh](auto&& vertices)
        {
            AddVertexData(mesh, vertices);
        };

        std::visit(visitor, block.data);
    }
}

void B3dTreeVisitor::Visit(const std::string& /* name */, const block_data::SimpleGeneratedObjects40& /* block */, VisitMode /* visitMode */)
{
    B3D_NOT_IMPLEMENTED();
}

} // namespace app
} // namespace d2_hack
