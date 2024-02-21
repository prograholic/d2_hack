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
                TerrainPtr& terrain)
        : B3dTreeVisitor(b3dId, sceneManager, rootNode, meshManager)
        , m_terrain(terrain)
        , m_sceneManager(sceneManager)
    {
    }

    virtual void Visit(NodeSimpleGeneratedObjects40& node, VisitMode visitMode)
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
                D2_HACK_LOG(Visit) << "unsupported generator `" << generatorName << "`";
            }
        }
    }
private:
    TerrainPtr& m_terrain;
    Ogre::SceneManager* m_sceneManager;
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
        RoadVisitor visitor{b3dId, sceneManager, rootSceneNode, meshManager, m_terrain};

        VisitNode(m_b3dNode, visitor);
    }
}

Terrain* B3dRoad::GetTerrain()
{
    return m_terrain.get();
}


} // namespace app
} // namespace d2_hack
