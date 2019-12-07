#ifndef D2_HACK_APPLICATION_H
#define D2_HACK_APPLICATION_H

#include "d2_hack_platform_support.h"

D2_HACK_DISABLE_WARNING_BEGIN(4100)
D2_HACK_DISABLE_WARNING_BEGIN(4275)
D2_HACK_DISABLE_WARNING_BEGIN(4251)

#include <Terrain/OgreTerrain.h>
#include <Terrain/OgreTerrainGroup.h>

D2_HACK_DISABLE_WARNING_END() // 4251
D2_HACK_DISABLE_WARNING_END() // 4275
D2_HACK_DISABLE_WARNING_END() // 4100


#include "base_application.h"


namespace d2_hack
{

struct ogre_deleter
{
    template <typename T>
    void operator()(T* ptr)
    {
        OGRE_DELETE ptr;
    }
};

}


class D2HackApplication :public BaseApplication
{
public:
    D2HackApplication();

private:

    std::unique_ptr<Ogre::TerrainGlobalOptions, d2_hack::ogre_deleter> m_terrainGlobalOptions;
    std::unique_ptr<Ogre::TerrainGroup, d2_hack::ogre_deleter> m_terrainGroup;

    virtual void CreateScene() override;

    void CreateTerrain();
    void ConfigureTerrainDefaults(Ogre::Light* light);
    void InitBlendMaps(Ogre::Terrain* terrain);
    void DefineTerrain(long x, long y);
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

#endif /* D2_HACK_APPLICATION_H */
