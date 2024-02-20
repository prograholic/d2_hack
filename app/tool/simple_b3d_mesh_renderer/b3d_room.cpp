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
                block_data::Portals& portals)
        : B3dTreeVisitor(b3dId, sceneManager, rootNode, meshManager)
        , m_portals(portals)
    {
    }

    virtual void Visit(resource::data::b3d::NodeSimplePortal30& node, VisitMode visitMode) override
    {
        if (visitMode == VisitMode::PreOrder)
        {
            m_portals.push_back(node.GetBlockData());
        }
    }

private:
    block_data::Portals& m_portals;
};

static NodePtr ExtractRoadNodeWithPrefix(const resource::data::b3d::NodePtr& b3dNode, const std::string& prefix)
{
    NodeList& childNodes = b3dNode->GetChildNodeList();
    auto pos = childNodes.begin();
    while (pos != childNodes.end())
    {
        if ((*pos)->GetName().starts_with(prefix))
        {
            break;
        }
        ++pos;
    }

    if (pos == childNodes.end())
    {
        OGRE_EXCEPT(Ogre::Exception::ERR_INVALID_STATE, "cannot find node with prefix " + prefix + ", node name: " + b3dNode->GetName(), "ExtractRoadNodeWithPrefix");
    }

    NodePtr res = *pos;
    childNodes.erase(pos);

    return res;
}

B3dRoom::B3dRoom(const std::string& b3dId,
                 const resource::data::b3d::NodePtr& b3dNode,
                 Ogre::SceneManager* sceneManager,
                 Ogre::MeshManager* meshManager,
                 Ogre::SceneNode* rootSceneNode)
    : m_b3dNode(b3dNode)
    , m_portals()
    , m_sceneManager(sceneManager)
    , m_meshManager(meshManager)
    , m_rootSceneNode(rootSceneNode)
{
    auto roadB3dNode = ExtractRoadNodeWithPrefix(b3dNode, "road_" + b3dId + "_");
    m_road = std::make_unique<B3dRoad>(b3dId, roadB3dNode, m_sceneManager, m_meshManager, rootSceneNode);

    //auto objB3dNode = ExtractRoadNodeWithPrefix(b3dNode, "obj_" + b3dId + "_");
    //m_obj = std::make_unique<B3dObject????

    RoomVisitor visitor{b3dId, m_sceneManager, rootSceneNode, m_meshManager, m_portals};

    VisitNode(m_b3dNode, visitor);
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


} // namespace app
} // namespace d2_hack