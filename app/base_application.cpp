#include <d2_hack/app/base_application.h>

D2_HACK_DISABLE_WARNING_BEGIN(4251)

#include <OgreTextureManager.h>
#include <OgreArchiveManager.h>

D2_HACK_DISABLE_WARNING_END() //4251


namespace d2_hack
{
namespace app
{

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
    m_manager.reset(new resource::manager::Manager);
    m_d2ResArchiveFactory.reset(new resource::archive::res::ResArchive::Factory);
    Ogre::ArchiveManager::getSingleton().addArchiveFactory(m_d2ResArchiveFactory.get());

    m_txrImageCodec.reset(new resource::image::TxrImageCodec);
    Ogre::Codec::registerCodec(m_txrImageCodec.get());

    m_rawImageCodec.reset(new resource::image::RawImageCodec);
    Ogre::Codec::registerCodec(m_rawImageCodec.get());

    m_ogreMaterialProvider.reset(new resource::archive::res::OgreMaterialProvider);

    OgreBites::ApplicationContext::setup();
    addInputListener(this);
    setWindowGrab();

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

void BaseApplication::shutdown()
{
    m_manager.reset();

    OgreBites::ApplicationContext::shutdown();
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

} // namespace app
} // namespace d2_hack
