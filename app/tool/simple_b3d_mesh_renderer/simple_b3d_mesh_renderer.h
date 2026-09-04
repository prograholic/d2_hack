#ifndef D2_HACK_APP_TOOL_SIMPLE_B3D_MESH_RENDERER_SIMPLE_B3D_MESH_RENDERER_H
#define D2_HACK_APP_TOOL_SIMPLE_B3D_MESH_RENDERER_SIMPLE_B3D_MESH_RENDERER_H

#include <d2_hack/app/base_b3d_application.h>

namespace d2_hack
{
namespace app
{

class SimpleB3dMeshRenderer : public BaseB3dApplication
{
public:
    SimpleB3dMeshRenderer();

    virtual void CreateScene() override;

    virtual bool keyPressed(const OgreBites::KeyboardEvent& evt) override;

    virtual void shutdown() override;

private:
    std::vector<B3dRoomPtr> m_rooms;

    virtual void CreateRooms(const resource::data::b3d::B3dForest& forest, Ogre::SceneNode* b3dSceneNode) override;
    virtual void CreateMoveableObjects(const resource::data::b3d::B3dForest& forest, Ogre::SceneNode* b3dSceneNode) override;

    void ApplyRoomEvent(const char* eventName, size_t eventId);
};


extern const char* node_name;

} // namespace app
} // namespace d2_hack

#endif /* D2_HACK_APP_TOOL_SIMPLE_TXR_RENDERER_SIMPLE_TXR_RENDERER_H */
