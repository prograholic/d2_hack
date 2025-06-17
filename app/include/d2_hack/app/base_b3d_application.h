#ifndef D2_HACK_APP_INCLUDE_D2_HACK_APP_BASE_B3D_APPLICATION_H
#define D2_HACK_APP_INCLUDE_D2_HACK_APP_BASE_B3D_APPLICATION_H

#include <d2_hack/app/base_application.h>
#include <d2_hack/app/b3d_car.h>
#include <d2_hack/app/b3d_room.h>

namespace d2_hack
{
namespace app
{

class BaseB3dApplication : public BaseApplication
{
public:
    explicit BaseB3dApplication(const std::string& appName);


public:

    void CreateB3dScene(const resource::data::b3d::B3dRegistry& b3dRegistry, const resource::data::b3d::CarNameList& carNames, Ogre::SceneNode* b3dSceneNode);

    void ProcessCameraMovement();

    virtual bool keyPressed(const OgreBites::KeyboardEvent& evt) override;

private:
    std::vector<B3dCarPtr> m_cars;
    std::vector<B3dRoomPtr> m_rooms;

    scene_node::WorldContext m_worldContext;

    void PrintNodesStats(const char* prefix);

    void CreateRoomNodes(const resource::data::b3d::B3dTree& tree, Ogre::SceneNode* b3dSceneNode);

    void CreateCarNodes(const resource::data::b3d::B3dTree& tree, const resource::data::b3d::CarNameList& carNames, Ogre::SceneNode* b3dSceneNode);


    void CreateCarNode(const resource::data::b3d::B3dTree& tree, const std::string_view& carName, const Ogre::Vector3& location, Ogre::SceneNode* b3dSceneNode);

    void OnCameraMoved(const scene_node::WorldContext& worldContext, const Ogre::Vector3f& movement);
};

} // namespace app
} // namespace d2_hack

#endif // D2_HACK_APP_INCLUDE_D2_HACK_APP_BASE_B3D_APPLICATION_H
