#include "b3d_road.h"

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
#if 0
        if (node.GetName().starts_with("road_"))
        {
            // skip processing
        }
        else if (node.GetName().starts_with("hit_road_"))
        {
            if (visitMode == VisitMode::PreOrder)
            {
                //
            }
        }
#endif //0

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
    RoadVisitor visitor{b3dId, sceneManager, rootSceneNode, meshManager};

    VisitNode(m_b3dNode, visitor);
}


} // namespace app
} // namespace d2_hack
