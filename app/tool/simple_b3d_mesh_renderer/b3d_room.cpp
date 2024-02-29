#include "b3d_room.h"

#include "b3d_tree_visitor.h"

namespace d2_hack
{
namespace app
{

using namespace resource::data::b3d;

class RoomVisitor : public B3dTreeVisitor
{
public:
    RoomVisitor(const std::string& b3dId,
                Ogre::SceneManager* sceneManager,
                Ogre::SceneNode* rootNode,
                Ogre::MeshManager* meshManager,
                block_data::Portals& portals,
                B3dRenderableObjectList& renderables)
        : B3dTreeVisitor(b3dId, sceneManager, rootNode, meshManager, renderables)
        , m_portals(portals)
    {
    }

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
};

B3dRoom::B3dRoom(const std::string& b3dId,
                 const resource::data::b3d::NodePtr& b3dNode,
                 Ogre::SceneManager* sceneManager,
                 Ogre::MeshManager* meshManager,
                 Ogre::SceneNode* rootSceneNode)
    : m_b3dNode(b3dNode)
    , m_portals()
    , m_road()
    , m_renderables()
    , m_sceneManager(sceneManager)
    , m_meshManager(meshManager)
    , m_rootSceneNode(rootSceneNode)
{
    auto roadB3dNode = b3dNode->ExtractFirstNodeWithCategory(NodeCategory::RoadNode);
    if (roadB3dNode)
    {
        m_road = std::make_unique<B3dRoad>(b3dId, roadB3dNode, m_sceneManager, m_meshManager, rootSceneNode);
    }
    
    //auto objB3dNode = ExtractRoadNodeWithPrefix(b3dNode, "obj_" + b3dId + "_");
    //m_obj = std::make_unique<B3dObject????

    RoomVisitor visitor{b3dId, m_sceneManager, rootSceneNode, m_meshManager, m_portals, m_renderables};

    auto visitResult = VisitNode(m_b3dNode, visitor);
    (void)visitResult;
}

void B3dRoom::GetGasStations() const
{
}

void B3dRoom::GetServiceStations() const
{
}

const block_data::Portals& B3dRoom::GetPortals() const
{
    return m_portals;
}

B3dRoad* B3dRoom::GetRoad()
{
    return m_road.get();
}


} // namespace app
} // namespace d2_hack
