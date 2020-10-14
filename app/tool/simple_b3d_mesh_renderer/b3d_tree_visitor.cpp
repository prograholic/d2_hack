#include "b3d_tree_visitor.h"

#include <d2_hack/common/utils.h>
#include <d2_hack/common/log.h>

#include <d2_hack/resource/data/b3d_utils.h>

//#define B3D_NOT_IMPLEMENTED() D2_HACK_LOG("") << __FUNCSIG__ << ": NOT IMPLEMENTED"
#define B3D_NOT_IMPLEMENTED()


namespace d2_hack
{
namespace app
{

using namespace resource::data::b3d;



B3dTreeVisitor::B3dTreeVisitor(const char* b3dId,
                               const std::string& b3dName,
                               Ogre::SceneManager* sceneManager,
                               Ogre::SceneNode* rootNode,
                               Ogre::MeshManager* meshManager,
                               const common::Materials& materials)
    : B3dSceneBuilder(b3dId, b3dName, sceneManager, rootNode, meshManager, materials)
{
}

void B3dTreeVisitor::Visit(const std::string& /* name */, block_data::Empty0& /* block */, VisitMode /* visitMode */)
{
    // no need to implement
}

void B3dTreeVisitor::Visit(const std::string& name, block_data::GroupRoadInfraObjects4& /* block */, VisitMode visitMode)
{
    ProcessSceneNode(name, visitMode);
}

void B3dTreeVisitor::Visit(const std::string& name, block_data::GroupObjects5& /* block */, VisitMode visitMode)
{
    ProcessSceneNode(name, visitMode);
}

void B3dTreeVisitor::Visit(const std::string& name, block_data::GroupVertex7& block, VisitMode visitMode)
{
    Ogre::MeshPtr mesh = ProcessMesh(name, visitMode);
    if (visitMode == VisitMode::PreOrder)
    {
        AddVertexData(mesh, block.vertices);
    }
}

void B3dTreeVisitor::Visit(const std::string& name, block_data::SimpleFaces8& block, VisitMode visitMode)
{
    VisitFaces(name, block, visitMode);
}

void B3dTreeVisitor::Visit(const std::string& /* name */, block_data::GroupTrigger9& /* block */, VisitMode /* visitMode */)
{
    B3D_NOT_IMPLEMENTED();
}

void B3dTreeVisitor::Visit(const std::string& /* name */, block_data::GroupLodParameters10& block, VisitMode visitMode)
{
    ProcessLod(block, visitMode);
}

void B3dTreeVisitor::Visit(const std::string& /* name */, block_data::GroupUnknown12& /* block */, VisitMode /* visitMode */)
{
    B3D_NOT_IMPLEMENTED();
}

void B3dTreeVisitor::Visit(const std::string& /* name */, block_data::SimpleTrigger13& /* block */, VisitMode /* visitMode */)
{
    B3D_NOT_IMPLEMENTED();
}

void B3dTreeVisitor::Visit(const std::string& /* name */, block_data::SimpleUnknown14& /* block */, VisitMode /* visitMode */)
{
    B3D_NOT_IMPLEMENTED();
}

void B3dTreeVisitor::Visit(const std::string& /* name */, block_data::SimpleObjectConnector18& block, VisitMode visitMode)
{
    ProcessObjectConnector(block, visitMode);
}

void B3dTreeVisitor::Visit(const std::string& name, block_data::GroupObjects19& /* block */, VisitMode visitMode)
{
    ProcessSceneNode(name, visitMode);
}

void B3dTreeVisitor::Visit(const std::string& /* name */, block_data::SimpleFlatCollision20& /* block */, VisitMode /* visitMode */)
{
    B3D_NOT_IMPLEMENTED();
}

void B3dTreeVisitor::Visit(const std::string& name, block_data::GroupObjects21& /* block */, VisitMode visitMode)
{
    ProcessSceneNode(name, visitMode);
}

void B3dTreeVisitor::Visit(const std::string& /* name */, block_data::SimpleVolumeCollision23& /* block */, VisitMode /* visitMode */)
{
    B3D_NOT_IMPLEMENTED();
}

void B3dTreeVisitor::Visit(const std::string& name, block_data::GroupTransformMatrix24& block, VisitMode visitMode)
{
    ProcessTransformQueue(name, block, visitMode);
}

void B3dTreeVisitor::Visit(const std::string& name, block_data::SimpleFaces28& block, VisitMode visitMode)
{
    VisitFaces(name, block, visitMode);
}

void B3dTreeVisitor::Visit(const std::string& /* name */, block_data::GroupUnknown29& /* block */, VisitMode /* visitMode */)
{
    B3D_NOT_IMPLEMENTED();
}

void B3dTreeVisitor::Visit(const std::string& /* name */, block_data::SimplePortal30& /* block */, VisitMode /* visitMode */)
{
    B3D_NOT_IMPLEMENTED();
}

void B3dTreeVisitor::Visit(const std::string& name, block_data::GroupLightingObjects33& block, VisitMode visitMode)
{
    ProcessLight(name, block, visitMode);
}

void B3dTreeVisitor::Visit(const std::string& name, block_data::SimpleFaces35& block, VisitMode visitMode)
{
    VisitFaces(name, block, visitMode);
}

void B3dTreeVisitor::Visit(const std::string& name, block_data::GroupVertexData37& block, VisitMode visitMode)
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

void B3dTreeVisitor::Visit(const std::string& /* name */, block_data::SimpleGeneratedObjects40& /* block */, VisitMode /* visitMode */)
{
    B3D_NOT_IMPLEMENTED();
}

template <typename Faces>
void B3dTreeVisitor::VisitFaces(const std::string& /* name */, Faces& block, resource::data::b3d::VisitMode visitMode)
{
    if (visitMode == VisitMode::PreOrder)
    {
        for (const auto& face : block.faces)
        {
            Ogre::SubMesh* subMesh = CreateSubMesh(GetFaceMapping(block, face));

            auto visitor = [this, subMesh](auto&& items)
            {
                SetSubMeshData(subMesh, items);
            };
            std::visit(visitor, face.data);
        }
    }
}


} // namespace app
} // namespace d2_hack
