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
                               Ogre::MeshManager* meshManager)
    : B3dSceneBuilder(b3dId, sceneManager, rootNode, meshManager)
{
}

VisitResult B3dTreeVisitor::Visit(NodeHierarchyBreaker& /* node */, VisitMode /* visitMode */)
{
    // no need to implement
    return VisitResult::Continue;
}

VisitResult B3dTreeVisitor::Visit(NodeEmpty0& /* node */, VisitMode /* visitMode */)
{
    // no need to implement
    return VisitResult::Continue;
}

VisitResult B3dTreeVisitor::Visit(NodeSimpleObjectConnector1& node, VisitMode visitMode)
{
    ProcessObjectConnector(node, visitMode);

    return VisitResult::Continue;
}

VisitResult B3dTreeVisitor::Visit(NodeGroupUnknown2& /* node */, VisitMode /* visitMode */)
{
    B3D_NOT_IMPLEMENTED();

    return VisitResult::Continue;
}

VisitResult B3dTreeVisitor::Visit(NodeGroupObjects5& node, VisitMode visitMode)
{
    ProcessSceneNode(node.GetName(), visitMode);

    return VisitResult::Continue;
}

VisitResult B3dTreeVisitor::Visit(NodeSimpleFaces8& node, VisitMode visitMode)
{
    VisitFaces(node, visitMode);

    return VisitResult::Continue;
}

VisitResult B3dTreeVisitor::Visit(NodeGroupTrigger9& /* node */, VisitMode /* visitMode */)
{
    B3D_NOT_IMPLEMENTED();

    return VisitResult::Continue;
}

VisitResult B3dTreeVisitor::Visit(NodeGroupLodParameters10& /* node */, VisitMode /* visitMode */)
{
    // no need to implement

    return VisitResult::Continue;
}

VisitResult B3dTreeVisitor::Visit(NodeGroupUnknown12& /* node */, VisitMode /* visitMode */)
{
    B3D_NOT_IMPLEMENTED();

    return VisitResult::Continue;
}

VisitResult B3dTreeVisitor::Visit(NodeSimpleTrigger13& /* node */, VisitMode /* visitMode */)
{
    B3D_NOT_IMPLEMENTED();

    return VisitResult::Continue;
}

VisitResult B3dTreeVisitor::Visit(NodeSimpleUnknown14& /* node */, VisitMode /* visitMode */)
{
    B3D_NOT_IMPLEMENTED();

    return VisitResult::Continue;
}

VisitResult B3dTreeVisitor::Visit(NodeSimpleObjectConnector18& node, VisitMode visitMode)
{
    ProcessObjectConnector(node, visitMode);

    return VisitResult::Continue;
}

VisitResult B3dTreeVisitor::Visit(NodeGroupObjects19& node, VisitMode visitMode)
{
    ProcessSceneNode(node.GetName(), visitMode);

    return VisitResult::Continue;
}

VisitResult B3dTreeVisitor::Visit(NodeSimpleFlatCollision20& /* node */, VisitMode /* visitMode */)
{
    B3D_NOT_IMPLEMENTED();

    return VisitResult::Continue;
}

VisitResult B3dTreeVisitor::Visit(NodeGroupObjects21& node, VisitMode visitMode)
{
    ProcessSceneNode(node.GetName(), visitMode);

    return VisitResult::Continue;
}

VisitResult B3dTreeVisitor::Visit(NodeSimpleVolumeCollision23& /* node */, VisitMode /* visitMode */)
{
    B3D_NOT_IMPLEMENTED();

    return VisitResult::Continue;
}

VisitResult B3dTreeVisitor::Visit(NodeGroupTransformMatrix24& /* node */, VisitMode /* visitMode */)
{
    // No need to implement
    return VisitResult::Continue;
}

VisitResult B3dTreeVisitor::Visit(NodeSimpleUnknown25& /* node */, VisitMode /* visitMode */)
{
    B3D_NOT_IMPLEMENTED();

    return VisitResult::Continue;
}

VisitResult B3dTreeVisitor::Visit(NodeSimpleFaces28& node, VisitMode visitMode)
{
    VisitFaces(node, visitMode);

    return VisitResult::Continue;
}

VisitResult B3dTreeVisitor::Visit(NodeGroupUnknown29& /* node */, VisitMode /* visitMode */)
{
    B3D_NOT_IMPLEMENTED();

    return VisitResult::Continue;
}

VisitResult B3dTreeVisitor::Visit(NodeSimplePortal30& /* node */, VisitMode /* visitMode */)
{
    B3D_NOT_IMPLEMENTED();

    return VisitResult::Continue;
}

VisitResult B3dTreeVisitor::Visit(NodeGroupLightingObjects33& node, VisitMode visitMode)
{
    ProcessLight(node, visitMode);

    return VisitResult::Continue;
}

VisitResult B3dTreeVisitor::Visit(NodeSimpleUnknown34& /* node */, VisitMode /* visitMode */)
{
    B3D_NOT_IMPLEMENTED();

    return VisitResult::Continue;
}

VisitResult B3dTreeVisitor::Visit(NodeSimpleFaces35& node, VisitMode visitMode)
{
    VisitFaces(node, visitMode);

    return VisitResult::Continue;
}

VisitResult B3dTreeVisitor::Visit(NodeGroupUnknown39& /* node */, VisitMode /* visitMode */)
{
    B3D_NOT_IMPLEMENTED();

    return VisitResult::Continue;
}

VisitResult B3dTreeVisitor::Visit(NodeSimpleGeneratedObjects40& /* node */, VisitMode /* visitMode */)
{
    B3D_NOT_IMPLEMENTED();

    return VisitResult::Continue;
}

template <typename FacesNode>
void B3dTreeVisitor::VisitFaces(FacesNode& node, VisitMode visitMode)
{
    if (visitMode == VisitMode::PreOrder)
    {
        const auto& block = node.GetBlockData();
        for (const auto& face : block.faces)
        {
            CreateMesh(node.GetName(), face.meshInfo, node.GetOriginalRoot()->GetMaterialNameByIndex(face.materialIndex));
        }
    }
}


} // namespace app
} // namespace d2_hack
