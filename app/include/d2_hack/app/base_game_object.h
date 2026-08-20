#ifndef D2_HACK_APP_INCLUDE_D2_HACK_APP_BASE_GAME_OBJECT_H
#define D2_HACK_APP_INCLUDE_D2_HACK_APP_BASE_GAME_OBJECT_H

#include <d2_hack/common/platform.h>

#include <memory>

#include <d2_hack/scene_node/scene_node_base.h>


namespace d2_hack
{
namespace app
{

class BaseGameObject
{
public:
    explicit BaseGameObject(scene_node::SceneNodeBaseList rootNodes);

    virtual ~BaseGameObject() = default;

    const scene_node::SceneNodeBaseList& GetRootNodes() const;

protected:
    scene_node::SceneNodeBaseList m_rootNodes;
};
typedef std::unique_ptr<BaseGameObject> BaseGameObjectPtr;

} // namespace app
} // namespace d2_hack

#endif /* D2_HACK_APP_INCLUDE_D2_HACK_APP_BASE_GAME_OBJECT_H */
