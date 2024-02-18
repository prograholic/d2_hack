#include "simple_b3d_mesh_renderer.h"

#include <OgreEntity.h>
#include <OgreMesh.h>
#include <OgreSubMesh.h>

#include <d2_hack/common/log.h>
#include <d2_hack/common/log.h>
#include <d2_hack/common/types.h>
#include <d2_hack/common/utils.h>
#include <d2_hack/resource/data/b3d_reader.h>
#include <d2_hack/resource/data/b3d_tree_optimization.h>
#include <d2_hack/resource/data/b3d_utils.h>

#include "b3d_tree_visitor.h"

namespace d2_hack
{
namespace app
{

using namespace resource::data::b3d;

static void VisitNode(const NodePtr& node, B3dTreeVisitor& visitor)
{
    node->Visit(visitor, VisitMode::PreOrder);

    const auto& children = node->GetChildNodeList();
    for (auto child : children)
    {
        VisitNode(child, visitor);
    }

    node->Visit(visitor, VisitMode::PostOrder);
}

static void VisitTree(const B3dTree& tree, B3dTreeVisitor& visitor)
{
    for (auto node : tree.rootNodes)
    {
        VisitNode(node, visitor);
    }
}

static void ConnectTruckToScenes(B3dForest& forest, const std::string& truckName)
{
    for (auto& tree : forest.forest)
    {
        block_data::GroupObjects19 object19Data{};
        auto object19 = MakeVisitableNode(tree, WeakNodePtr{}, MakeBlockHeader(common::ResourceName{}, block_data::GroupObjectsBlock19), object19Data);
        
        block_data::GroupObjects5 object5data{};
        auto object5 = MakeVisitableNode(tree, object19, MakeBlockHeader(common::ResourceName{}, block_data::GroupObjectsBlock5), object5data);

        block_data::SimpleObjectConnector18 object18Data{};
        object18Data.space = common::ResourceName{}; // no need to transform
        object18Data.object = common::StringToResourceName(truckName);
        auto object18 = MakeVisitableNode(tree, object5, MakeBlockHeader(common::ResourceName{}, block_data::SimpleObjectConnectorBlock18), object18Data);

        tree->rootNodes.push_back(object19);
    }
}


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

    B3dForest b3dForest = ReadB3d(SinglePlayerRegistry);

    ConnectTruckToScenes(b3dForest, "Zil");

    transformation::Transform(b3dForest);
    transformation::Optimize(b3dForest);

    for (auto& tree : b3dForest.forest)
    {
        B3dTreeVisitor visitor{tree->id, tree->transformations, m_sceneManager, b3dSceneNode, mRoot->getMeshManager()};

        VisitTree(*tree, visitor);
    }

    b3dSceneNode->pitch(Ogre::Radian(Ogre::Degree(-90)));
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

} // namespace app
} // namespace d2_hack
