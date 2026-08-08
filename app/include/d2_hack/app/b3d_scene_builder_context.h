#ifndef D2_HACK_APP_B3D_SCENE_BUILDER_CONTEXT_H
#define D2_HACK_APP_B3D_SCENE_BUILDER_CONTEXT_H

#include <d2_hack/common/platform.h>

#include <string_view>

#include <OgreSceneManager.h>
#include <OgreSceneNode.h>
#include <OgreMeshManager.h>

#include <d2_hack/resource/archive/ogre_material_provider.h>

namespace d2_hack
{
namespace app
{

struct B3dSceneBuilderContext
{
    Ogre::SceneManager* sceneManager;
    Ogre::SceneNode* ogreRootNode;
    Ogre::MeshManager* meshManager;
    resource::archive::res::OgreMaterialProvider* ogreMaterialProvider;
};


} // namespace app
} // namespace d2_hack

#endif /* D2_HACK_APP_B3D_SCENE_BUILDER_CONTEXT_H */
