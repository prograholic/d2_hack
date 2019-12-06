/*
-----------------------------------------------------------------------------
Filename:    TutorialApplication.h
-----------------------------------------------------------------------------

This source file is part of the
   ___                 __    __ _ _    _
  /___\__ _ _ __ ___  / / /\ \ (_) | _(_)
 //  // _` | '__/ _ \ \ \/  \/ / | |/ / |
/ \_// (_| | | |  __/  \  /\  /| |   <| |
\___/ \__, |_|  \___|   \/  \/ |_|_|\_\_|
      |___/
      Tutorial Framework
      http://www.ogre3d.org/tikiwiki/
-----------------------------------------------------------------------------
*/
#ifndef __TutorialApplication_h_
#define __TutorialApplication_h_

#include "base_application.h"


class TutorialApplication :public BaseApplication
{
    
};

#if 0

class TutorialApplication : public BaseApplication
{
public:
    TutorialApplication();
    virtual ~TutorialApplication();

    Ogre::RenderWindow * getWindow(void) { return mWindow; }
    Ogre::Timer * getTimer(void) { return mTimer; }
    OIS::Mouse * getMouse(void) { return mMouse; }
    OIS::Keyboard * getKeyboard(void) { return mKeyboard; }
protected:
    virtual void createScene(void);
    Ogre::Timer *mTimer;

    Ogre::TerrainGlobalOptions* m_terrainGlobalOptions;
    Ogre::TerrainGroup* m_terrainGroup;


private:
    void configureTerrainDefaults(Ogre::Light* light);
    void initBlendMaps(Ogre::Terrain* terrain);
    void createTerrain();
    void defineTerrain(long x, long y);
};

#endif //0

#endif // #ifndef __TutorialApplication_h_
