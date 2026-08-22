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


static void AddEntityToBullet(Ogre::SceneNode* node, Ogre::Bullet::DynamicsWorld* dynWorld)
{
    const auto& objs = node->getAttachedObjects();
    for (auto obj : objs)
    {
        Ogre::Entity* e = dynamic_cast<Ogre::Entity*>(obj);
        if (e)
        {
            dynWorld->addRigidBody(100, e, Ogre::Bullet::CT_SPHERE);
        }
    }

    const auto& children = node->getChildren();
    for (auto child : children)
    {
        AddEntityToBullet(static_cast<Ogre::SceneNode*>(child), dynWorld);
    }
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

    B3dRegistry reg = SinglePlayerRegistry;
    reg.entries.resize(1);

    CreateB3dScene(reg, b3dSceneNode);

    m_dynWorld.reset(new Ogre::Bullet::DynamicsWorld(Ogre::Vector3(0.0f, -9.8f, 0.0)));
    m_dbgDraw.reset(new Ogre::Bullet::DebugDrawer(m_sceneManager->getRootSceneNode(), m_dynWorld->getBtWorld()));

#if 0
    
    

    for (auto& moveableObject : m_moveableObjects)
    {
        for (const auto& moveableRootNode : moveableObject->GetRootNodes())
        {
            Ogre::SceneNode* sceneNode = std::static_pointer_cast<scene_node::OgreSceneNodeBase>(moveableRootNode)->GetOgreSceneNode();
            AddEntityToBullet(sceneNode, m_dynWorld.get());
        }
    }
#endif //0

    //m_dynWorld->addRigidBody(5, player, Bullet::CT_SPHERE);
    //m_dynWorld->addRigidBody(0, level, Bullet::CT_TRIMESH);
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
#if 0
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
    else if (evt.keysym.sym == '6')
    {
        static float angle = 0.0f;
        angle -= 0.1f;
        auto car = static_cast<B3dTruck*>(m_moveableObjects[0].get());
        //car->SwitchLight(true);
        
        car->Rotate(Ogre::Degree{ angle }, "wheel5");
    }
    else if (evt.keysym.sym == 'b')
    {
        D2_HACK_LOG(BREAK) << "BAAD";
    }
    else if (evt.keysym.sym == 'g')
    {
        D2_HACK_LOG(BREAK) << "GOOD";
    }
#endif //0
#if 1
    if (evt.keysym.sym == '0')
    {
        static float angle = 0.0f;
        angle -= 0.1f;
        for (auto& obj : m_moveableObjects)
        {
            auto wheelBased = dynamic_cast<WheelBasedMoveableObject*>(obj.get());
            if (wheelBased)
            {
                wheelBased->Rotate(Ogre::Degree{ angle }, WheelId::Wheel0);
            }
        }
    }
    else if (evt.keysym.sym == '1')
    {
        static float angle = 0.0f;
        angle -= 0.1f;
        for (auto& obj : m_moveableObjects)
        {
            auto wheelBased = dynamic_cast<WheelBasedMoveableObject*>(obj.get());
            if (wheelBased)
            {
                wheelBased->Rotate(Ogre::Degree{ angle }, WheelId::Wheel1);
            }
        }
    }
    else if (evt.keysym.sym == '2')
    {
        static float angle = 0.0f;
        angle -= 0.1f;
        for (auto& obj : m_moveableObjects)
        {
            auto wheelBased = dynamic_cast<WheelBasedMoveableObject*>(obj.get());
            if (wheelBased)
            {
                wheelBased->Rotate(Ogre::Degree{ angle }, WheelId::Wheel2);
            }
        }
    }
    else if (evt.keysym.sym == '3')
    {
        static float angle = 0.0f;
        angle -= 0.1f;
        for (auto& obj : m_moveableObjects)
        {
            auto wheelBased = dynamic_cast<WheelBasedMoveableObject*>(obj.get());
            if (wheelBased)
            {
                wheelBased->Rotate(Ogre::Degree{ angle }, WheelId::Wheel3);
            }
        }
    }
    else if (evt.keysym.sym == '4')
    {
        static float angle = 0.0f;
        angle -= 0.1f;
        for (auto& obj : m_moveableObjects)
        {
            auto wheelBased = dynamic_cast<WheelBasedMoveableObject*>(obj.get());
            if (wheelBased)
            {
                wheelBased->Rotate(Ogre::Degree{ angle }, WheelId::Wheel4);
            }
        }
    }
    else if (evt.keysym.sym == '5')
    {
        static float angle = 0.0f;
        angle -= 0.1f;
        for (auto& obj : m_moveableObjects)
        {
            auto wheelBased = dynamic_cast<WheelBasedMoveableObject*>(obj.get());
            if (wheelBased)
            {
                wheelBased->Rotate(Ogre::Degree{ angle }, WheelId::Wheel5);
            }
        }
    }
    else if (evt.keysym.sym == '6')
    {
        static float angle = 0.0f;
        angle -= 0.1f;
        for (auto& obj : m_moveableObjects)
        {
            auto wheelBased = dynamic_cast<WheelBasedMoveableObject*>(obj.get());
            if (wheelBased)
            {
                wheelBased->Rotate(Ogre::Degree{ angle }, WheelId::Wheel6);
            }
        }
    }
    else if (evt.keysym.sym == '7')
    {
        static float angle = 0.0f;
        angle -= 0.1f;
        for (auto& obj : m_moveableObjects)
        {
            auto wheelBased = dynamic_cast<WheelBasedMoveableObject*>(obj.get());
            if (wheelBased)
            {
                wheelBased->Rotate(Ogre::Degree{ angle }, WheelId::Wheel7);
            }
        }
    }
    else if (evt.keysym.sym == 'f')
    {
        static size_t dmgLevel = 0;
        dmgLevel = (dmgLevel + 1) % 4;
        for (auto& obj : m_moveableObjects)
        {
            auto mutObj = dynamic_cast<MutableObject*>(obj.get());
            if (mutObj)
            {
                mutObj->SetDamage(DamageKey::BR, static_cast<DamageLevel>(dmgLevel));
            }
        }
    }
#endif //0

    return BaseB3dApplication::keyPressed(evt);
}

void SimpleB3dCarRenderer::shutdown()
{
    BaseApplication::shutdown();
}

bool SimpleB3dCarRenderer::frameStarted(const Ogre::FrameEvent& event)
{
    m_dynWorld->getBtWorld()->stepSimulation(event.timeSinceLastFrame, 10);
    m_dbgDraw->update();

    return BaseB3dApplication::frameStarted(event);
}


void SimpleB3dCarRenderer::CreateRooms(const resource::data::b3d::B3dForest& /* forest */, Ogre::SceneNode* /* b3dSceneNode */)
{
}

void SimpleB3dCarRenderer::CreateMoveableObjects(const resource::data::b3d::B3dForest& forest, Ogre::SceneNode* b3dSceneNode)
{
    static const std::string_view Cars[] =
    {
        //"Storm",
        "Zil"
        //"STrailerP"
    };
    for (size_t i = 0; i != AllCarNames.size(); ++i)
    //for (size_t i = 0; i != _countof(Cars); ++i)
    {
        //m_moveableObjects.emplace_back(CreateMoveableObject(forest, Cars[i], Ogre::Vector3{ 3.5f * i, 0, 0 }, b3dSceneNode));
        m_moveableObjects.emplace_back(CreateMoveableObject(forest, AllCarNames[i], Ogre::Vector3{ 3.5f * i, 0, 0 }, b3dSceneNode));
    }
}

} // namespace app
} // namespace d2_hack
