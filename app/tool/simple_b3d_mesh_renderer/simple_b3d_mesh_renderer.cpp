#include "simple_b3d_mesh_renderer.h"

#include <OgreEntity.h>
#include <OgreMesh.h>
#include <OgreSubMesh.h>

#include <d2_hack/common/log.h>
#include <d2_hack/common/log.h>
#include <d2_hack/common/types.h>
#include <d2_hack/resource/data/b3d_reader.h>
#include <d2_hack/resource/data/b3d_tree_optimization.h>

#include "b3d_tree_visitor.h"

namespace d2_hack
{
namespace app
{

using namespace resource::data::b3d;

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
#if 1
    LoadB3d("ba", b3dSceneNode);
    LoadB3d("bb", b3dSceneNode);
    LoadB3d("bc", b3dSceneNode);
    LoadB3d("bd", b3dSceneNode);
    LoadB3d("be", b3dSceneNode);
    LoadB3d("bf", b3dSceneNode);
    LoadB3d("bg", b3dSceneNode);
    LoadB3d("ca", b3dSceneNode);
    LoadB3d("cb", b3dSceneNode);
    LoadB3d("cc", b3dSceneNode);
    LoadB3d("ce", b3dSceneNode);
    LoadB3d("cf", b3dSceneNode);
    LoadB3d("ch", b3dSceneNode);
    LoadB3d("da", b3dSceneNode);
    LoadB3d("db", b3dSceneNode);
    LoadB3d("dc", b3dSceneNode);
    LoadB3d("dq", b3dSceneNode);
#endif
    LoadB3d("dr", b3dSceneNode);

    b3dSceneNode->pitch(Ogre::Radian(Ogre::Degree(-90)));
}


void VisitNode(const NodePtr& node, B3dTreeVisitor& visitor)
{
    node->Visit(visitor, VisitMode::PreOrder);

    const auto& children = node->GetChildNodeList();
    for (auto child : children)
    {
        VisitNode(child, visitor);
    }

    node->Visit(visitor, VisitMode::PostOrder);
}

void VisitTree(const B3dTree& tree, B3dTreeVisitor& visitor)
{
    for (auto node : tree.rootNodes)
    {
        VisitNode(node, visitor);
    }
}

void SimpleB3dMeshRenderer::LoadB3d(const char* b3dId, Ogre::SceneNode* b3dSceneNode)
{
    std::string fullB3dName = D2_ROOT_DIR "/ENV/" + std::string(b3dId) + ".b3d";
    std::ifstream inputFile{fullB3dName, std::ios_base::binary};
    if (!inputFile)
    {
        OGRE_EXCEPT(Ogre::Exception::ERR_FILE_NOT_FOUND, "failed to open file");
    }

    Ogre::FileStreamDataStream dataStream(&inputFile, false);
    B3dReader reader;
    B3dTree b3dTree = reader.Read(dataStream);

    optimization::Optimize(b3dTree);
    
    B3dTreeVisitor visitor{b3dId, fullB3dName, m_sceneManager, b3dSceneNode, mRoot->getMeshManager(), b3dTree.materials};

    VisitTree(b3dTree, visitor);
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


void PrintSubMeshesForNode(Ogre::SceneNode* node, int& cnt)
{
    const auto& children = node->getAttachedObjects();

    auto obj = children[cnt % children.size()];
    obj->setVisible(!obj->getVisible());
    D2_HACK_LOG(XXX) << node->getName() << " -> " << obj->getName();
    cnt += 1;
}

bool SimpleB3dMeshRenderer::keyPressed(const OgreBites::KeyboardEvent& evt)
{
    //D2_HACK_LOG("SimpleB3dMeshRenderer::keyPressed") << evt.type << ", " << evt.keysym.sym << ", " << evt.keysym.mod;
    if (evt.keysym.sym == '1')
    {
        PrintSceneNode(m_sceneManager->getRootSceneNode(), 0);
    }
    else if (evt.keysym.sym == '2')
    {
        static int cnt = 0;

        Ogre::SceneNode* show_009 = m_sceneManager->getSceneNode("ah_show_ah_009_scene_node");
        PrintSubMeshesForNode(show_009, cnt);
    }
    else if (evt.keysym.sym == '3')
    {
        static int cnt = 0;

        const auto& children = m_sceneManager->getRootSceneNode()->getChild("b3d.scene_node")->getChildren();
        Ogre::Node* node = children[cnt % children.size()];

        Ogre::SceneNode* sceneNode = static_cast<Ogre::SceneNode*>(node);
        sceneNode->flipVisibility();
        D2_HACK_LOG(YYY) << node->getName();
        cnt += 1;
    }
    else if (evt.keysym.sym == '4')
    {
        static int cnt = 0;

        Ogre::SceneNode* show_010 = m_sceneManager->getSceneNode("ah_show_ah_010_scene_node");
        PrintSubMeshesForNode(show_010, cnt);
    }
    else if (evt.keysym.sym == '5')
    {
        static int cnt = 0;
        cnt += 1;
        int dir = ((cnt % 2) * 2) - 1; // [-1, 1]

        const auto& children = m_sceneManager->getRootSceneNode()->getChild("b3d.scene_node")->getChildren();

        int i = 0;
        for (auto child : children)
        {
            i += 2;
            auto pos = child->getPosition();
            pos.z += (i * dir);

            child->setPosition(pos);
        }
    }
    else if (evt.keysym.sym == 'b')
    {
        D2_HACK_LOG(BREAK) << "BREAK";
    }

    return BaseApplication::keyPressed(evt);
}

} // namespace app
} // namespace d2_hack
