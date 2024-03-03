#include "b3d_hit.h"

#include "b3d_tree_visitor.h"

namespace d2_hack
{
namespace app
{

using namespace resource::data::b3d;

#if 0

class B3dHitVisitor : public B3dTreeVisitor
{
public:
    B3dHitVisitor(const std::string& b3dId,
                  Ogre::SceneManager* sceneManager,
                  Ogre::SceneNode* rootNode,
                  Ogre::MeshManager* meshManager,
                  B3dRenderableObjectList& renderables)
        : B3dTreeVisitor(b3dId, sceneManager, rootNode, meshManager)
        , m_renderables(renderables)
    {
    }

    virtual VisitResult Visit(NodeGroupRoadInfraObjects4& node, VisitMode /* visitMode */) override
    {
        RaiseExceptionAction::RaiseException(node.GetName().c_str(), node.GetType());
        return VisitResult::Stop;
    }

    virtual VisitResult Visit(NodeGroupVertexData7& node, VisitMode /* visitMode */) override
    {
        m_renderables.emplace_back(std::make_unique<B3dRenderableObject>(GetB3dId(), node.shared_from_this(), GetSceneManager(), GetMeshManager(), GetCurrentSceneNode()));
        return VisitResult::SkipChildrenAndPostOrder;
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
};


#endif //0

B3dHit::B3dHit(const B3dNodePtr& b3dNode,
               B3dSceneBuilder& sceneBuilder)
    : m_renderables()
{
    B3dTreeVisitor visitor{sceneBuilder};

    auto visitResult = VisitNode(b3dNode, visitor);
    (void)visitResult;
}


} // namespace app
} // namespace d2_hack
