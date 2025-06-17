#include "simple_b3d_mesh_renderer.h"

#include <OgreEntity.h>
#include <OgreMesh.h>
#include <OgreSubMesh.h>

#include <d2_hack/common/log.h>

namespace d2_hack
{
namespace app
{

using namespace common;
using namespace resource::data::b3d;


SimpleB3dMeshRenderer::SimpleB3dMeshRenderer()
    : BaseB3dApplication("SimpleB3dMeshRenderer")
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

    CreateB3dScene(SinglePlayerRegistry, AllCarNames, b3dSceneNode);

    b3dSceneNode->pitch(Ogre::Radian(Ogre::Degree(-90)), Ogre::Node::TransformSpace::TS_WORLD);
}

static void PrintSceneNode(Ogre::Node* node, int indent)
{
    D2_HACK_LOG(PrintSceneNode) << std::string(indent, ' ') << "NODE: " << node->getName() << ", " << node->_getDerivedPosition();
    Ogre::SceneNode* sc = static_cast<Ogre::SceneNode*>(node);
    const auto& objs = sc->getAttachedObjects();
    for (auto obj : objs)
    {
        D2_HACK_LOG(PrintSceneNode) << std::string(indent, ' ') << " OBJECT: " << obj->getName();
        Ogre::Entity* e = dynamic_cast<Ogre::Entity*>(obj);
        if (e)
        {
            D2_HACK_LOG(PrintSceneNode) << std::string(indent, ' ') << "  MESH: " << e->getMesh()->getName();
            const auto& submeshes = e->getMesh()->getSubMeshes();
            for (auto submesh : submeshes)
            {
                D2_HACK_LOG(PrintSceneNode) << std::string(indent, ' ') << "   SUBMESH: " << submesh->getMaterialName();
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
    const auto children = node->getAttachedObjects();
    if (!children.empty())
    {
        auto obj = children[cnt % children.size()];

        D2_HACK_LOG(XXX) << node->getName() << " -> " << obj->getName();
        cnt += 1;
    }
}

const char* node_name = "b3d.scene_node";

bool SimpleB3dMeshRenderer::keyPressed(const OgreBites::KeyboardEvent& evt)
{
    ProcessCameraMovement();

    //D2_HACK_LOG("SimpleB3dMeshRenderer::keyPressed") << evt.type << ", " << evt.keysym.sym << ", " << evt.keysym.mod;
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

        const auto& children = m_sceneManager->getSceneNode(node_name)->getChildren();
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

        auto children = m_sceneManager->getSceneNode(node_name)->getChildren();

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
        D2_HACK_LOG(BREAK) << "BAAD";
    }
    else if (evt.keysym.sym == 'g')
    {
        D2_HACK_LOG(BREAK) << "GOOD";
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

void SimpleB3dMeshRenderer::shutdown()
{
    BaseApplication::shutdown();
}


void SimpleB3dMeshRenderer::ProcessCameraMovement()
{
    Ogre::Vector3f currentPlayerPosition = m_cameraSceneNode->_getDerivedPosition();
    Ogre::Vector3f movement = currentPlayerPosition - m_worldContext.playerPosition;
    m_worldContext.playerPosition = currentPlayerPosition;

    if (movement != Ogre::Vector3f::ZERO)
    {
        OnCameraMoved(m_worldContext, movement);
    }
}

} // namespace app
} // namespace d2_hack
