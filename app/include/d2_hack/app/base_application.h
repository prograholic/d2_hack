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

#include <d2_hack/resource/archive/res.h>
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

#if 0    

    virtual ~BaseApplication();

    virtual void go();

    // Ogre::FrameListener


protected:
    virtual bool setup();
    virtual bool configure();
    virtual void chooseSceneManager(void);
    virtual void createCamera(void);
    virtual void createFrameListener(void);
    virtual void createScene(void) = 0; // Override me!
    virtual void destroyScene(void);
    virtual void createViewports(void);
    virtual void setupResources(void);
    virtual void createResourceListener(void);
    virtual void loadResources(void);

    // OIS::KeyListener
    virtual bool keyPressed(const OIS::KeyEvent &arg);
    virtual bool keyReleased(const OIS::KeyEvent &arg);
    // OIS::MouseListener
    virtual bool mouseMoved(const OIS::MouseEvent &arg);
    virtual bool mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
    virtual bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id);

    // Ogre::WindowEventListener
    //Adjust mouse clipping area
    virtual void windowResized(Ogre::RenderWindow* rw);
    //Unattach OIS before window shutdown (very important under Linux)
    virtual void windowClosed(Ogre::RenderWindow* rw);


    Ogre::Root *mRoot;
    Ogre::Camera* mCamera;
    Ogre::SceneManager* mSceneMgr;
    Ogre::RenderWindow* mWindow;
    Ogre::String mResourcesCfg;
    Ogre::String mPluginsCfg;

    // OgreBites
    OgreBites::TrayManager* mTrayMgr;
    OgreBites::CameraMan* mCameraMan;       // basic camera controller
    OgreBites::ParamsPanel* mDetailsPanel;     // sample details panel
    Ogre::OverlaySystem* mOverlaySystem;
    bool mCursorWasVisible;                    // was cursor visible before dialog appeared
    bool mShutDown;



#endif//0
};

} // namespace app
} // namespace d2_hack

#endif // D2_HACK_APP_INCLUDE_D2_HACK_APP_BASE_APPLICATION_H
