#ifndef D2_HACK_APP_TOOL_SIMPLE_B3D_MESH_RENDERER_B3D_OBJECT_H
#define D2_HACK_APP_TOOL_SIMPLE_B3D_MESH_RENDERER_B3D_OBJECT_H

#include <d2_hack/common/platform.h>

#include <memory>

#include <OgreSceneNode.h>

#include <d2_hack/resource/data/b3d_node.h>

#include "b3d_renderable_object.h"
#include "b3d_road_group.h"

namespace d2_hack
{
namespace app
{

class B3dObject
{
public:
    B3dObject(const std::string& b3dId,
              const resource::data::b3d::NodePtr& b3dNode,
              Ogre::SceneManager* sceneManager,
              Ogre::MeshManager* meshManager,
              Ogre::SceneNode* rootSceneNode);

private:
    resource::data::b3d::NodePtr m_b3dNode;

    B3dRenderableObjectList m_renderables;
    B3dRoadGroupList m_roadGroupList;
};

typedef std::unique_ptr<B3dObject> B3dObjectPtr;

} // namespace app
} // namespace d2_hack

#endif /* D2_HACK_APP_TOOL_SIMPLE_B3D_MESH_RENDERER_B3D_OBJECT_H */
