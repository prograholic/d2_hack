#include "b3d_hit.h"

#include "b3d_tree_visitor.h"

namespace d2_hack
{
namespace app
{

using namespace resource::data::b3d;


B3dHit::B3dHit(const std::string& b3dId,
               const resource::data::b3d::NodePtr& b3dNode,
               Ogre::SceneManager* sceneManager,
               Ogre::MeshManager* meshManager,
               Ogre::SceneNode* rootSceneNode)
    : m_b3dNode(b3dNode)
    , m_renderables()
{

    B3dTreeVisitor visitor{b3dId, sceneManager, rootSceneNode, meshManager, m_renderables};

    auto visitResult = VisitNode(m_b3dNode, visitor);
    (void)visitResult;
}


} // namespace app
} // namespace d2_hack
