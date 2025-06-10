#include "base_game_object.h"

#include <d2_hack/scene_node/switchable_scene_nodes.h>

#include "b3d_tree_visitor.h"

namespace d2_hack
{
namespace app
{

using namespace resource::data::b3d;

BaseGameObject::BaseGameObject(const B3dNodePtr& b3dNode,
                               const std::string& b3dId,
                               Ogre::SceneManager* sceneManager,
                               Ogre::SceneNode* rootNode,
                               Ogre::MeshManager* meshManager,
                               resource::archive::res::OgreMaterialProvider* ogreMaterialProvider)
    : m_rootNodes()
{
    B3dSceneBuilder sceneBuilder{b3dId, sceneManager, rootNode, meshManager, ogreMaterialProvider, m_rootNodes};
    B3dTreeVisitor visitor{sceneBuilder};
    auto visitResult = VisitNode(b3dNode, visitor);
    (void)visitResult;
}

void BaseGameObject::OnCameraMoved(const scene_node::WorldContext& worldContext, const Ogre::Vector3f& movement)
{
    for (const auto& rootNode : m_rootNodes)
    {
        rootNode->OnCameraMoved(worldContext, movement);
    }
}

} // namespace app
} // namespace d2_hack
