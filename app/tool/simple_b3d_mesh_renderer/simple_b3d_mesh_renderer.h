#ifndef D2_HACK_APP_TOOL_SIMPLE_B3D_MESH_RENDERER_SIMPLE_B3D_MESH_RENDERER_H
#define D2_HACK_APP_TOOL_SIMPLE_B3D_MESH_RENDERER_SIMPLE_B3D_MESH_RENDERER_H

#include <d2_hack/app/base_application.h>

#include <d2_hack/resource/data/b3d_tree.h>

#include "b3d_room.h"
#include "b3d_car.h"

namespace d2_hack
{
namespace app
{

class SimpleB3dMeshRenderer : public BaseApplication
{
public:
    SimpleB3dMeshRenderer();

private:
    std::vector<B3dRoomPtr> m_rooms;
    std::vector<B3dCarPtr> m_cars;

    void CreateRootNodes(const resource::data::b3d::B3dTree& tree, Ogre::SceneNode* b3dSceneNode);

    virtual void CreateScene() override;

    virtual bool keyPressed(const OgreBites::KeyboardEvent& evt) override;

    virtual void shutdown() override;
};


extern const char* node_name;

} // namespace app
} // namespace d2_hack

#endif /* D2_HACK_APP_TOOL_SIMPLE_TXR_RENDERER_SIMPLE_TXR_RENDERER_H */
