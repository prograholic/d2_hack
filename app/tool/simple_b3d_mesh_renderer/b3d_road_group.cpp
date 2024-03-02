#include "b3d_road_group.h"

#include "b3d_tree_visitor.h"

namespace d2_hack
{
namespace app
{

using namespace resource::data::b3d;

class RoadGroupVisitor : public B3dTreeVisitor
{
public:
    RoadGroupVisitor(const std::string& b3dId,
                     Ogre::SceneManager* sceneManager,
                     Ogre::SceneNode* rootNode,
                     Ogre::MeshManager* meshManager,
                     B3dRenderableObjectList& renderables)
        : B3dTreeVisitor(b3dId, sceneManager, rootNode, meshManager, &renderables, nullptr)
    {
    }

    virtual VisitResult Visit(NodeGroupRoadInfraObjects4& /* node */, VisitMode /* visitMode */) override
    {
        // Prevent recursion
        return VisitResult::Continue;
    }
};

B3dRoadGroup::B3dRoadGroup(const std::string& b3dId,
                           const resource::data::b3d::NodePtr& b3dNode,
                           Ogre::SceneManager* sceneManager,
                           Ogre::MeshManager* meshManager,
                           Ogre::SceneNode* rootSceneNode)
    : m_b3dNode(b3dNode)
    , m_renderables()
{
    RoadGroupVisitor visitor{b3dId, sceneManager, rootSceneNode, meshManager, m_renderables};

    auto visitResult = VisitNode(m_b3dNode, visitor);
    (void)visitResult;
}

} // namespace app
} // namespace d2_hack
