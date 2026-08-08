#ifndef D2_HACK_APP_TOOL_SIMPLE_B3D_CAR_RENDERER_SIMPLE_B3D_CAR_RENDERER_H
#define D2_HACK_APP_TOOL_SIMPLE_B3D_CAR_RENDERER_SIMPLE_B3D_CAR_RENDERER_H

#include <d2_hack/app/base_b3d_application.h>

namespace d2_hack
{
namespace app
{

class SimpleB3dCarRenderer : public BaseB3dApplication
{
public:
    SimpleB3dCarRenderer();

    virtual void CreateScene() override;

    virtual bool keyPressed(const OgreBites::KeyboardEvent& evt) override;

    virtual void shutdown() override;

private:
    std::vector<MoveableObjectPtr> m_moveableObjects;

    virtual void CreateRooms(const resource::data::b3d::B3dForest& forest, Ogre::SceneNode* b3dSceneNode) override;
    virtual void CreateMoveableObjects(const resource::data::b3d::B3dForest& forest, Ogre::SceneNode* b3dSceneNode) override;
    virtual void OnCameraMoved(const scene_node::WorldContext& worldContext, const Ogre::Vector3f& movement) override;
};


extern const char* node_name;

} // namespace app
} // namespace d2_hack

#endif /* D2_HACK_APP_TOOL_SIMPLE_B3D_CAR_RENDERER_SIMPLE_B3D_CAR_RENDERER_H */
