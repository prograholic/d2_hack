#include "simple_txr_renderer.h"

#include <OgreEntity.h>

namespace d2_hack
{
namespace app
{

SimpleTxrRenderer::SimpleTxrRenderer()
    : BaseApplication()
{
}

void SimpleTxrRenderer::CreateScene()
{
    m_sceneManager->setAmbientLight(Ogre::ColourValue(0.5f, 0.5f, 0.5f));

    Ogre::Entity* plane = m_sceneManager->createEntity("Plane", Ogre::SceneManager::PT_PLANE);

    plane->setMaterialName("PlaneMaterial", "D2");

    Ogre::SceneNode* headNode = m_sceneManager->getRootSceneNode()->createChildSceneNode("HeadNode");
    headNode->attachObject(plane);

    Ogre::Light* light = m_sceneManager->createLight("MainLight");
    Ogre::SceneNode* lightSceneNode = m_sceneManager->getRootSceneNode()->createChildSceneNode();
    lightSceneNode->attachObject(light);
    lightSceneNode->setPosition(20.0f, 80.0f, 150.0f);
}



} // namespace app
} // namespace d2_hack
