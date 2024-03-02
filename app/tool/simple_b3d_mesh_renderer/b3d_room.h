#ifndef D2_HACK_APP_TOOL_SIMPLE_B3D_MESH_RENDERER_B3D_ROOM_H
#define D2_HACK_APP_TOOL_SIMPLE_B3D_MESH_RENDERER_B3D_ROOM_H

#include <d2_hack/common/platform.h>

#include <memory>

#include <OgreSceneNode.h>

#include <d2_hack/resource/data/b3d_node.h>

#include "b3d_road.h"
#include "b3d_object.h"
#include "b3d_road_group.h"

namespace d2_hack
{
namespace app
{

class B3dRoom
{
public:
    B3dRoom(const std::string& b3dId,
            const resource::data::b3d::NodePtr& b3dNode,
            Ogre::SceneManager* sceneManager,
            Ogre::MeshManager* meshManager,
            Ogre::SceneNode* rootSceneNode);

    void GetGasStations() const;

    void GetServiceStations() const;

    const resource::data::b3d::block_data::Portals& GetPortals() const;

    B3dRoad* GetRoad();

private:
    resource::data::b3d::NodePtr m_b3dNode;
    resource::data::b3d::block_data::Portals m_portals;
    B3dRoadPtr m_road;
    B3dObjectPtr m_obj;
    B3dRenderableObjectList m_renderables;
    B3dRoadGroupList m_roadGroupList;


    Ogre::SceneManager* m_sceneManager;
    Ogre::MeshManager* m_meshManager;
    Ogre::SceneNode* m_rootSceneNode;
};

typedef std::unique_ptr<B3dRoom> B3dRoomPtr;

} // namespace app
} // namespace d2_hack

#endif /* D2_HACK_APP_TOOL_SIMPLE_B3D_MESH_RENDERER_B3D_ROOM_H */
