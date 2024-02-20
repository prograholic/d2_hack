#ifndef D2_HACK_APP_TOOL_SIMPLE_B3D_MESH_RENDERER_B3D_HIT_H
#define D2_HACK_APP_TOOL_SIMPLE_B3D_MESH_RENDERER_B3D_HIT_H

#include <d2_hack/common/platform.h>

#include <memory>

#include <d2_hack/resource/data/b3d_node.h>

namespace d2_hack
{
namespace app
{

class B3dHit
{
public:
    B3dHit(const std::string& b3dId,
            const resource::data::b3d::NodePtr& b3dNode,
            Ogre::SceneManager* sceneManager,
            Ogre::MeshManager* meshManager,
            Ogre::SceneNode* rootSceneNode);

private:
    resource::data::b3d::NodePtr m_b3dNode;
};

typedef std::unique_ptr<B3dHit> B3dHitPtr;

} // namespace app
} // namespace d2_hack

#endif /* D2_HACK_APP_TOOL_SIMPLE_B3D_MESH_RENDERER_B3D_HIT_H */
