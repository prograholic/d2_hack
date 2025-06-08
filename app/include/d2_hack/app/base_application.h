#ifndef D2_HACK_APP_INCLUDE_D2_HACK_APP_BASE_APPLICATION_H
#define D2_HACK_APP_INCLUDE_D2_HACK_APP_BASE_APPLICATION_H

#include <d2_hack/common/platform.h>

#include <memory>

D2_HACK_DISABLE_WARNING_BEGIN(4100)
D2_HACK_DISABLE_WARNING_BEGIN(4275)
D2_HACK_DISABLE_WARNING_BEGIN(4251)
D2_HACK_DISABLE_WARNING_BEGIN(4996)

#include <Bites/OgreApplicationContext.h>
#include <Bites/OgreTrays.h>
#include <Bites/OgreCameraMan.h>
#include <OgreSceneManager.h>

D2_HACK_DISABLE_WARNING_END() // 4996
D2_HACK_DISABLE_WARNING_END() // 4251
D2_HACK_DISABLE_WARNING_END() // 4275
D2_HACK_DISABLE_WARNING_END() // 4100

#include <d2_hack/resource/archive/res_archive.h>
#include <d2_hack/resource/image/raw.h>
#include <d2_hack/resource/image/txr.h>
#include <d2_hack/resource/manager/manager.h>

namespace d2_hack
{
namespace app
{

class BaseApplication : public OgreBites::ApplicationContext, public OgreBites::InputListener
{
public:
    BaseApplication();

protected:
    std::unique_ptr<resource::manager::Manager> m_manager;
    /// Custom resource IO
    std::unique_ptr<resource::archive::res::ResArchive::Factory> m_d2ResArchiveFactory;

    std::unique_ptr<resource::image::TxrImageCodec> m_txrImageCodec;
    std::unique_ptr<resource::image::RawImageCodec> m_rawImageCodec;
    Ogre::SceneManager* m_sceneManager;
    Ogre::SceneNode* m_cameraSceneNode;
    Ogre::Camera* m_camera;
    std::unique_ptr<OgreBites::CameraMan> m_cameraManager;

    bool m_shutdown;

    virtual void setup() override;
    virtual void shutdown() override;

    virtual bool keyPressed(const OgreBites::KeyboardEvent& evt) override;
    virtual bool keyReleased(const OgreBites::KeyboardEvent& evt) override;

    virtual bool mouseMoved(const OgreBites::MouseMotionEvent& evt) override;
    virtual bool mousePressed(const OgreBites::MouseButtonEvent& evt) override;
    virtual bool mouseReleased(const OgreBites::MouseButtonEvent& evt) override;
    virtual bool mouseWheelRolled(const OgreBites::MouseWheelEvent& evt) override;

    virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt) override;


    void CreateCamera();
    void CreateViewports();
    void CreateResourceListener();

    virtual void CreateScene() = 0;
};

} // namespace app
} // namespace d2_hack

#endif // D2_HACK_APP_INCLUDE_D2_HACK_APP_BASE_APPLICATION_H
