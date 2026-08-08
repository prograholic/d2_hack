#include "simple_b3d_car_renderer.h"

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


SimpleB3dCarRenderer::SimpleB3dCarRenderer()
    : BaseB3dApplication("SimpleB3dCarRenderer")
    , m_moveableObjects()
{
}
void SimpleB3dCarRenderer::CreateScene()
{
    m_sceneManager->setAmbientLight(Ogre::ColourValue(0.5f, 0.5f, 0.5f));

    Ogre::Light* light = m_sceneManager->createLight("MainLight");
    Ogre::SceneNode* rootNode = m_sceneManager->getRootSceneNode();
    Ogre::SceneNode* lightSceneNode = rootNode->createChildSceneNode("light.scene_node");
    lightSceneNode->attachObject(light);
    lightSceneNode->setPosition(20.0f, 80.0f, 150.0f);

    Ogre::SceneNode* b3dSceneNode = rootNode->createChildSceneNode("b3d.scene_node");

    CreateB3dScene(SinglePlayerRegistry, b3dSceneNode);
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

bool SimpleB3dCarRenderer::keyPressed(const OgreBites::KeyboardEvent& evt)
{
    return BaseB3dApplication::keyPressed(evt);
}

void SimpleB3dCarRenderer::shutdown()
{
    BaseApplication::shutdown();
}


void SimpleB3dCarRenderer::CreateRooms(const resource::data::b3d::B3dForest& /* forest */, Ogre::SceneNode* /* b3dSceneNode */)
{
}

void SimpleB3dCarRenderer::CreateMoveableObjects(const resource::data::b3d::B3dForest& forest, Ogre::SceneNode* b3dSceneNode)
{
    for (size_t i = 0; i != AllCarNames.size(); ++i)
    {
        m_moveableObjects.emplace_back(CreateMoveableObject(forest, AllCarNames[i], Ogre::Vector3{ 3.5f * i, 0, 0 }, b3dSceneNode));
    }
}

void SimpleB3dCarRenderer::OnCameraMoved(const scene_node::WorldContext& worldContext, const Ogre::Vector3f& movement)
{
    for (const auto& moveableObject : m_moveableObjects)
    {
        moveableObject->OnCameraMoved(worldContext, movement);
    }
}


} // namespace app
} // namespace d2_hack
