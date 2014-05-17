#include "TutorialApplication.h"


//-------------------------------------------------------------------------------------
TutorialApplication::TutorialApplication()
{
  mTimer = OGRE_NEW Ogre::Timer();
  mTimer->reset();
}
//-------------------------------------------------------------------------------------
TutorialApplication::~TutorialApplication()
{
  OGRE_DELETE m_terrainGroup;
  OGRE_DELETE m_terrainGlobalOptions;
}

//-------------------------------------------------------------------------------------
void TutorialApplication::createScene(void)
{
  mSceneMgr->setAmbientLight(Ogre::ColourValue(0.5f, 0.5f, 0.5f));

  Ogre::Entity* plane = mSceneMgr->createEntity("Plane", Ogre::SceneManager::PT_PLANE);

  plane->setMaterialName("PlaneMaterial", "Generated");

  Ogre::SceneNode* headNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("HeadNode");
  headNode->attachObject(plane);

  Ogre::Light* light = mSceneMgr->createLight("MainLight");
  light->setPosition(20.0f, 80.0f, 50.0f);

  m_terrainGlobalOptions = OGRE_NEW Ogre::TerrainGlobalOptions();

  m_terrainGroup = OGRE_NEW Ogre::TerrainGroup(mSceneMgr, Ogre::Terrain::ALIGN_X_Z, 513, 12000.0f);

  m_terrainGroup->loadAllTerrains(true);
}


int main(int argc, char *argv[])
{
  try
  {
    TutorialApplication app;
    app.go();
  }
  catch(const Ogre::Exception & e)
  {
    std::cerr << "An exception has occured: " <<
                 e.getFullDescription().c_str() << std::endl;
  }
  return 0;
}
