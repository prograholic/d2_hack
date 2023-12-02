#ifndef D2_HACK_APP_TOOL_SIMPLE_B3D_MESH_RENDERER_SIMPLE_B3D_MESH_RENDERER_H
#define D2_HACK_APP_TOOL_SIMPLE_B3D_MESH_RENDERER_SIMPLE_B3D_MESH_RENDERER_H

#include <d2_hack/app/base_application.h>


namespace d2_hack
{
namespace app
{

class SimpleB3dMeshRenderer : public BaseApplication
{
public:
    SimpleB3dMeshRenderer();

private:
    virtual void CreateScene() override;

    bool keyPressed(const OgreBites::KeyboardEvent& evt) override;
};

} // namespace app
} // namespace d2_hack

#endif /* D2_HACK_APP_TOOL_SIMPLE_TXR_RENDERER_SIMPLE_TXR_RENDERER_H */
