#include "b3d_car.h"

#include "b3d_tree_visitor.h"

namespace d2_hack
{
namespace app
{

using namespace resource::data::b3d;

class CarVisitor : public RaiseExceptionVisitor, private B3dSceneBuilder
{
public:
    CarVisitor(const std::string& b3dId,
               Ogre::SceneManager* sceneManager,
               Ogre::SceneNode* rootNode,
               Ogre::MeshManager* meshManager,
               B3dRenderableObjectList& renderables,
               block_data::SimpleUnknown14& unknown14)
        : RaiseExceptionVisitor()
        , B3dSceneBuilder(b3dId, sceneManager, rootNode, meshManager)
        , m_renderables(renderables)
        , m_unknown14(unknown14)
    {
    }

    virtual VisitResult Visit(NodeGroupObjects5& /* node */, VisitMode /* visitMode */) override
    {
        return VisitResult::Continue; // unused node
    }

    virtual VisitResult Visit(NodeGroupVertexData7& node, VisitMode /* visitMode */) override
    {
        m_renderables.emplace_back(std::make_unique<B3dRenderableObject>(GetB3dId(), node.shared_from_this(), GetSceneManager(), GetMeshManager(), GetCurrentSceneNode()));
        return VisitResult::SkipChildrenAndPostOrder;
    }

    virtual VisitResult Visit(NodeGroupLodParameters10& /* node */, VisitMode /* visitMode */) override
    {
        return VisitResult::Continue; // TODO: process 10 block
    }

    virtual VisitResult Visit(NodeSimpleUnknown14& node, VisitMode /* visitMode */) override
    {
        m_unknown14 = node.GetBlockData();
        return VisitResult::SkipChildrenAndPostOrder;
    }

    virtual VisitResult Visit(NodeSimpleObjectConnector18& node, VisitMode visitMode) override
    {
        ProcessObjectConnector(node, visitMode);

        return VisitResult::Continue;
    }

    virtual VisitResult Visit(NodeGroupObjects21& /* node */, VisitMode /* visitMode */) override
    {
        return VisitResult::Continue; // TODO: process 21 block
    }

    virtual VisitResult Visit(NodeGroupObjects19& node, VisitMode /* visitMode */) override
    {
        assert(!node.HasParent()); // should be top-level node
        (void)node;
        
        return VisitResult::Continue;
    }

    virtual VisitResult Visit(NodeGroupVertexData36& node, VisitMode /* visitMode */) override
    {
        m_renderables.emplace_back(std::make_unique<B3dRenderableObject>(GetB3dId(), node.shared_from_this(), GetSceneManager(), GetMeshManager(), GetCurrentSceneNode()));
        return VisitResult::SkipChildrenAndPostOrder;
    }

    virtual VisitResult Visit(NodeGroupVertexData37& node, VisitMode /* visitMode */) override
    {
        m_renderables.emplace_back(std::make_unique<B3dRenderableObject>(GetB3dId(), node.shared_from_this(), GetSceneManager(), GetMeshManager(), GetCurrentSceneNode()));
        return VisitResult::SkipChildrenAndPostOrder;
    }

private:
    B3dRenderableObjectList& m_renderables;
    block_data::SimpleUnknown14& m_unknown14;
};

B3dCar::B3dCar(const std::string& b3dId,
               const resource::data::b3d::NodePtr& b3dNode,
               Ogre::SceneManager* sceneManager,
               Ogre::MeshManager* meshManager,
               Ogre::SceneNode* rootSceneNode)
    : m_b3dNode(b3dNode)
    , m_renderables()
    , m_unknown14{}
    , m_sceneManager(sceneManager)
    , m_meshManager(meshManager)
    , m_rootSceneNode(rootSceneNode)
{
    CarVisitor visitor{b3dId, sceneManager, rootSceneNode, meshManager, m_renderables, m_unknown14};
    auto visitResult = VisitNode(m_b3dNode, visitor);
    (void)visitResult;
}

} // namespace app
} // namespace d2_hack
