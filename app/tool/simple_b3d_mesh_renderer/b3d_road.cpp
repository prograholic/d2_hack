#include "b3d_road.h"

#include <d2_hack/common/log.h>
#include <d2_hack/common/utils.h>

#include "b3d_tree_visitor.h"


namespace d2_hack
{
namespace app
{

using namespace resource::data::b3d;

#if 0
class RoadVisitor : public B3dTreeVisitor
{
public:
    RoadVisitor(const std::string& b3dId,
                Ogre::SceneManager* sceneManager,
                Ogre::SceneNode* rootNode,
                Ogre::MeshManager* meshManager,
                TerrainPtr& terrain,
                B3dRenderableObjectList& renderables)
        : B3dTreeVisitor(b3dId, sceneManager, rootNode, meshManager)
        , m_terrain(terrain)
        , m_renderables(renderables)
        , m_sceneManager(sceneManager)
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

    virtual VisitResult Visit(NodeSimpleGeneratedObjects40& node, VisitMode visitMode) override
    {
        if (visitMode == VisitMode::PreOrder)
        {
            auto generatorName = common::ResourceNameToString(node.GetBlockData().name);
            if (generatorName == "$$GeneratorOfTerrain")
            {
                m_terrain = std::make_unique<Terrain>(m_sceneManager);
            }
            else
            {
                OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS, "Unsupported generator `" + generatorName + "`, node: " + node.GetName());
            }
        }

        return VisitResult::Continue;
    }

private:
    TerrainPtr& m_terrain;
    B3dRenderableObjectList& m_renderables;
    Ogre::SceneManager* m_sceneManager;
};

#endif //0

B3dRoad::B3dRoad(const B3dNodePtr& b3dNode, B3dSceneBuilder& sceneBuilder)
    : m_renderables()
{
    auto hitB3dNode = b3dNode->ExtractFirstNodeWithCategory(NodeCategory::RoadHitNode);
    if (hitB3dNode)
    {
        m_hit = std::make_unique<B3dHit>(hitB3dNode, sceneBuilder);
    }

    if (!b3dNode->GetChildNodeList().empty())
    {
        B3dTreeVisitor visitor{sceneBuilder};

        auto visitResult = VisitNode(b3dNode, visitor);
        (void)visitResult;
    }
}

} // namespace app
} // namespace d2_hack
