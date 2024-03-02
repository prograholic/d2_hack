#include "b3d_object.h"

#include "b3d_tree_visitor.h"

namespace d2_hack
{
namespace app
{

using namespace resource::data::b3d;

class ObjectVisitor : public B3dTreeVisitor
{
public:
    ObjectVisitor(const std::string& b3dId,
                  Ogre::SceneManager* sceneManager,
                  Ogre::SceneNode* rootNode,
                  Ogre::MeshManager* meshManager,
                  B3dRenderableObjectList& renderables,
                  B3dRoadGroupList& roadGroupList)
        : B3dTreeVisitor(b3dId, sceneManager, rootNode, meshManager, &renderables, &roadGroupList)
    {
    }
};

B3dObject::B3dObject(const std::string& b3dId,
                     const resource::data::b3d::NodePtr& b3dNode,
                     Ogre::SceneManager* sceneManager,
                     Ogre::MeshManager* meshManager,
                     Ogre::SceneNode* rootSceneNode)
    : m_b3dNode(b3dNode)
    , m_renderables()
    , m_roadGroupList()
{
    ObjectVisitor visitor{b3dId, sceneManager, rootSceneNode, meshManager, m_renderables, m_roadGroupList};

    auto visitResult = VisitNode(m_b3dNode, visitor);
    (void)visitResult;
}

} // namespace app
} // namespace d2_hack
