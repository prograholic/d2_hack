#include "simple_b3d_mesh_renderer.h"

//#include <OgreMesh.h>
//#include <OgreSubMesh.h>
//#include <OgreMeshManager.h>
//#include <OgreHardwareBufferManager.h>
//#include <OgreEntity.h>


//
//#include <d2_hack/common/resource_mgmt.h>
#include <d2_hack/common/log.h>
#include <d2_hack/common/types.h>

#include "b3d_listener.h"

namespace d2_hack
{
namespace app
{


SimpleB3dMeshRenderer::SimpleB3dMeshRenderer()
    : BaseApplication()
{
}

void SimpleB3dMeshRenderer::CreateScene()
{
    m_sceneManager->setAmbientLight(Ogre::ColourValue(0.5f, 0.5f, 0.5f));

    Ogre::Light* light = m_sceneManager->createLight("MainLight");
    Ogre::SceneNode* rootNode = m_sceneManager->getRootSceneNode();
    Ogre::SceneNode* lightSceneNode = rootNode->createChildSceneNode("light.scene_node");
    lightSceneNode->attachObject(light);
    lightSceneNode->setPosition(20.0f, 80.0f, 150.0f);


    Ogre::SceneNode* b3dSceneNode = rootNode->createChildSceneNode("b3d.scene_node");

    LoadB3d("aa", b3dSceneNode);
    LoadB3d("ab", b3dSceneNode);
    LoadB3d("ac", b3dSceneNode);
    LoadB3d("ad", b3dSceneNode);
    LoadB3d("ae", b3dSceneNode);
    LoadB3d("af", b3dSceneNode);
    LoadB3d("ag", b3dSceneNode);
    LoadB3d("ah", b3dSceneNode);
    LoadB3d("aj", b3dSceneNode);
    LoadB3d("ak", b3dSceneNode);
    LoadB3d("al", b3dSceneNode);
    LoadB3d("am", b3dSceneNode);
    LoadB3d("ap", b3dSceneNode);
    LoadB3d("aq", b3dSceneNode);
    LoadB3d("ar", b3dSceneNode);
    LoadB3d("as", b3dSceneNode);
    LoadB3d("at", b3dSceneNode);
    LoadB3d("au", b3dSceneNode);
    LoadB3d("av", b3dSceneNode);
    LoadB3d("aw", b3dSceneNode);
    LoadB3d("ax", b3dSceneNode);

    b3dSceneNode->pitch(Ogre::Radian(Ogre::Degree(-90)));
}






void SimpleB3dMeshRenderer::LoadB3d(const char* b3dId, Ogre::SceneNode* b3dSceneNode)
{
    std::string fullB3dName = D2_ROOT_DIR "/ENV/" + std::string(b3dId) + ".b3d";
    B3dMeshListener listener{b3dId, fullB3dName, m_sceneManager, b3dSceneNode, mRoot->getMeshManager() };

    std::ifstream inputFile{fullB3dName, std::ios_base::binary};
    if (!inputFile)
    {
        OGRE_EXCEPT(Ogre::Exception::ERR_FILE_NOT_FOUND, "failed to open file");
    }

    Ogre::FileStreamDataStream dataStream(&inputFile, false);
    resource::data::b3d::B3dReader reader;
    reader.Read(dataStream, listener);
}


static void PrintSceneNode(Ogre::Node* node, int indent)
{
    D2_HACK_LOG(PrintSceneNode) << std::string(indent, ' ') << node->getName();
    const auto& children = node->getChildren();
    {
        for (auto child : children)
        {
            PrintSceneNode(child, indent + 1);
        }
    }
}

bool SimpleB3dMeshRenderer::keyPressed(const OgreBites::KeyboardEvent& evt)
{
    static int visible_count = 0;
    //D2_HACK_LOG("SimpleB3dMeshRenderer::keyPressed") << evt.type << ", " << evt.keysym.sym << ", " << evt.keysym.mod;
    if (evt.keysym.sym == 'l')
    {
        PrintSceneNode(m_sceneManager->getRootSceneNode(), 0);
    }

    return BaseApplication::keyPressed(evt);
}

} // namespace app
} // namespace d2_hack
