#ifndef D2_HACK_APP_TOOL_SIMPLE_TXR_RENDERER_SIMPLE_TXR_RENDERER_H
#define D2_HACK_APP_TOOL_SIMPLE_TXR_RENDERER_SIMPLE_TXR_RENDERER_H

#include <d2_hack/app/base_b3d_application.h>

#include <d2_hack/resource/archive/ogre_material_provider.h>

namespace d2_hack
{
namespace app
{

class SimpleTxrRenderer : public BaseB3dApplication
{
public:
    SimpleTxrRenderer();

private:
    virtual void CreateScene() override;

    void CreateEntityForResource(Ogre::SceneNode* parent, const std::string_view& b3dId, const std::string_view& resource);
};

} // namespace app
} // namespace d2_hack

#endif /* D2_HACK_APP_TOOL_SIMPLE_TXR_RENDERER_SIMPLE_TXR_RENDERER_H */
