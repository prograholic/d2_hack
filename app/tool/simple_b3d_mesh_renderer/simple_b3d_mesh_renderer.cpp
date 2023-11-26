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

    LoadB3d("ENV", "aa", b3dSceneNode);
    LoadB3d("ENV", "ab", b3dSceneNode);
    LoadB3d("ENV", "ac", b3dSceneNode);
    LoadB3d("ENV", "ad", b3dSceneNode);
    LoadB3d("ENV", "ae", b3dSceneNode);
    LoadB3d("ENV", "af", b3dSceneNode);
    LoadB3d("ENV", "ag", b3dSceneNode);
    LoadB3d("ENV", "ah", b3dSceneNode);
    LoadB3d("ENV", "aj", b3dSceneNode);
    LoadB3d("ENV", "ak", b3dSceneNode);
    LoadB3d("ENV", "al", b3dSceneNode);
    LoadB3d("ENV", "am", b3dSceneNode);
    LoadB3d("ENV", "ap", b3dSceneNode);
    LoadB3d("ENV", "aq", b3dSceneNode);
    LoadB3d("ENV", "ar", b3dSceneNode);
    LoadB3d("ENV", "as", b3dSceneNode);
    LoadB3d("ENV", "at", b3dSceneNode);
    LoadB3d("ENV", "au", b3dSceneNode);
    LoadB3d("ENV", "av", b3dSceneNode);
    LoadB3d("ENV", "aw", b3dSceneNode);
    LoadB3d("ENV", "ax", b3dSceneNode);

    LoadB3d("ENV", "ba", b3dSceneNode);
    LoadB3d("ENV", "bb", b3dSceneNode);
    LoadB3d("ENV", "bc", b3dSceneNode);
    LoadB3d("ENV", "bd", b3dSceneNode);
    LoadB3d("ENV", "be", b3dSceneNode);
    LoadB3d("ENV", "bf", b3dSceneNode);
    LoadB3d("ENV", "bg", b3dSceneNode);

    LoadB3d("ENV", "ca", b3dSceneNode);
    LoadB3d("ENV", "cb", b3dSceneNode);
    LoadB3d("ENV", "cc", b3dSceneNode);
    LoadB3d("ENV", "ce", b3dSceneNode);
    LoadB3d("ENV", "cf", b3dSceneNode);
    LoadB3d("ENV", "ch", b3dSceneNode);

    LoadB3d("ENV", "da", b3dSceneNode);
    LoadB3d("ENV", "db", b3dSceneNode);
    LoadB3d("ENV", "dc", b3dSceneNode);
    LoadB3d("ENV", "dq", b3dSceneNode);
    LoadB3d("ENV", "dr", b3dSceneNode);

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

void SimpleB3dMeshRenderer::LoadB3d(const std::string& subdirectory, const std::string& b3dId, Ogre::SceneNode* b3dSceneNode)
{
    std::string fullB3dName = D2_ROOT_DIR "/" + subdirectory + "/" + b3dId + ".b3d";
    std::ifstream inputFile{fullB3dName, std::ios_base::binary};
    if (!inputFile)
    {
        OGRE_EXCEPT(Ogre::Exception::ERR_FILE_NOT_FOUND, "failed to open file");
    }

    Ogre::FileStreamDataStream dataStream(&inputFile, false);
    B3dReader reader;
    B3dTree b3dTree = reader.Read(dataStream);

    transformation::Transform(b3dTree);
    transformation::Optimize(b3dTree);
    
    B3dTreeVisitor visitor{b3dId, fullB3dName, m_sceneManager, b3dSceneNode, mRoot->getMeshManager(), b3dTree.materials};

    VisitTree(b3dTree, visitor);
}


static void PrintSceneNode(Ogre::Node* node, int indent)
{
    D2_HACK_LOG(PrintSceneNode) << std::string(indent, ' ') << "NODE: " << node->getName() << ", " << node->_getDerivedPosition();
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
    if (!children.empty())
    {
        auto obj = children[cnt % children.size()];
        obj->setVisible(!obj->getVisible());
        D2_HACK_LOG(XXX) << node->getName() << " -> " << obj->getName();
        cnt += 1;
    }
}

bool SimpleB3dMeshRenderer::keyPressed(const OgreBites::KeyboardEvent& evt)
{
    //D2_HACK_LOG("SimpleB3dMeshRenderer::keyPressed") << evt.type << ", " << evt.keysym.sym << ", " << evt.keysym.mod;

    const char* node_name = "ap_AirportVyshka_clone_scene_node_17";

    if (evt.keysym.sym == '1')
    {
        PrintSceneNode(m_sceneManager->getRootSceneNode(), 0);
    }
    else if (evt.keysym.sym == '2')
    {
        static int cnt = 0;

        Ogre::SceneNode* node = m_sceneManager->getSceneNode(node_name, false);
        if (node)
        {
            PrintSubMeshesForNode(node, cnt);
        }
    }
    else if (evt.keysym.sym == '3')
    {
        static int cnt = 0;

        const auto& children = m_sceneManager->getSceneNode(node_name, false)->getChildren();
        if (!children.empty())
        {
            Ogre::Node* node = children[cnt % children.size()];

            Ogre::SceneNode* sceneNode = static_cast<Ogre::SceneNode*>(node);
            sceneNode->flipVisibility();
            D2_HACK_LOG(YYY) << node->getName();
            cnt += 1;
        }
    }
    else if (evt.keysym.sym == '5')
    {
        static int cnt = 0;
        cnt += 1;
        int dir = ((cnt % 2) * 2) - 1; // [-1, 1]

        auto children = m_sceneManager->getRootSceneNode()->getChild("b3d.scene_node")->getChildren();

        int i = 0;
        while (!children.empty())
        {
            auto child = children[0];
            children.erase(children.begin());
            children.insert(children.end(), child->getChildren().begin(), child->getChildren().end());

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
    else if (evt.keysym.sym == '=')
    {
        m_cameraManager->setTopSpeed(m_cameraManager->getTopSpeed() * 2);
    }
    else if (evt.keysym.sym == '-')
    {
        m_cameraManager->setTopSpeed(m_cameraManager->getTopSpeed() / 2);
    }

    return BaseApplication::keyPressed(evt);
}

} // namespace app
} // namespace d2_hack
