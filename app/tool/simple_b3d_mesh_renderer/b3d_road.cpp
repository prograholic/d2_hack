#include "b3d_road.h"

#include <d2_hack/common/log.h>

#include "b3d_tree_visitor.h"

namespace d2_hack
{
namespace app
{

using namespace resource::data::b3d;

class RoadVisitor : public B3dTreeVisitor
{
public:
    RoadVisitor(const std::string& b3dId,
                Ogre::SceneManager* sceneManager,
                Ogre::SceneNode* rootNode,
                Ogre::MeshManager* meshManager)
        : B3dTreeVisitor(b3dId, sceneManager, rootNode, meshManager)
    {
    }

    virtual void Visit(NodeGroupObjects5& node, VisitMode visitMode) override
    {
        return B3dTreeVisitor::Visit(node, visitMode);
    }
};

B3dRoad::B3dRoad(const std::string& b3dId,
                 const resource::data::b3d::NodePtr& b3dNode,
                 Ogre::SceneManager* sceneManager,
                 Ogre::MeshManager* meshManager,
                 Ogre::SceneNode* rootSceneNode)
    : m_b3dNode(b3dNode)
{
    auto hitB3dNode = b3dNode->ExtractFirstNodeWithCategory(NodeCategory::RoadHitNode);
    if (hitB3dNode)
    {
        m_hit = std::make_unique<B3dHit>(b3dId, hitB3dNode, sceneManager, meshManager, rootSceneNode);
    }

    if (!m_b3dNode->GetChildNodeList().empty())
    {
        RoadVisitor visitor{b3dId, sceneManager, rootSceneNode, meshManager};

        VisitNode(m_b3dNode, visitor);
    }
}


} // namespace app
} // namespace d2_hack
