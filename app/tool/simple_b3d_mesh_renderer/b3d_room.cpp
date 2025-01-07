#include "b3d_room.h"

#include "b3d_tree_visitor.h"

namespace d2_hack
{
namespace app
{

using namespace resource::data::b3d;

B3dRoom::B3dRoom(const B3dNodePtr& b3dNode,
                 const std::string& b3dId,
                 Ogre::SceneManager* sceneManager,
                 Ogre::SceneNode* rootNode,
                 Ogre::MeshManager* meshManager)
    : m_rootNodes()
{
    B3dSceneBuilder sceneBuilder{b3dId, sceneManager, rootNode, meshManager, m_rootNodes};
    B3dTreeVisitor visitor{sceneBuilder};
    auto visitResult = VisitNode(b3dNode, visitor);
    (void)visitResult;
}

void B3dRoom::PlayerMoved(const scene_node::WorldContext& worldContext, const Ogre::Vector3f& movement)
{
    for (const auto& rootNode : m_rootNodes)
    {
        rootNode->PlayerMoved(worldContext, movement);
    }
}

} // namespace app
} // namespace d2_hack
