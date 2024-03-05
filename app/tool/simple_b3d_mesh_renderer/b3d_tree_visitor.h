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

        auto sceneNode = self->GetSceneBuilder().ProcessSceneNode(node.GetName(), visitMode);

        if (visitMode == resource::data::b3d::VisitMode::PreOrder)
        {
            auto parentB3dSceneNode = self->GetSceneBuilder().GetParentB3dSceneNode();
            auto b3dSceneNode = self->CreateNode(node, parentB3dSceneNode, sceneNode);
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
    B3dSceneNodeBasePtr CreateNode(const NodeType& node, const B3dSceneNodeBasePtr& /* parent */, Ogre::SceneNode* /* sceneNode */)
    {
        resource::data::b3d::RaiseExceptionAction::RaiseException(node.GetName().c_str(), node.GetType());

        return B3dSceneNodeBasePtr{};
    }

    B3dSceneNodeBasePtr CreateNode(const resource::data::b3d::NodeGroupObjects5& node, const B3dSceneNodeBasePtr& parent, Ogre::SceneNode* sceneNode);

    B3dSceneNodeBasePtr CreateNode(const resource::data::b3d::NodeSimpleFaces8& node, const B3dSceneNodeBasePtr& parent, Ogre::SceneNode* sceneNode);

    B3dSceneNodeBasePtr CreateNode(const resource::data::b3d::NodeGroupLodParameters10& node, const B3dSceneNodeBasePtr& parent, Ogre::SceneNode* sceneNode);

    B3dSceneNodeBasePtr CreateNode(const resource::data::b3d::NodeGroupObjects19& node, const B3dSceneNodeBasePtr& parent, Ogre::SceneNode* sceneNode);

    B3dSceneNodeBasePtr CreateNode(const resource::data::b3d::NodeGroupVertexData37& node, const B3dSceneNodeBasePtr& parent, Ogre::SceneNode* sceneNode);

    B3dSceneNodeBasePtr CreateNode(const resource::data::b3d::NodeSimpleFaces28& node, const B3dSceneNodeBasePtr& parent, Ogre::SceneNode* sceneNode);
    
    B3dSceneNodeBasePtr CreateNode(const resource::data::b3d::NodeSimpleFaces35& node, const B3dSceneNodeBasePtr& parent, Ogre::SceneNode* sceneNode);

private:
    B3dSceneBuilder& m_sceneBuilder;

    template <typename FacesNode>
    void VisitFaces(FacesNode& block);
};


} // namespace app
} // namespace d2_hack

#endif /* D2_HACK_APP_TOOL_SIMPLE_B3D_MESH_RENDERER_B3D_TREE_VISITOR_H */
