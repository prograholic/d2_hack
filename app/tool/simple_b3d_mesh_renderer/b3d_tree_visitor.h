#ifndef D2_HACK_APP_TOOL_SIMPLE_B3D_MESH_RENDERER_B3D_TREE_VISITOR_H
#define D2_HACK_APP_TOOL_SIMPLE_B3D_MESH_RENDERER_B3D_TREE_VISITOR_H

#include <d2_hack/common/node_base.h>

#include <d2_hack/resource/data/b3d_visitor.h>

#include "b3d_scene_builder.h"

namespace d2_hack
{
namespace app
{

struct SceneNodeGenericAction
{
    template <typename VisitorType, typename NodeType>
    static resource::data::b3d::VisitResult Perform(VisitorType* self, NodeType& node, resource::data::b3d::VisitMode visitMode)
    {

        auto ogreSceneNode = self->GetSceneBuilder().ProcessOgreSceneNode(node.GetName(), visitMode);

        if (visitMode == resource::data::b3d::VisitMode::PreOrder)
        {
            auto parentB3dSceneNode = self->GetSceneBuilder().GetParentSceneNode();
            auto b3dSceneNode = self->CreateNode(node, parentB3dSceneNode, ogreSceneNode);
            self->GetSceneBuilder().PushToSceneNodeStack(b3dSceneNode);
        }
        else
        {
            self->GetSceneBuilder().PopFromSceneNodeStack();
        }

        return resource::data::b3d::VisitResult::Continue;
    }
};

class B3dTreeVisitor : public resource::data::b3d::GenericActionVisitor<B3dTreeVisitor, SceneNodeGenericAction>
{
public:
    explicit B3dTreeVisitor(B3dSceneBuilder& sceneBuilder);

    B3dSceneBuilder& GetSceneBuilder();

    template <typename NodeType>
    scene_node::SceneNodeBasePtr CreateNode(const NodeType& node, const scene_node::SceneNodeBasePtr& /* parent */, Ogre::SceneNode* /* sceneNode */)
    {
        resource::data::b3d::RaiseExceptionAction::RaiseException(node.GetName().c_str(), node.GetType());

        return scene_node::SceneNodeBasePtr{};
    }

    scene_node::SceneNodeBasePtr CreateNode(const resource::data::b3d::NodeEventEntry& node, const scene_node::SceneNodeBasePtr& parent, Ogre::SceneNode* sceneNode);

    scene_node::SceneNodeBasePtr CreateNode(const resource::data::b3d::NodeGroupUnknown2& node, const scene_node::SceneNodeBasePtr& parent, Ogre::SceneNode* sceneNode);

    scene_node::SceneNodeBasePtr CreateNode(const resource::data::b3d::NodeGroupRoadInfraObjects4& node, const scene_node::SceneNodeBasePtr& parent, Ogre::SceneNode* sceneNode);

    scene_node::SceneNodeBasePtr CreateNode(const resource::data::b3d::NodeGroupObjects5& node, const scene_node::SceneNodeBasePtr& parent, Ogre::SceneNode* sceneNode);

    scene_node::SceneNodeBasePtr CreateNode(const resource::data::b3d::NodeGroupVertexData7& node, const scene_node::SceneNodeBasePtr& parent, Ogre::SceneNode* sceneNode);

    scene_node::SceneNodeBasePtr CreateNode(const resource::data::b3d::NodeSimpleFaces8& node, const scene_node::SceneNodeBasePtr& parent, Ogre::SceneNode* sceneNode);

    scene_node::SceneNodeBasePtr CreateNode(const resource::data::b3d::NodeGroupTrigger9& node, const scene_node::SceneNodeBasePtr& parent, Ogre::SceneNode* sceneNode);

    scene_node::SceneNodeBasePtr CreateNode(const resource::data::b3d::NodeGroupLodParameters10& node, const scene_node::SceneNodeBasePtr& parent, Ogre::SceneNode* sceneNode);

    scene_node::SceneNodeBasePtr CreateNode(const resource::data::b3d::NodeGroupUnknown12& node, const scene_node::SceneNodeBasePtr& parent, Ogre::SceneNode* sceneNode);

    scene_node::SceneNodeBasePtr CreateNode(const resource::data::b3d::NodeSimpleTrigger13& node, const scene_node::SceneNodeBasePtr& parent, Ogre::SceneNode* sceneNode);

    scene_node::SceneNodeBasePtr CreateNode(const resource::data::b3d::NodeSimpleUnknown14& node, const scene_node::SceneNodeBasePtr& parent, Ogre::SceneNode* sceneNode);

    scene_node::SceneNodeBasePtr CreateNode(const resource::data::b3d::NodeSimpleObjectConnector18& node, const scene_node::SceneNodeBasePtr& parent, Ogre::SceneNode* sceneNode);

    scene_node::SceneNodeBasePtr CreateNode(const resource::data::b3d::NodeGroupObjects19& node, const scene_node::SceneNodeBasePtr& parent, Ogre::SceneNode* sceneNode);

    scene_node::SceneNodeBasePtr CreateNode(const resource::data::b3d::NodeSimpleFlatCollision20& node, const scene_node::SceneNodeBasePtr& parent, Ogre::SceneNode* sceneNode);

    scene_node::SceneNodeBasePtr CreateNode(const resource::data::b3d::NodeGroupObjects21& node, const scene_node::SceneNodeBasePtr& parent, Ogre::SceneNode* sceneNode);

    scene_node::SceneNodeBasePtr CreateNode(const resource::data::b3d::NodeSimpleVolumeCollision23& node, const scene_node::SceneNodeBasePtr& parent, Ogre::SceneNode* sceneNode);

    scene_node::SceneNodeBasePtr CreateNode(const resource::data::b3d::NodeSimpleUnknown25& node, const scene_node::SceneNodeBasePtr& parent, Ogre::SceneNode* sceneNode);

    scene_node::SceneNodeBasePtr CreateNode(const resource::data::b3d::NodeSimpleFaces28& node, const scene_node::SceneNodeBasePtr& parent, Ogre::SceneNode* sceneNode);

    scene_node::SceneNodeBasePtr CreateNode(const resource::data::b3d::NodeGroupUnknown29& node, const scene_node::SceneNodeBasePtr& parent, Ogre::SceneNode* sceneNode);

    scene_node::SceneNodeBasePtr CreateNode(const resource::data::b3d::NodeSimplePortal30& node, const scene_node::SceneNodeBasePtr& parent, Ogre::SceneNode* sceneNode);

    scene_node::SceneNodeBasePtr CreateNode(const resource::data::b3d::NodeGroupLightingObjects33& node, const scene_node::SceneNodeBasePtr& parent, Ogre::SceneNode* sceneNode);

    scene_node::SceneNodeBasePtr CreateNode(const resource::data::b3d::NodeSimpleUnknown34& node, const scene_node::SceneNodeBasePtr& parent, Ogre::SceneNode* sceneNode);

    scene_node::SceneNodeBasePtr CreateNode(const resource::data::b3d::NodeSimpleFaces35& node, const scene_node::SceneNodeBasePtr& parent, Ogre::SceneNode* sceneNode);

    scene_node::SceneNodeBasePtr CreateNode(const resource::data::b3d::NodeGroupVertexData37& node, const scene_node::SceneNodeBasePtr& parent, Ogre::SceneNode* sceneNode);

    scene_node::SceneNodeBasePtr CreateNode(const resource::data::b3d::NodeSimpleGeneratedObjects40& node, const scene_node::SceneNodeBasePtr& parent, Ogre::SceneNode* sceneNode);

private:
    B3dSceneBuilder& m_sceneBuilder;

    // TODO: Should use Ogre::SceneNode* for binding mesh and entity???
    template <typename FacesNode>
    void VisitFaces(FacesNode& block);
};


} // namespace app
} // namespace d2_hack

#endif /* D2_HACK_APP_TOOL_SIMPLE_B3D_MESH_RENDERER_B3D_TREE_VISITOR_H */
