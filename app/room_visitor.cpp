#include "room_visitor.h"

#include <OgreMeshManager.h>

#include <d2_hack/scene_node/switchable_scene_nodes.h>
#include <d2_hack/scene_node/generator_scene_nodes.h>

#include <d2_hack/common/utils.h>
#include <d2_hack/common/log.h>
#include <d2_hack/common/resource_mgmt.h>

namespace d2_hack
{
namespace app
{

using namespace resource::data::b3d;

RoomVisitor::RoomVisitor(std::string_view b3dId,
                         std::string_view blockName,
                         Ogre::MeshManager* meshManager,
                         Ogre::SceneManager* sceneManager,
                         resource::archive::res::OgreMaterialProvider* ogreMaterialProvider)
    : GameObjectVisitorBase(b3dId, blockName, meshManager, ogreMaterialProvider)
    , m_sceneManager(sceneManager)
{
}

VisitResult RoomVisitor::Visit(const std::shared_ptr<NodeGroupRoadInfraObjects4>& /* node */, VisitMode /* visitMode */)
{
    return VisitResult::Continue;
}

VisitResult RoomVisitor::Visit(const std::shared_ptr<NodeGroupTrigger9>& node, VisitMode visitMode)
{
    if (visitMode == VisitMode::PreOrder)
    {
        auto parentB3dSceneNode = GetParentSceneNode();
        auto b3dSceneNode = CreateSceneNode<scene_node::GroupTrigger9>(parentB3dSceneNode, node->GetName(), node->GetBlockData());
        PushToSceneNodeStack(b3dSceneNode);
    }
    else
    {
        PopFromSceneNodeStack();
    }

    return VisitResult::Continue;
}

VisitResult RoomVisitor::Visit(const std::shared_ptr<NodeGroupUnknown12>& /* node */, VisitMode /* visitMode */)
{
    return VisitResult::Continue;
}

VisitResult RoomVisitor::Visit(const std::shared_ptr<NodeSimpleTrigger13>& /* node */, VisitMode /* visitMode */)
{
    return VisitResult::Continue;
}

VisitResult RoomVisitor::Visit(const std::shared_ptr<NodeGroupObjects19>& /* node */, VisitMode /* visitMode */)
{
    return VisitResult::Continue;
}

VisitResult RoomVisitor::Visit(const std::shared_ptr<NodeSimpleFlatCollision20>& /* node */, VisitMode /* visitMode */)
{
    return VisitResult::Continue;
}

VisitResult RoomVisitor::Visit(const std::shared_ptr<NodeGroupUnknown29>& /* node */, VisitMode /* visitMode */)
{
    return VisitResult::Continue;
}

VisitResult RoomVisitor::Visit(const std::shared_ptr<NodeSimplePortal30>& /* node */, VisitMode /* visitMode */)
{
    return VisitResult::Continue;
}

VisitResult RoomVisitor::Visit(const std::shared_ptr<NodeSimpleUnknown34>& /* node */, VisitMode /* visitMode */)
{
    return VisitResult::Continue;
}

VisitResult RoomVisitor::Visit(const std::shared_ptr<NodeSimpleGeneratedObjects40>& node, VisitMode visitMode)
{
    auto generatorName = common::ResourceNameToStringView(node->GetBlockData().name);
    if (visitMode == VisitMode::PreOrder)
    {
        scene_node::SceneNodeBasePtr newNode;
        if (generatorName == "$$GeneratorOfTerrain")
        {
            auto newName = std::format("terrain.{}", node->GetName());
            newNode = CreateSceneNode<scene_node::TerrainSceneNode>(GetParentSceneNode(), newName, m_sceneManager);
            PushToSceneNodeStack(newNode);
        }
        else if (generatorName == "$$TreeGenerator1")
        {
            newNode = CreateSceneNode<scene_node::TreeGeneratorSceneNode>(GetParentSceneNode(), node->GetName());

            TreeData treeData;

            FillTreeDataMeshAndMaterial(GetB3dId(), node->GetBlockData(), treeData);
            treeData.rootNodes.push_back(newNode);
            treeData.name = node->GetName();
            treeData.location = node->GetBlockData().boundingSphere.center;

            m_trees.push_back(treeData);
        }
        else
        {
            D2_HACK_LOG(RoomVisitor::Visit) << "skipping unsupported generator: " << generatorName;
        }
    }
    else
    {
        if (generatorName == "$$GeneratorOfTerrain")
        {
            PopFromSceneNodeStack();
        }
    }

    return VisitResult::Continue;
}

const std::vector<TreeData>& RoomVisitor::GetTreeData()
{
    return m_trees;
}

struct TreeParams
{
    std::string materialName;
    std::string extraMaterialName;
    std::string meshName;
    float textureScale;
    float scale;
    Ogre::Vector3f location;
};

static void CalculateMaterialName(std::uint32_t type, std::string& materialName, std::string& extraMaterialName)
{
    const std::uint32_t subType = (type >> 4) & 0x0f;
    if ((subType % 2) == 0)
    {
        materialName = std::format("tree{}{}", subType, subType + 1);
    }
    else
    {
        materialName = std::format("tree{}{}", subType - 1, subType);
    }

    if ((type % 2) != 0)
    {
        extraMaterialName = std::format("tree{}", subType);
    }
    else
    {
        extraMaterialName = "";
    }
}

static TreeParams DeduceTreeParams(std::string_view b3dId, const block_data::SimpleGeneratedObjects40& data)
{
    TreeParams res;

    res.scale = data.boundingSphere.radius;
    res.location = data.boundingSphere.center;
    res.textureScale = 1.0f;

    CalculateMaterialName(data.unknown0.type, res.materialName, res.extraMaterialName);

    const std::uint32_t scaleType = (data.unknown0.type >> 8) & 0x0f;
    switch (scaleType)
    {
    case 0:
        res.textureScale = 0.5f;
        break;

    case 1:
        res.textureScale = 0.75f;
        break;

    case 2:
        res.textureScale = 0.6666667f;
        break;

    case 3:
        res.textureScale = 0.8f;
        break;

    case 4:
        res.textureScale = 0.8f;
        break;

    case 5:
        res.textureScale = 0.55f;
        break;

    case 6:
        res.textureScale = 1.0;
        break;

    default:
        OGRE_EXCEPT(Ogre::Exception::ERR_INVALID_STATE, std::format("Unknown scale type {}, type: {}", scaleType, data.unknown0.type));
    }

    D2_HACK_LOG(DeduceTreeParams) <<
        "data.unknown0.type: 0x" << std::hex << data.unknown0.type << ", "
        "material name: " << res.materialName << ", "
        "extra material name: " << res.extraMaterialName;


    res.meshName = "tree_mesh_" + res.materialName + "_" + std::to_string(res.scale) + "_" + std::to_string(res.textureScale);
    res.materialName = common::GetMaterialFileName(b3dId, res.materialName);
    res.extraMaterialName = common::GetMaterialFileName(b3dId, res.extraMaterialName);

    return res;
}

static Ogre::MeshPtr CreateMeshForTree(const TreeParams& treeParams, Ogre::SceneManager* sceneManager)
{
    Ogre::ManualObject* obj = sceneManager->createManualObject();

    obj->estimateVertexCount(16);
    obj->estimateIndexCount(48);

    obj->begin(treeParams.materialName, Ogre::RenderOperation::OT_TRIANGLE_LIST, common::DefaultResourceGroup);

    obj->position(treeParams.scale / 5 - 1, 0, 0);
    obj->textureCoord(treeParams.textureScale - 0.49f, 1.0f);

    obj->position(treeParams.scale / 5, 0, 0);
    obj->textureCoord(treeParams.textureScale - 0.05f, 1.0f);

    obj->position(treeParams.scale / 5 - 1, treeParams.scale, 0);
    obj->textureCoord(treeParams.textureScale - 0.49f, 0);

    obj->position(treeParams.scale / 5, treeParams.scale, 0);
    obj->textureCoord(treeParams.textureScale - 0.05f, 0);

    obj->position(0, 0, treeParams.scale / 5 * -1);
    obj->textureCoord(treeParams.textureScale - 0.05f, 1.0f);

    obj->position(0, 0, treeParams.scale / 5);
    obj->textureCoord(treeParams.textureScale - 0.49f, 1.0f);

    obj->position(0, treeParams.scale, treeParams.scale / 5 * -1);
    obj->textureCoord(treeParams.textureScale - 0.05f, 0);

    obj->position(0, treeParams.scale, treeParams.scale / 5);
    obj->textureCoord(treeParams.textureScale - 0.49f, 0);

    obj->position(treeParams.scale / 7 * -1, 0, treeParams.scale / 7 * -1);
    obj->textureCoord(treeParams.textureScale - 0.49f, 1.0f);

    obj->position(treeParams.scale / 7, 0, treeParams.scale / 7);
    obj->textureCoord(treeParams.textureScale - 0.05f, 1.0f);

    obj->position(treeParams.scale / 7 * -1, treeParams.scale, treeParams.scale / 7 * -1);
    obj->textureCoord(treeParams.textureScale - 0.49f, 0);

    obj->position(treeParams.scale / 7, treeParams.scale, treeParams.scale / 7);
    obj->textureCoord(treeParams.textureScale - 0.05f, 0);

    obj->position(treeParams.scale / 7 * -1, 0, treeParams.scale / 7);
    obj->textureCoord(treeParams.textureScale - 0.05f, 1.0f);

    obj->position(treeParams.scale / 7, 0, treeParams.scale / 7 * -1);
    obj->textureCoord(treeParams.textureScale - 0.49f, 1.0f);

    obj->position(treeParams.scale / 7 * -1, treeParams.scale, treeParams.scale / 7);
    obj->textureCoord(treeParams.textureScale - 0.05f, 0);

    obj->position(treeParams.scale / 7, treeParams.scale, treeParams.scale / 7 * -1);
    obj->textureCoord(treeParams.textureScale - 0.49f, 0);

    ///////////////////////////////////////////////////////////////////////////

    obj->triangle(0, 1, 2);
    obj->triangle(3, 2, 1);
    obj->triangle(0, 2, 1);
    obj->triangle(3, 1, 2);
    obj->triangle(4, 5, 6);
    obj->triangle(7, 6, 5);
    obj->triangle(4, 6, 5);
    obj->triangle(7, 5, 6);
    obj->triangle(8, 9, 10);
    obj->triangle(11, 10, 9);
    obj->triangle(8, 10, 9);
    obj->triangle(11, 9, 10);
    obj->triangle(12, 13, 14);
    obj->triangle(15, 14, 13);
    obj->triangle(12, 14, 13);
    obj->triangle(15, 13, 14);

    ///////////////////////////////////////////////////////////////////////////

    obj->end();

    auto res = obj->convertToMesh(treeParams.meshName);

    sceneManager->destroyManualObject(obj);

    return res;
}

void RoomVisitor::FillTreeDataMeshAndMaterial(std::string_view b3dId, const block_data::SimpleGeneratedObjects40& data, TreeData& treeData)
{
    TreeParams treeParams = DeduceTreeParams(b3dId, data);

    Ogre::MeshPtr treeMesh = Ogre::MeshManager::getSingleton().getByName(treeParams.meshName);

    if (!treeMesh)
    {
        D2_HACK_LOG(RoomVisitor::FillTreeDataMeshAndMaterial) << "cannot find mesh \"" << treeParams.meshName << "\", create new...";
        treeMesh = CreateMeshForTree(treeParams, m_sceneManager);
    }
    else
    {
        D2_HACK_LOG(RoomVisitor::FillTreeDataMeshAndMateria) << "reuse mesh \"" << treeParams.meshName << "\"";
    }

    treeData.mesh = treeMesh;
    treeData.materialName = treeParams.materialName;
}



} // namespace app
} // namespace d2_hack
