#ifndef D2_HACK_BASE_APPLICATION_H
#define D2_HACK_BASE_APPLICATION_H

#include <Bites/OgreApplicationContext.h>
#include <Bites/OgreTrays.h>
#include <Bites/OgreCameraMan.h>

#include "d2_res_archive.h"
#include "txr_image_codec.h"
#include "raw_image_codec.h"

class BaseApplication : public OgreBites::ApplicationContext, public OgreBites::InputListener
{
public:
    BaseApplication();


protected:
    void setup();
#if 0    
    
    virtual ~BaseApplication();

    virtual void go();

    // Ogre::FrameListener
    virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);

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
    virtual bool keyPressed( const OIS::KeyEvent &arg );
    virtual bool keyReleased( const OIS::KeyEvent &arg );
    // OIS::MouseListener
    virtual bool mouseMoved( const OIS::MouseEvent &arg );
    virtual bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
    virtual bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );

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

    /// Custom resource IO
    boost::scoped_ptr<D2ResArchiveFactory> mD2ResArchiveFactory;

    boost::scoped_ptr<TxrImageCodec> mTxrImageCodec;
    boost::scoped_ptr<RawImageCodec> m_rawImageCodec;
    
#endif//0
};

#endif // D2_HACK_BASE_APPLICATION_H
