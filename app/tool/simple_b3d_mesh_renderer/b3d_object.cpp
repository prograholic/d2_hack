#include "b3d_object.h"

#include "b3d_tree_visitor.h"

namespace d2_hack
{
namespace app
{

using namespace resource::data::b3d;

#if 0
class ObjectVisitor : public B3dTreeVisitor
{
public:
    ObjectVisitor(const std::string& b3dId,
                  Ogre::SceneManager* sceneManager,
                  Ogre::SceneNode* rootNode,
                  Ogre::MeshManager* meshManager,
                  B3dRenderableObjectList& renderables,
                  B3dRoadGroupList& roadGroupList)
        : B3dTreeVisitor(b3dId, sceneManager, rootNode, meshManager)
        , m_renderables(renderables)
        , m_roadGroupList(roadGroupList)
    {
    }

    virtual VisitResult Visit(NodeGroupRoadInfraObjects4& node, VisitMode /* visitMode */) override
    {
        m_roadGroupList.emplace_back(std::make_unique<B3dRoadGroup>(GetB3dId(), node.shared_from_this(), GetSceneManager(), GetMeshManager(), GetCurrentSceneNode()));
        return VisitResult::SkipChildrenAndPostOrder;
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
    B3dRoadGroupList& m_roadGroupList;
};

#endif //0

B3dObject::B3dObject(const B3dNodePtr& b3dNode, B3dSceneBuilder& sceneBuilder)
    : m_renderables()
    , m_roadGroupList()
{
    B3dTreeVisitor visitor{sceneBuilder};

    auto visitResult = VisitNode(b3dNode, visitor);
    (void)visitResult;
}

} // namespace app
} // namespace d2_hack
