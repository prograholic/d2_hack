#include "b3d_road.h"

#include <d2_hack/common/log.h>
#include <d2_hack/common/utils.h>

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

B3dRoad::B3dRoad(const std::string& b3dId,
                 const resource::data::b3d::NodePtr& b3dNode,
                 Ogre::SceneManager* sceneManager,
                 Ogre::MeshManager* meshManager,
                 Ogre::SceneNode* rootSceneNode)
    : m_b3dNode(b3dNode)
    , m_renderables()
{
    auto hitB3dNode = b3dNode->ExtractFirstNodeWithCategory(NodeCategory::RoadHitNode);
    if (hitB3dNode)
    {
        m_hit = std::make_unique<B3dHit>(b3dId, hitB3dNode, sceneManager, meshManager, rootSceneNode);
    }

    if (!m_b3dNode->GetChildNodeList().empty())
    {
        RoadVisitor visitor{b3dId, sceneManager, rootSceneNode, meshManager, m_terrain, m_renderables};

        auto visitResult = VisitNode(m_b3dNode, visitor);
        (void)visitResult;
    }
}

} // namespace app
} // namespace d2_hack
