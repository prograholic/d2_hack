#include "b3d_renderable_object.h"

#include "b3d_tree_visitor.h"

namespace d2_hack
{
namespace app
{

using namespace resource::data::b3d;


B3dRenderableObject::B3dRenderableObject(const std::string& b3dId,
                                         const resource::data::b3d::NodePtr& b3dNode,
                                         Ogre::SceneManager* sceneManager,
                                         Ogre::MeshManager* meshManager,
                                         Ogre::SceneNode* rootSceneNode)
    : m_b3dNode(b3dNode)
    , m_sceneNode(nullptr)
{
    B3dTreeVisitor visitor{b3dId, sceneManager, rootSceneNode, meshManager};

    auto visitResult = VisitNode(m_b3dNode, visitor);
    (void)visitResult;
}


} // namespace app
} // namespace d2_hack
