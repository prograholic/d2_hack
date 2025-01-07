#include "simple_b3d_mesh_renderer.h"

#include <d2_hack/common/log.h>
#include <d2_hack/common/utils.h>

#include <d2_hack/resource/data/b3d_visitor.h>
#include <d2_hack/resource/data/b3d_reader.h>
#include <d2_hack/resource/data/b3d_utils.h>
#include <d2_hack/resource/data/b3d_tree_optimization.h>

#include <d2_hack/scene_node/terrain_scene_node.h>

#include <OgreEntity.h>
#include <OgreMesh.h>
#include <OgreSubMesh.h>

#include "b3d_tree_visitor.h"

namespace d2_hack
{
namespace app
{

using namespace common;
using namespace resource::data::b3d;

static void ConnectTruckToScenes(B3dForest& forest, const std::string& truckName, const Ogre::Vector3& pos)
{
    bool done = false;
    for (auto& tree : forest.forest)
    {
        if (done)
        {
            break;
        }
        block_data::GroupObjects19 object19Data{};
        auto object19 = MakeVisitableNode(tree, WeakNodePtr{}, MakeBlockHeader(common::StringToResourceName(CarNodeNamePrefix + truckName), block_data::GroupObjectsBlock19), object19Data);

        block_data::GroupObjects5 object5data{};
        auto object5 = MakeVisitableNode(tree, object19, MakeBlockHeader(common::ResourceName{}, block_data::GroupObjectsBlock5), object5data);

        block_data::SimpleObjectConnector18 object18Data{};
        object18Data.object = common::StringToResourceName(truckName);

        Transform tf{ Ogre::Matrix3::IDENTITY, pos };

        object18Data.transformation.push_back(tf);
        auto object18 = MakeVisitableNode(tree, object5, MakeBlockHeader(common::ResourceName{}, block_data::SimpleObjectConnectorBlock18), object18Data);

        tree->rootNodes.push_back(object19);

        done = true;
    }
}


SimpleB3dMeshRenderer::SimpleB3dMeshRenderer()
    : BaseApplication()
{
}

void SimpleB3dMeshRenderer::CreateRootNodes(const resource::data::b3d::B3dTree& tree, Ogre::SceneNode* b3dSceneNode)
{
    for (const auto& rootNode : tree.rootNodes)
    {
        if (rootNode->GetNodeCategory() == NodeCategory::RoomNode)
        {
            if (!rootNode->GetChildNodeList().empty())
            {
                m_rooms.emplace_back(std::make_unique<B3dRoom>(rootNode, tree.id, m_sceneManager, b3dSceneNode, mRoot->getMeshManager()));
            }
            else
            {
                D2_HACK_LOG(CreateRootNodes) << "Skipping empty room: `" << rootNode->GetName() << "`";
            }
        }
        else if (rootNode->GetNodeCategory() == NodeCategory::CarNode)
        {
            m_cars.emplace_back(std::make_unique<B3dCar>(rootNode, tree.id, m_sceneManager, b3dSceneNode, mRoot->getMeshManager()));
        }
        else
        {
            D2_HACK_LOG(CreateRootNodes) << "Skipping uncategorized root node: `" << rootNode->GetName() << "`";
        }
    }
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

    B3dForest b3dForest = ReadB3d(SinglePlayerRegistry);

    const char* names[] =
    {
        "Zil",
#if 0
        "Kamaz",
        "Freightliner",
        "Scania",
        "Renault",
        "Kenworth",
        "Mack",
        "Peterbilt",
        "Daf",
        "Mercedes",
        "Volvo",
        "Storm",
        "International",
        "BmwM5police",
        "BmwM5",
        "Cayman",
        "Offroad",
        "Pickup",
        "Patrol",
        "Gazelle",
        "Gazelle1C",
        "Sobol",
        "RenaultR",
        "KamazR",
        "ScaniaR",
        "ZilR",
        "MercedesR",
        "VolvoR",
        "DafR",
        "StormR",
        "STrailerP",
        "STrailerT",
        "STrailerM",
        "STrailerStorm",
        "k50",
        "PBmwM5",
        "POffroad",
        "PPickup",
        "PPatrol",
        "PGazelle",
        "PSobol",
        "PMarera",
        "PMegan",
        "PMini",
        "POka",
        "PVan",
        "PBus",
        "PVolga",
        "PFiat",
        "PAvensis",
        "Mini",
        "Marera",
        "Bus",
        "Katok",
        "Megan",
        "Oka",
        "Van",
        "Avensis",
        "Volga",
        "Fiat",
#endif //0
    };

    for (size_t i = 0; i != sizeof(names) / sizeof(names[0]); ++i)
    {
        ConnectTruckToScenes(b3dForest, names[i], Ogre::Vector3{ 3.5f * i, 0, 0 });
    }

    transformation::Transform(b3dForest);
    transformation::Optimize(b3dForest);

    for (auto& tree : b3dForest.forest)
    {
        CreateRootNodes(*tree, b3dSceneNode);
    }

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
    MovePlayer();

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
    m_rooms.clear(); // TODO: rework with RAII
    BaseApplication::shutdown();
}


void SimpleB3dMeshRenderer::MovePlayer()
{
    Ogre::Vector3f currentPlayerPosition = m_cameraSceneNode->_getDerivedPosition();
    Ogre::Vector3f movement = currentPlayerPosition - m_worldContext.playerPosition;
    m_worldContext.playerPosition = currentPlayerPosition;

    for (const auto& room : m_rooms)
    {
        room->PlayerMoved(m_worldContext, movement);
    }
}

} // namespace app
} // namespace d2_hack
