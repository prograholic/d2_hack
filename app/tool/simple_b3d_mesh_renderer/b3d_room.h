#ifndef D2_HACK_APP_TOOL_SIMPLE_B3D_MESH_RENDERER_B3D_ROOM_H
#define D2_HACK_APP_TOOL_SIMPLE_B3D_MESH_RENDERER_B3D_ROOM_H

#include <d2_hack/common/platform.h>

#include <OgreSceneNode.h>

#include <d2_hack/resource/data/b3d_node.h>

namespace d2_hack
{
namespace app
{

class B3dRoom
{
public:
    explicit B3dRoom(Ogre::SceneNode* rootNode);



private:
    Ogre::SceneNode* m_rootNode;
};

} // namespace app
} // namespace d2_hack

#endif /* D2_HACK_APP_TOOL_SIMPLE_B3D_MESH_RENDERER_B3D_ROOM_H */
