#include <iostream>

#include "d2_hack_application.h"

int main(int /* argc */, char * /* argv */ [])
{
    try
    {
        D2HackApplication app;
        
        app.initApp();
        app.getRoot()->startRendering();
        app.closeApp();
    }
    catch(const Ogre::Exception& e)
    {
        std::cerr << "An OGRE exception has occured: " << e.getFullDescription().c_str() << std::endl;
    }
    catch (const std::exception& e)
    {
        std::cerr << "An exception has occured: " << e.what() << std::endl;
    }
    
    return 0;
}
