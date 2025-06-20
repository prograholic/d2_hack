#ifndef D2_HACK_APP_INCLUDE_D2_HACK_APP_BASE_GAME_OBJECT_H
#define D2_HACK_APP_INCLUDE_D2_HACK_APP_BASE_GAME_OBJECT_H

#include <d2_hack/common/platform.h>

#include <memory>

#include <d2_hack/resource/data/b3d_node.h>
#include <d2_hack/scene_node/scene_node_base.h>
#include <d2_hack/resource/archive/ogre_material_provider.h>

namespace d2_hack
{
namespace app
{

class BaseGameObject
{
public:
    BaseGameObject(const resource::data::b3d::B3dNodePtr& b3dNode,
                   const std::string_view& b3dId,
                   Ogre::SceneManager* sceneManager,
                   Ogre::SceneNode* rootNode,
                   Ogre::MeshManager* meshManager,
                   resource::archive::res::OgreMaterialProvider* ogreMaterialProvider);

    virtual ~BaseGameObject() = default;


    void OnCameraMoved(const scene_node::WorldContext& worldContext, const Ogre::Vector3f& movement);

private:
    scene_node::SceneNodeBaseList m_rootNodes;
};
typedef std::unique_ptr<BaseGameObject> BaseGameObjectPtr;

} // namespace app
} // namespace d2_hack

#endif /* D2_HACK_APP_INCLUDE_D2_HACK_APP_BASE_GAME_OBJECT_H */
