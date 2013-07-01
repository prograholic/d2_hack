#include "TutorialApplication.h"


//-------------------------------------------------------------------------------------
TutorialApplication::TutorialApplication(void)
{
  mTimer = OGRE_NEW Ogre::Timer();
  mTimer->reset();
}
//-------------------------------------------------------------------------------------
TutorialApplication::~TutorialApplication(void)
{
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
}



int main(int argc, char *argv[])
{
  // Create application object
  TutorialApplication app;

  try {
    app.go();
  } catch( Ogre::Exception& e ) {
    std::cerr << "An exception has occured: " <<
                 e.getFullDescription().c_str() << std::endl;
  }
  return 0;
}
