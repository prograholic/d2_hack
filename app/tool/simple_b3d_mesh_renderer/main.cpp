#include <iostream>

#include "simple_b3d_mesh_renderer.h"

int main(int argc, char * argv [])
{
    try
    {
        if (argc > 1)
        {
            d2_hack::app::node_name = argv[1];
        }
        d2_hack::app::SimpleB3dMeshRenderer app;
        
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
