#include "b3d_room.h"

#include "b3d_tree_visitor.h"

namespace d2_hack
{
namespace app
{

B3dRoom::B3dRoom(const std::string& b3dId,
                 const resource::data::b3d::NodePtr& b3dNode,
                 Ogre::SceneManager* sceneManager,
                 Ogre::MeshManager* meshManager,
                 Ogre::SceneNode* rootSceneNode)
    : m_b3dNode(b3dNode)
    , m_sceneManager(sceneManager)
    , m_meshManager(meshManager)
    , m_rootSceneNode(rootSceneNode)
{
    B3dTreeVisitor visitor{b3dId, m_sceneManager, rootSceneNode, m_meshManager};

    VisitNode(m_b3dNode, visitor);
}

void B3dRoom::GetGasStations() const
{
}

void B3dRoom::GetServiceStations() const
{
}

} // namespace app
} // namespace d2_hack
