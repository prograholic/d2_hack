#ifndef D2_HACK_APP_INCLUDE_D2_HACK_APP_B3D_ROOM_H
#define D2_HACK_APP_INCLUDE_D2_HACK_APP_B3D_ROOM_H

#include <d2_hack/app/base_game_object.h>

namespace d2_hack
{
namespace app
{


class B3dRoom : public BaseGameObject
{
public:
    B3dRoom(const resource::data::b3d::B3dNodePtr& b3dNode,
            const std::string_view& b3dId,
            Ogre::SceneManager* sceneManager,
            Ogre::SceneNode* rootNode,
            Ogre::MeshManager* meshManager,
            resource::archive::res::OgreMaterialProvider* ogreMaterialProvider);
};
typedef std::unique_ptr<B3dRoom> B3dRoomPtr;

} // namespace app
} // namespace d2_hack

#endif /* D2_HACK_APP_INCLUDE_D2_HACK_APP_B3D_ROOM_H */
