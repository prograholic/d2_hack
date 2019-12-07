#ifndef D2_HACK_TUTORIAL_APPLICATION_H
#define D2_HACK_TUTORIAL_APPLICATION_H

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

#endif /* D2_HACK_TUTORIAL_APPLICATION_H */
