#ifndef D2_HACK_APP_TOOL_SIMPLE_B3D_MESH_RENDERER_B3D_ROOM_H
#define D2_HACK_APP_TOOL_SIMPLE_B3D_MESH_RENDERER_B3D_ROOM_H

#include <d2_hack/common/platform.h>

#include <memory>

#include <d2_hack/resource/data/b3d_node.h>
#include <d2_hack/scene_node/scene_node_base.h>

namespace d2_hack
{
namespace app
{


class B3dRoom
{
public:
    B3dRoom(const resource::data::b3d::B3dNodePtr& b3dNode,
           const std::string& b3dId,
           Ogre::SceneManager* sceneManager,
           Ogre::SceneNode* rootNode,
           Ogre::MeshManager* meshManager);


    void Activate(const scene_node::WorldContext& worldContext);

private:
    scene_node::SceneNodeBaseList m_rootNodes;
};
typedef std::unique_ptr<B3dRoom> B3dRoomPtr;

} // namespace app
} // namespace d2_hack

#endif /* D2_HACK_APP_TOOL_SIMPLE_B3D_MESH_RENDERER_B3D_ROOM_H */
