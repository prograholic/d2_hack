#ifndef D2_HACK_APP_TOOL_SIMPLE_B3D_MESH_RENDERER_B3D_CAR_H
#define D2_HACK_APP_TOOL_SIMPLE_B3D_MESH_RENDERER_B3D_CAR_H

#include <d2_hack/common/platform.h>

#include <memory>

#include <OgreSceneNode.h>

#include <d2_hack/resource/data/b3d_node.h>

#include "b3d_scene_builder.h"
#include "b3d_renderable_object.h"

namespace d2_hack
{
namespace app
{

class B3dCar
{
public:
    B3dCar(const resource::data::b3d::B3dNodePtr& b3dNode,
           const std::string& b3dId,
           Ogre::SceneManager* sceneManager,
           Ogre::SceneNode* rootNode,
           Ogre::MeshManager* meshManager);

private:
    B3dSceneNodeBaseList m_rootB3dNodes;

    B3dRenderableObjectList m_renderables;

    resource::data::b3d::block_data::SimpleUnknown14 m_unknown14;
};

typedef std::unique_ptr<B3dCar> B3dCarPtr;

} // namespace app
} // namespace d2_hack

#endif /* D2_HACK_APP_TOOL_SIMPLE_B3D_MESH_RENDERER_B3D_CAR_H */
