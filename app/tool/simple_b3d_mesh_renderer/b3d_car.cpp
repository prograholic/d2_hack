#include "b3d_car.h"

#include "b3d_tree_visitor.h"

namespace d2_hack
{
namespace app
{

using namespace resource::data::b3d;

class CarVisitor : public B3dTreeVisitor
{
public:
    CarVisitor(const std::string& b3dId,
               Ogre::SceneManager* sceneManager,
               Ogre::SceneNode* rootNode,
               Ogre::MeshManager* meshManager,
               B3dRenderableObjectList& renderables)
        : B3dTreeVisitor(b3dId, sceneManager, rootNode, meshManager, renderables)
    {
    }

#if 0
    virtual VisitResult Visit(resource::data::b3d::NodeSimplePortal30& node, VisitMode visitMode) override
    {
        if (visitMode == VisitMode::PreOrder)
        {
            m_portals.push_back(node.GetBlockData());
        }

        return VisitResult::Continue;
    }

private:
    block_data::Portals& m_portals;
#endif //0
};

B3dCar::B3dCar(const std::string& b3dId,
               const resource::data::b3d::NodePtr& b3dNode,
               Ogre::SceneManager* sceneManager,
               Ogre::MeshManager* meshManager,
               Ogre::SceneNode* rootSceneNode)
    : m_b3dNode(b3dNode)
    , m_renderables()
    , m_sceneManager(sceneManager)
    , m_meshManager(meshManager)
    , m_rootSceneNode(rootSceneNode)
{
    CarVisitor visitor{b3dId, sceneManager, rootSceneNode, meshManager, m_renderables};
    auto visitResult = VisitNode(m_b3dNode, visitor);
    (void)visitResult;
}

} // namespace app
} // namespace d2_hack
