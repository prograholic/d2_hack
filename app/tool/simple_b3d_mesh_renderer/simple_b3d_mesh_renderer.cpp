#include "simple_b3d_mesh_renderer.h"

#include <OgreEntity.h>
#include <OgreMesh.h>
#include <OgreSubMesh.h>

#include <d2_hack/common/log.h>
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

#if 1
    LoadB3d("aa", b3dSceneNode);
    LoadB3d("ab", b3dSceneNode);
    LoadB3d("ac", b3dSceneNode);
    LoadB3d("ad", b3dSceneNode);
    LoadB3d("ae", b3dSceneNode);
    LoadB3d("af", b3dSceneNode);
    LoadB3d("ag", b3dSceneNode);
#endif
    LoadB3d("ah", b3dSceneNode);
#if 1
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
#endif

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
    D2_HACK_LOG(PrintSceneNode) << std::string(indent, ' ') << "NODE: " << node->getName();
    Ogre::SceneNode* sc = static_cast<Ogre::SceneNode*>(node);
    const auto& objs = sc->getAttachedObjects();
    for (auto obj : objs)
    {
        D2_HACK_LOG(PrintSceneNode) << std::string(indent, ' ') << "OBJECT: " << obj->getName();
        Ogre::Entity* e = dynamic_cast<Ogre::Entity*>(obj);
        if (e)
        {
            D2_HACK_LOG(PrintSceneNode) << std::string(indent, ' ') << " MESH: " << e->getMesh()->getName();
            const auto& submeshes = e->getMesh()->getSubMeshes();
            for (auto submesh : submeshes)
            {
                D2_HACK_LOG(PrintSceneNode) << std::string(indent, ' ') << "  SUBMESH: " << submesh->getMaterialName();
            }
        }
    }

    const auto& children = node->getChildren();
    for (auto child : children)
    {
        PrintSceneNode(child, indent + 4);
    }
}

bool SimpleB3dMeshRenderer::keyPressed(const OgreBites::KeyboardEvent& evt)
{
    static int cnt = 0;
    //D2_HACK_LOG("SimpleB3dMeshRenderer::keyPressed") << evt.type << ", " << evt.keysym.sym << ", " << evt.keysym.mod;
    if (evt.keysym.sym == '1')
    {
        PrintSceneNode(m_sceneManager->getRootSceneNode(), 0);
    }
    else if (evt.keysym.sym == '2')
    {
        //const auto& children = m_sceneManager->getRootSceneNode()->getChild("b3d.scene_node")->getChildren();
        Ogre::SceneNode* show_009 = m_sceneManager->getSceneNode("ah_show_ah_009_scene_node");
        //const auto& children = show_009->getChildren();
        const auto& children = show_009->getAttachedObjects();
        
        auto obj = children[cnt % children.size()];
        obj->setVisible(!obj->getVisible());
        D2_HACK_LOG(XXX) << obj->getName();
        cnt += 1;
    }

    return BaseApplication::keyPressed(evt);
}

} // namespace app
} // namespace d2_hack
