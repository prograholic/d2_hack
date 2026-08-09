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

    void CreateB3dScene(const resource::data::b3d::B3dRegistry& b3dRegistry, Ogre::SceneNode* b3dSceneNode);

    B3dRoomPtr CreateRoom(const resource::data::b3d::B3dForest& forest, const std::string_view& roomId, Ogre::SceneNode* b3dSceneNode);

    MoveableObjectPtr CreateMoveableObject(const resource::data::b3d::B3dForest& forest, const std::string_view& movObjId, const Ogre::Vector3& location, Ogre::SceneNode* b3dSceneNode);

    void ProcessCameraMovement();

    virtual bool keyPressed(const OgreBites::KeyboardEvent& evt) override;

private:
    scene_node::WorldContext m_worldContext;

    void PrintNodesStats(const char* prefix);

    virtual void CreateRooms(const resource::data::b3d::B3dForest& forest, Ogre::SceneNode* b3dSceneNode) = 0;
    virtual void CreateMoveableObjects(const resource::data::b3d::B3dForest& forest, Ogre::SceneNode* b3dSceneNode) = 0;

    virtual void OnCameraMoved(const scene_node::WorldContext& worldContext, const Ogre::Vector3f& movement) = 0;

    B3dCarPtr CreateCar(std::string_view b3dId, const resource::data::b3d::B3dNodePtr& moveableObject, Ogre::SceneNode* moveableSceneNode);
    B3dTruckPtr CreateTruck(std::string_view b3dId, const resource::data::b3d::B3dNodePtr& moveableObject, Ogre::SceneNode* moveableSceneNode);
    B3dSemiTrailerPtr CreateSemiTrailer(std::string_view b3dId, const resource::data::b3d::B3dNodePtr& moveableObject, Ogre::SceneNode* moveableSceneNode);
    MoveableObjectPtr CreateCustomMoveableObject(std::string_view b3dId, const resource::data::b3d::B3dNodePtr& moveableObject, Ogre::SceneNode* moveableSceneNode);
};

} // namespace app
} // namespace d2_hack

#endif // D2_HACK_APP_INCLUDE_D2_HACK_APP_BASE_B3D_APPLICATION_H
