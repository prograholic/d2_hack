#include "base_application.h"

D2_HACK_DISABLE_WARNING_BEGIN(4251)

#include <OgreTextureManager.h>
#include <OgreArchiveManager.h>

D2_HACK_DISABLE_WARNING_END() //4251

#include "d2_hack_common.h"

BaseApplication::BaseApplication()
    : OgreBites::ApplicationContext("d2_hack")
    , m_sceneManager(nullptr)
    , m_cameraSceneNode(nullptr)
    , m_camera(nullptr)
    , m_cameraManager(nullptr)
    , m_shutdown(false)
{
}

void BaseApplication::setup()
{
    m_d2ResArchiveFactory.reset(new D2ResArchiveFactory);
    Ogre::ArchiveManager::getSingleton().addArchiveFactory(m_d2ResArchiveFactory.get());

    m_txrImageCodec.reset(new TxrImageCodec);
    Ogre::Codec::registerCodec(m_txrImageCodec.get());

    m_rawImageCodec.reset(new RawImageCodec);
    Ogre::Codec::registerCodec(m_rawImageCodec.get());

    OgreBites::ApplicationContext::setup();
    addInputListener(this);

    m_sceneManager = mRoot->createSceneManager();

    CreateCamera();

    CreateViewports();

    // Set default mipmap level (NB some APIs ignore this)
    Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);

    // Create any resource listeners (for loading screens)
    CreateResourceListener();

    // Create the scene
    CreateScene();

    //CreateFrameListener();
}

bool BaseApplication::keyPressed(const OgreBites::KeyboardEvent& evt)
{
    if (evt.keysym.sym == OgreBites::SDLK_ESCAPE)
    {
        m_shutdown = true;
    }

    m_cameraManager->keyPressed(evt);

    return true;
}

bool BaseApplication::keyReleased(const OgreBites::KeyboardEvent& evt)
{
    m_cameraManager->keyReleased(evt);

    return true;
}

bool BaseApplication::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
    if (m_shutdown)
    {
        return false;
    }

    m_cameraManager->frameRendered(evt);

    return OgreBites::ApplicationContext::frameRenderingQueued(evt);
}

bool BaseApplication::mouseMoved(const OgreBites::MouseMotionEvent& evt)
{
    m_cameraManager->mouseMoved(evt);
    return true;
}

bool BaseApplication::mousePressed(const OgreBites::MouseButtonEvent& evt)
{
    m_cameraManager->mousePressed(evt);
    return true;
}

bool BaseApplication::mouseReleased(const OgreBites::MouseButtonEvent& evt)
{
    m_cameraManager->mouseReleased(evt);
    return true;
}

bool BaseApplication::mouseWheelRolled(const OgreBites::MouseWheelEvent& evt)
{
    m_cameraManager->mouseWheelRolled(evt);
    return true;
}

void BaseApplication::CreateCamera()
{
    m_camera = m_sceneManager->createCamera("PlayerCam");
    m_camera->setNearClipDistance(5);
    
    m_cameraSceneNode = m_sceneManager->getRootSceneNode()->createChildSceneNode();
    m_cameraSceneNode->attachObject(m_camera);

    m_cameraSceneNode->setPosition(Ogre::Vector3(0, 0, 80));
    m_cameraSceneNode->lookAt(Ogre::Vector3(0, 0, -300), Ogre::Node::TS_LOCAL);

    m_cameraManager.reset(new OgreBites::CameraMan(m_cameraSceneNode));
}

void BaseApplication::CreateViewports()
{
    // Create one viewport, entire window
    Ogre::Viewport* vp = mWindows[0].render->addViewport(m_camera);
    vp->setBackgroundColour(Ogre::ColourValue(0, 0, 0));

    // Alter the camera aspect ratio to match the viewport
    m_camera->setAspectRatio(Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));
}

void BaseApplication::CreateResourceListener()
{
    //
}

#if 0

//-------------------------------------------------------------------------------------
BaseApplication::BaseApplication()
    : OgreBites::ApplicationContext("afafa")
    , mRoot(0),
    mCamera(0),
    mWindow(0),
    mResourcesCfg(Ogre::StringUtil::BLANK),
    mPluginsCfg(Ogre::StringUtil::BLANK),
    mTrayMgr(0),
    mCameraMan(0),
    mDetailsPanel(0),
    mOverlaySystem(0),
    mCursorWasVisible(false),
    mD2ResArchiveFactory(),
    mTxrImageCodec()
{
}

//-------------------------------------------------------------------------------------
BaseApplication::~BaseApplication()
{
    //if (mTrayMgr) delete mTrayMgr;
    //if (mCameraMan) delete mCameraMan;

    //Remove ourself as a Window listener
    Ogre::WindowEventUtilities::removeWindowEventListener(mWindow, this);
    windowClosed(mWindow);

    if (mTxrImageCodec)
    {
      Ogre::Codec::unregisterCodec(mTxrImageCodec.get());
    }

    if (m_rawImageCodec)
    {
      Ogre::Codec::unregisterCodec(m_rawImageCodec.get());
    }

    OGRE_DELETE mOverlaySystem;
    OGRE_DELETE mRoot;
}

//-------------------------------------------------------------------------------------
bool BaseApplication::configure()
{
    // Show the configuration dialog and initialise the system
    // You can skip this and use root.restoreConfig() to load configuration
    // settings if you were sure there are valid ones saved in ogre.cfg
    if(mRoot->showConfigDialog())
    {
        // If returned true, user clicked OK so initialise
        // Here we choose to let the system create a default rendering window by passing 'true'
        mWindow = mRoot->initialise(true, "TutorialApplication Render Window");

        return true;
    }
    else
    {
        return false;
    }
}
//-------------------------------------------------------------------------------------
void BaseApplication::createFrameListener(void)
{
    Ogre::LogManager::getSingletonPtr()->logMessage("*** Initializing OIS ***");
    OIS::ParamList pl;
    size_t windowHnd = 0;
    std::ostringstream windowHndStr;

    mWindow->getCustomAttribute("WINDOW", &windowHnd);
    windowHndStr << windowHnd;
    pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));

    mInputManager = OIS::InputManager::createInputSystem( pl );

    mKeyboard = static_cast<OIS::Keyboard*>(mInputManager->createInputObject( OIS::OISKeyboard, true ));
    mMouse = static_cast<OIS::Mouse*>(mInputManager->createInputObject( OIS::OISMouse, true ));

    mMouse->setEventCallback(this);
    mKeyboard->setEventCallback(this);

    //Set initial mouse clipping size
    windowResized(mWindow);

    //Register as a Window listener
    Ogre::WindowEventUtilities::addWindowEventListener(mWindow, this);

    OgreBites::InputContext inputContext;
    inputContext.mMouse = mMouse;
    inputContext.mKeyboard = mKeyboard;

    mTrayMgr = new OgreBites::SdkTrayManager("InterfaceName", mWindow, inputContext, this);
    mTrayMgr->showFrameStats(OgreBites::TL_BOTTOMLEFT);
    mTrayMgr->showLogo(OgreBites::TL_BOTTOMRIGHT);
    mTrayMgr->hideCursor();

    // create a params panel for displaying sample details
    Ogre::StringVector items;
    items.push_back("cam.pX");
    items.push_back("cam.pY");
    items.push_back("cam.pZ");
    items.push_back("");
    items.push_back("cam.oW");
    items.push_back("cam.oX");
    items.push_back("cam.oY");
    items.push_back("cam.oZ");
    items.push_back("");
    items.push_back("Filtering");
    items.push_back("Poly Mode");

    mDetailsPanel = mTrayMgr->createParamsPanel(OgreBites::TL_NONE, "DetailsPanel", 200, items);
    mDetailsPanel->setParamValue(9, "Bilinear");
    mDetailsPanel->setParamValue(10, "Solid");
    mDetailsPanel->hide();

    mRoot->addFrameListener(this);
}
//-------------------------------------------------------------------------------------
void BaseApplication::destroyScene(void)
{
}
//-------------------------------------------------------------------------------------
void BaseApplication::createViewports(void)
{
    // Create one viewport, entire window
    Ogre::Viewport* vp = mWindow->addViewport(mCamera);
    vp->setBackgroundColour(Ogre::ColourValue(0,0,0));

    // Alter the camera aspect ratio to match the viewport
    mCamera->setAspectRatio(
        Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));
}
//-------------------------------------------------------------------------------------
void BaseApplication::createResourceListener(void)
{

}
//-------------------------------------------------------------------------------------
void BaseApplication::loadResources(void)
{
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
}
//-------------------------------------------------------------------------------------
bool BaseApplication::setup(void)
{
    mRoot = OGRE_NEW Ogre::Root(mPluginsCfg);

    mOverlaySystem = OGRE_NEW Ogre::OverlaySystem();

    mD2ResArchiveFactory.reset(new D2ResArchiveFactory);
    Ogre::ArchiveManager::getSingleton().addArchiveFactory(mD2ResArchiveFactory.get());

    mTxrImageCodec.reset(new TxrImageCodec);
    Ogre::Codec::registerCodec(mTxrImageCodec.get());

    m_rawImageCodec.reset(new RawImageCodec);
    Ogre::Codec::registerCodec(m_rawImageCodec.get());

    setupResources();

    bool carryOn = configure();
    if (!carryOn) return false;

    chooseSceneManager();
    createCamera();
    createViewports();

    // Set default mipmap level (NB some APIs ignore this)
    Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);

    // Create any resource listeners (for loading screens)
    createResourceListener();
    // Load resources
    loadResources();

    // Create the scene
    createScene();

    createFrameListener();

    return true;
};
//-------------------------------------------------------------------------------------
bool BaseApplication::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
    if(mWindow->isClosed())
        return false;

    //Need to capture/update each device
    mKeyboard->capture();
    mMouse->capture();

    mTrayMgr->frameRenderingQueued(evt);

    if (!mTrayMgr->isDialogVisible())
    {
        mCameraMan->frameRenderingQueued(evt);   // if dialog isn't up, then update the camera
        if (mDetailsPanel->isVisible())   // if details panel is visible, then update its contents
        {
            mDetailsPanel->setParamValue(0, Ogre::StringConverter::toString(mCamera->getDerivedPosition().x));
            mDetailsPanel->setParamValue(1, Ogre::StringConverter::toString(mCamera->getDerivedPosition().y));
            mDetailsPanel->setParamValue(2, Ogre::StringConverter::toString(mCamera->getDerivedPosition().z));
            mDetailsPanel->setParamValue(4, Ogre::StringConverter::toString(mCamera->getDerivedOrientation().w));
            mDetailsPanel->setParamValue(5, Ogre::StringConverter::toString(mCamera->getDerivedOrientation().x));
            mDetailsPanel->setParamValue(6, Ogre::StringConverter::toString(mCamera->getDerivedOrientation().y));
            mDetailsPanel->setParamValue(7, Ogre::StringConverter::toString(mCamera->getDerivedOrientation().z));
        }
    }

    return true;
}
//-------------------------------------------------------------------------------------
bool BaseApplication::keyPressed( const OIS::KeyEvent &arg )
{
    if (mTrayMgr->isDialogVisible()) return true;   // don't process any more keys if dialog is up

    if (arg.key == OIS::KC_F)   // toggle visibility of advanced frame stats
    {
        mTrayMgr->toggleAdvancedFrameStats();
    }
    else if (arg.key == OIS::KC_G)   // toggle visibility of even rarer debugging details
    {
        if (mDetailsPanel->getTrayLocation() == OgreBites::TL_NONE)
        {
            mTrayMgr->moveWidgetToTray(mDetailsPanel, OgreBites::TL_TOPRIGHT, 0);
            mDetailsPanel->show();
        }
        else
        {
            mTrayMgr->removeWidgetFromTray(mDetailsPanel);
            mDetailsPanel->hide();
        }
    }
    else if (arg.key == OIS::KC_T)   // cycle polygon rendering mode
    {
        Ogre::String newVal;
        Ogre::TextureFilterOptions tfo;
        unsigned int aniso;

        switch (mDetailsPanel->getParamValue(9).asUTF8()[0])
        {
        case 'B':
            newVal = "Trilinear";
            tfo = Ogre::TFO_TRILINEAR;
            aniso = 1;
            break;
        case 'T':
            newVal = "Anisotropic";
            tfo = Ogre::TFO_ANISOTROPIC;
            aniso = 8;
            break;
        case 'A':
            newVal = "None";
            tfo = Ogre::TFO_NONE;
            aniso = 1;
            break;
        default:
            newVal = "Bilinear";
            tfo = Ogre::TFO_BILINEAR;
            aniso = 1;
        }

        Ogre::MaterialManager::getSingleton().setDefaultTextureFiltering(tfo);
        Ogre::MaterialManager::getSingleton().setDefaultAnisotropy(aniso);
        mDetailsPanel->setParamValue(9, newVal);
    }
    else if (arg.key == OIS::KC_R)   // cycle polygon rendering mode
    {
        Ogre::String newVal;
        Ogre::PolygonMode pm;

        switch (mCamera->getPolygonMode())
        {
        case Ogre::PM_SOLID:
            newVal = "Wireframe";
            pm = Ogre::PM_WIREFRAME;
            break;
        case Ogre::PM_WIREFRAME:
            newVal = "Points";
            pm = Ogre::PM_POINTS;
            break;
        default:
            newVal = "Solid";
            pm = Ogre::PM_SOLID;
        }

        mCamera->setPolygonMode(pm);
        mDetailsPanel->setParamValue(10, newVal);
    }
    else if(arg.key == OIS::KC_F5)   // refresh all textures
    {
        Ogre::TextureManager::getSingleton().reloadAll();
    }
    else if (arg.key == OIS::KC_SYSRQ)   // take a screenshot
    {
        mWindow->writeContentsToTimestampedFile("screenshot", ".jpg");
    }

    mCameraMan->injectKeyDown(arg);
    return true;
}


//Adjust mouse clipping area
void BaseApplication::windowResized(Ogre::RenderWindow* rw)
{
    unsigned int width, height, depth;
    int left, top;
    rw->getMetrics(width, height, depth, left, top);

    const OIS::MouseState &ms = mMouse->getMouseState();
    ms.width = width;
    ms.height = height;
}

//Unattach OIS before window shutdown (very important under Linux)
void BaseApplication::windowClosed(Ogre::RenderWindow* rw)
{
    //Only close for window that created OIS (the main window in these demos)
    if( rw == mWindow )
    {
        if( mInputManager )
        {
            mInputManager->destroyInputObject( mMouse );
            mInputManager->destroyInputObject( mKeyboard );

            OIS::InputManager::destroyInputSystem(mInputManager);
            mInputManager = 0;
        }
    }
}

#endif //0