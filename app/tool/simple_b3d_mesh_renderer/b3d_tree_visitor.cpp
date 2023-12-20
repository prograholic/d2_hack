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



B3dTreeVisitor::B3dTreeVisitor(const std::string& b3dId,
                               Ogre::SceneManager* sceneManager,
                               Ogre::SceneNode* rootNode,
                               Ogre::MeshManager* meshManager,
                               const common::Materials& materials)
    : B3dSceneBuilder(b3dId, sceneManager, rootNode, meshManager, materials)
{
}

void B3dTreeVisitor::Visit(resource::data::b3d::NodeEmpty0& /* node */, VisitMode /* visitMode */)
{
    // no need to implement
}

void B3dTreeVisitor::Visit(resource::data::b3d::NodeSimpleObjectConnector1& node, VisitMode visitMode)
{
    ProcessObjectConnector(node, visitMode);
}

void B3dTreeVisitor::Visit(resource::data::b3d::NodeGroupUnknown2& /* node */, VisitMode /* visitMode */)
{
    B3D_NOT_IMPLEMENTED();
}

void B3dTreeVisitor::Visit(resource::data::b3d::NodeGroupRoadInfraObjects4& node, VisitMode visitMode)
{
    ProcessSceneNode(node.GetName(), visitMode);
}

void B3dTreeVisitor::Visit(resource::data::b3d::NodeGroupObjects5& node, VisitMode visitMode)
{
    ProcessSceneNode(node.GetName(), visitMode);
}

void B3dTreeVisitor::Visit(resource::data::b3d::NodeGroupVertexData7& node, VisitMode visitMode)
{
    ProcessSceneNode(node.GetName(), visitMode);
}

void B3dTreeVisitor::Visit(resource::data::b3d::NodeSimpleFaces8& node, VisitMode visitMode)
{
    VisitFaces(node, visitMode);
}

void B3dTreeVisitor::Visit(resource::data::b3d::NodeGroupTrigger9& /* node */, VisitMode /* visitMode */)
{
    B3D_NOT_IMPLEMENTED();
}

void B3dTreeVisitor::Visit(resource::data::b3d::NodeGroupLodParameters10& /* node */, VisitMode /* visitMode */)
{
    // no need to implement
}

void B3dTreeVisitor::Visit(resource::data::b3d::NodeGroupUnknown12& /* node */, VisitMode /* visitMode */)
{
    B3D_NOT_IMPLEMENTED();
}

void B3dTreeVisitor::Visit(resource::data::b3d::NodeSimpleTrigger13& /* node */, VisitMode /* visitMode */)
{
    B3D_NOT_IMPLEMENTED();
}

void B3dTreeVisitor::Visit(resource::data::b3d::NodeSimpleUnknown14& /* node */, VisitMode /* visitMode */)
{
    B3D_NOT_IMPLEMENTED();
}

void B3dTreeVisitor::Visit(resource::data::b3d::NodeSimpleObjectConnector18& node, VisitMode visitMode)
{
    ProcessObjectConnector(node, visitMode);
}

void B3dTreeVisitor::Visit(resource::data::b3d::NodeGroupObjects19& node, VisitMode visitMode)
{
    ProcessSceneNode(node.GetName(), visitMode);
}

void B3dTreeVisitor::Visit(resource::data::b3d::NodeSimpleFlatCollision20& /* node */, VisitMode /* visitMode */)
{
    B3D_NOT_IMPLEMENTED();
}

void B3dTreeVisitor::Visit(resource::data::b3d::NodeGroupObjects21& node, VisitMode visitMode)
{
    ProcessSceneNode(node.GetName(), visitMode);
}

void B3dTreeVisitor::Visit(resource::data::b3d::NodeSimpleVolumeCollision23& /* node */, VisitMode /* visitMode */)
{
    B3D_NOT_IMPLEMENTED();
}

void B3dTreeVisitor::Visit(resource::data::b3d::NodeGroupTransformMatrix24& node, VisitMode visitMode)
{
    ProcessTransformQueue(node, visitMode);
}

void B3dTreeVisitor::Visit(resource::data::b3d::NodeSimpleUnknown25& /* node */, VisitMode /* visitMode */)
{
    B3D_NOT_IMPLEMENTED();
}

void B3dTreeVisitor::Visit(resource::data::b3d::NodeSimpleFaces28& node, VisitMode visitMode)
{
    VisitFaces(node, visitMode);
}

void B3dTreeVisitor::Visit(resource::data::b3d::NodeGroupUnknown29& /* node */, VisitMode /* visitMode */)
{
    B3D_NOT_IMPLEMENTED();
}

void B3dTreeVisitor::Visit(resource::data::b3d::NodeSimplePortal30& /* node */, VisitMode /* visitMode */)
{
    B3D_NOT_IMPLEMENTED();
}

void B3dTreeVisitor::Visit(resource::data::b3d::NodeGroupLightingObjects33& node, VisitMode visitMode)
{
    ProcessLight(node, visitMode);
}

void B3dTreeVisitor::Visit(resource::data::b3d::NodeSimpleUnknown34& /* node */, VisitMode /* visitMode */)
{
    B3D_NOT_IMPLEMENTED();
}

void B3dTreeVisitor::Visit(resource::data::b3d::NodeSimpleFaces35& node, VisitMode visitMode)
{
    VisitFaces(node, visitMode);
}

void B3dTreeVisitor::Visit(resource::data::b3d::NodeGroupVertexData36& node, VisitMode visitMode)
{
    ProcessSceneNode(node.GetName(), visitMode);
}

void B3dTreeVisitor::Visit(resource::data::b3d::NodeGroupVertexData37& node, VisitMode visitMode)
{
    ProcessSceneNode(node.GetName(), visitMode);
}

void B3dTreeVisitor::Visit(resource::data::b3d::NodeGroupUnknown39& /* node */, VisitMode /* visitMode */)
{
    B3D_NOT_IMPLEMENTED();
}

void B3dTreeVisitor::Visit(resource::data::b3d::NodeSimpleGeneratedObjects40& /* node */, VisitMode /* visitMode */)
{
    B3D_NOT_IMPLEMENTED();
}

template <typename FacesNode>
void B3dTreeVisitor::VisitFaces(FacesNode& node, resource::data::b3d::VisitMode visitMode)
{
    if (visitMode == VisitMode::PreOrder)
    {
        const auto& block = node.GetBlockData();
        for (const auto& face : block.faces)
        {
            CreateMesh(node.GetName(), face.meshInfo, face.materialIndex);
        }
    }
}


} // namespace app
} // namespace d2_hack
