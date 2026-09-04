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
    , m_rooms()
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

    const B3dRegistry registry
    {
        D2_ROOT_DIR,
        "ENV",
        {
            "ad",
        }
    };

    //CreateB3dScene(registry, b3dSceneNode);
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

static const char* room_events[] =
{
    "AP5_people_key",
    "cows06",
    "Benzo2Key",
    "DC2_people_key",
    "AT3_people_key",
    "FallAB2Key",
    "sheep01",
    "BenzoKey",
    "AR2_people_key",
    "DC3_people_key",
    "K_CLOSE_BB0",
    "AD2_people_key",
    "FallABDA1Key",
    "AP7_people_key",
    "AU1_people_key",
    "DC4_people_key",
    "AU3_people_key",
    "MineKey",
    "AS2_people_key",
    "cows05",
    "FallAC1Key",
    "K_bb040_DR",
    "sheep10",
    "FallAC3Key",
    "ILLUM_LEVEL_05",
    "ILLUM_LEVEL_03",
    "StartKey",
    "ILLUM_LEVEL_13",
    "AT2_people_key",
    "AU6_people_key",
    "DC1_people_key",
    "FallDA1Key",
    "sheep60",
    "AD4_people_key",
    "ILLUM_LEVEL_06",
    "AR6_people_key",
    "AP1_people_key",
    "ILLUM_LEVEL_00",
    "AP3_people_key",
    "AT1_people_key",
    "AM3_people_key",
    "FloodAX1Key",
    "ILLUM_LEVEL_01",
    "ILLUM_LEVEL_15",
    "sheep11",
    "AD3_people_key",
    "FallDA3Key",
    "AS3_people_key",
    "ILLUM_LEVEL_07",
    "cows01",
    "AJ1_people_key",
    "CLOSE_BD_R",
    "PinKey",
    "TrafficLightKey0",
    "sheep00",
    "AJ2_people_key",
    "FloodAQKey",
    "AP8_people_key",
    "AP6_people_key",
    "AM2_people_key",
    "ILLUM_LEVEL_02",
    "sheep61",
    "sheep71",
    "K_bb040_DSR",
    "ILLUM_LEVEL_12",
    "AU2_people_key",
    "AR1_people_key",
    "TextureSizeKey",
    "sheep70",
    "RadarHeadOn",
    "sheep100",
    "RadarKey",
    "FallAB1Key",
    "TrafficLightKey1",
    "AJ3_people_key",
    "FloodAWKey",
    "AR9_people_key",
    "FallAB3Key",
    "AR8_people_key",
    "AJ4_people_key",
    "TL_operative",
    "cows04",
    "FallDA2Key",
    "ILLUM_LEVEL_04",
    "Benzo1Key",
    "AP2_people_key",
    "AD1_people_key",
    "GeometryKey",
    "sheep90",
    "K_bb041_DSR",
    "Benzo0Key",
    "GreenSvetKey",
    "AU5_people_key",
    "FallABDA2Key",
    "Faza8Key",
    "CLOSE_BD_L",
    "cows00",
    "RedSvetKey",
    "cows03",
    "AS1_people_key",
    "CLOSE_BD_L_BE",
    "FallAC2Key",
    "AP4_people_key",
    "FloodAX2Key",
    "AR5_people_key",
    "AM1_people_key",
    "sheep12",
    "FonarHeadOn",
    "AU4_people_key",
    "cows02",
    "ILLUM_LEVEL_14",

};

static size_t room_events_pos = 0;
static size_t event_index = 0;


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
    else if (evt.keysym.sym == 'e')
    {
        room_events_pos =  (room_events_pos + 1 ) % _countof(room_events);
        event_index = 0;
        ApplyRoomEvent(room_events[room_events_pos], event_index);
    }
    else if (evt.keysym.sym == 'r')
    {
        event_index += 1;
        ApplyRoomEvent(room_events[room_events_pos], event_index);
    }
    else if (evt.keysym.sym == 't')
    {
        event_index = 0;
        ApplyRoomEvent(room_events[room_events_pos], event_index);
    }
    else if (evt.keysym.sym == 'b')
    {
        D2_HACK_LOG(BREAK) << "BAAD";
    }
    else if (evt.keysym.sym == 'g')
    {
        D2_HACK_LOG(BREAK) << "GOOD";
    }

    return BaseB3dApplication::keyPressed(evt);
}

void SimpleB3dMeshRenderer::shutdown()
{
    BaseApplication::shutdown();
}

void SimpleB3dMeshRenderer::CreateRooms(const B3dForest& forest, Ogre::SceneNode* b3dSceneNode)
{
    for (const auto& tree : forest.forest)
    {
        for (const auto& rootNode : tree->rootNodes)
        {
            if (rootNode->GetNodeCategory() == NodeCategory::RoomNode)
            {
                if (!rootNode->GetChildNodeList().empty())
                {
                    auto roomSceneNode = b3dSceneNode->createChildSceneNode();
                    m_rooms.emplace_back(CreateRoom(forest, rootNode->GetName(), roomSceneNode));
                }
                else
                {
                    D2_HACK_LOG(CreateRootNodes) << "Skipping empty room: `" << rootNode->GetName() << "`";
                }
            }
            else
            {
                D2_HACK_LOG(CreateRootNodes) << "Skipping uncategorized root node: `" << rootNode->GetName() << "`";
            }
        }
    }
}

void SimpleB3dMeshRenderer::CreateMoveableObjects(const B3dForest& /* forest */, Ogre::SceneNode* /* b3dSceneNode */)
{
}

void SimpleB3dMeshRenderer::ApplyRoomEvent(const char* eventName, size_t eventId)
{
    for (const auto& room : m_rooms)
    {
        room->ApplyEvent(eventName, eventId);
    }
}

} // namespace app
} // namespace d2_hack
