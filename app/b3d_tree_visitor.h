#ifndef D2_HACK_APP_B3D_TREE_VISITOR_H
#define D2_HACK_APP_B3D_TREE_VISITOR_H

#include <d2_hack/common/node_base.h>

#include <d2_hack/resource/data/b3d_visitor.h>
#include <d2_hack/app/b3d_car.h>

#include "b3d_scene_builder.h"

namespace d2_hack
{
namespace app
{

struct SceneNodeGenericAction
{
    template <typename VisitorType, typename NodePtrType>
    static resource::data::b3d::VisitResult Perform(VisitorType* self, NodePtrType& node, resource::data::b3d::VisitMode visitMode)
    {
        auto ogreSceneNode = self->GetSceneBuilder().ProcessOgreSceneNode(node->GetName(), visitMode);

        if (visitMode == resource::data::b3d::VisitMode::PreOrder)
        {
            auto parentB3dSceneNode = self->GetSceneBuilder().GetParentSceneNode();
            auto b3dSceneNode = self->CreateNode(*node, parentB3dSceneNode, ogreSceneNode);
            self->GetSceneBuilder().PushToSceneNodeStack(b3dSceneNode);
        }
        else
        {
            self->GetSceneBuilder().PopFromSceneNodeStack();
        }

        return resource::data::b3d::VisitResult::Continue;
    }
};

class B3dTreeVisitor : public resource::data::b3d::GenericActionVisitor<B3dTreeVisitor, SceneNodeGenericAction>
{
public:
    explicit B3dTreeVisitor(B3dSceneBuilder& sceneBuilder);

    B3dSceneBuilder& GetSceneBuilder();

    template <typename NodeType>
    scene_node::SceneNodeBasePtr CreateNode(const NodeType& node, const scene_node::SceneNodeBasePtr& /* parent */, Ogre::SceneNode* /* sceneNode */)
    {
        resource::data::b3d::RaiseExceptionAction::RaiseException(node.GetName(), node.GetType());

        return scene_node::SceneNodeBasePtr{};
    }

    scene_node::SceneNodeBasePtr CreateNode(const resource::data::b3d::NodeEventEntry& node, const scene_node::SceneNodeBasePtr& parent, Ogre::SceneNode* sceneNode);

    scene_node::SceneNodeBasePtr CreateNode(const resource::data::b3d::NodeGroupUnknown2& node, const scene_node::SceneNodeBasePtr& parent, Ogre::SceneNode* sceneNode);

    scene_node::SceneNodeBasePtr CreateNode(const resource::data::b3d::NodeGroupRoadInfraObjects4& node, const scene_node::SceneNodeBasePtr& parent, Ogre::SceneNode* sceneNode);

    scene_node::SceneNodeBasePtr CreateNode(const resource::data::b3d::NodeGroupObjects5& node, const scene_node::SceneNodeBasePtr& parent, Ogre::SceneNode* sceneNode);

    scene_node::SceneNodeBasePtr CreateNode(const resource::data::b3d::NodeGroupVertexData7& node, const scene_node::SceneNodeBasePtr& parent, Ogre::SceneNode* sceneNode);

    scene_node::SceneNodeBasePtr CreateNode(const resource::data::b3d::NodeSimpleFaces8& node, const scene_node::SceneNodeBasePtr& parent, Ogre::SceneNode* sceneNode);

    scene_node::SceneNodeBasePtr CreateNode(const resource::data::b3d::NodeGroupTrigger9& node, const scene_node::SceneNodeBasePtr& parent, Ogre::SceneNode* sceneNode);

    scene_node::SceneNodeBasePtr CreateNode(const resource::data::b3d::NodeGroupLodParameters10& node, const scene_node::SceneNodeBasePtr& parent, Ogre::SceneNode* sceneNode);

    scene_node::SceneNodeBasePtr CreateNode(const resource::data::b3d::NodeGroupUnknown12& node, const scene_node::SceneNodeBasePtr& parent, Ogre::SceneNode* sceneNode);

    scene_node::SceneNodeBasePtr CreateNode(const resource::data::b3d::NodeSimpleTrigger13& node, const scene_node::SceneNodeBasePtr& parent, Ogre::SceneNode* sceneNode);

    scene_node::SceneNodeBasePtr CreateNode(const resource::data::b3d::NodeSimpleUnknown14& node, const scene_node::SceneNodeBasePtr& parent, Ogre::SceneNode* sceneNode);

    scene_node::SceneNodeBasePtr CreateNode(const resource::data::b3d::NodeSimpleObjectConnector18& node, const scene_node::SceneNodeBasePtr& parent, Ogre::SceneNode* sceneNode);

    scene_node::SceneNodeBasePtr CreateNode(const resource::data::b3d::NodeGroupObjects19& node, const scene_node::SceneNodeBasePtr& parent, Ogre::SceneNode* sceneNode);

    scene_node::SceneNodeBasePtr CreateNode(const resource::data::b3d::NodeSimpleFlatCollision20& node, const scene_node::SceneNodeBasePtr& parent, Ogre::SceneNode* sceneNode);

    scene_node::SceneNodeBasePtr CreateNode(const resource::data::b3d::NodeGroupObjects21& node, const scene_node::SceneNodeBasePtr& parent, Ogre::SceneNode* sceneNode);

    scene_node::SceneNodeBasePtr CreateNode(const resource::data::b3d::NodeSimpleVolumeCollision23& node, const scene_node::SceneNodeBasePtr& parent, Ogre::SceneNode* sceneNode);

    scene_node::SceneNodeBasePtr CreateNode(const resource::data::b3d::NodeSimpleUnknown25& node, const scene_node::SceneNodeBasePtr& parent, Ogre::SceneNode* sceneNode);

    scene_node::SceneNodeBasePtr CreateNode(const resource::data::b3d::NodeSimpleFaces28& node, const scene_node::SceneNodeBasePtr& parent, Ogre::SceneNode* sceneNode);

    scene_node::SceneNodeBasePtr CreateNode(const resource::data::b3d::NodeGroupUnknown29& node, const scene_node::SceneNodeBasePtr& parent, Ogre::SceneNode* sceneNode);

    scene_node::SceneNodeBasePtr CreateNode(const resource::data::b3d::NodeSimplePortal30& node, const scene_node::SceneNodeBasePtr& parent, Ogre::SceneNode* sceneNode);

    scene_node::SceneNodeBasePtr CreateNode(const resource::data::b3d::NodeGroupLightingObjects33& node, const scene_node::SceneNodeBasePtr& parent, Ogre::SceneNode* sceneNode);

    scene_node::SceneNodeBasePtr CreateNode(const resource::data::b3d::NodeSimpleUnknown34& node, const scene_node::SceneNodeBasePtr& parent, Ogre::SceneNode* sceneNode);

    scene_node::SceneNodeBasePtr CreateNode(const resource::data::b3d::NodeSimpleFaces35& node, const scene_node::SceneNodeBasePtr& parent, Ogre::SceneNode* sceneNode);

    scene_node::SceneNodeBasePtr CreateNode(const resource::data::b3d::NodeGroupVertexData37& node, const scene_node::SceneNodeBasePtr& parent, Ogre::SceneNode* sceneNode);

    scene_node::SceneNodeBasePtr CreateNode(const resource::data::b3d::NodeSimpleGeneratedObjects40& node, const scene_node::SceneNodeBasePtr& parent, Ogre::SceneNode* sceneNode);

private:
    B3dSceneBuilder& m_sceneBuilder;

    template <typename FacesNode>
    void VisitFaces(FacesNode& block);
};

struct WheelData
{
    scene_node::SceneNodeBaseList rootNodes;
    Ogre::MeshPtr mesh;
    std::string name;
    WheelId id;
    resource::data::b3d::TransformList transformations;
};



class GameObjectVisitorBase : public resource::data::b3d::RaiseExceptionVisitor
{
public:

    using VisitResult = resource::data::b3d::VisitResult;
    using VisitMode = resource::data::b3d::VisitMode;

    GameObjectVisitorBase(std::string_view b3dId,
                          std::string_view blockName,
                          Ogre::MeshManager* meshManager,
                          resource::archive::res::OgreMaterialProvider* ogreMaterialProvider);

    virtual VisitResult Visit(const std::shared_ptr<resource::data::b3d::NodeEventEntry>& node, VisitMode visitMode) override;

    virtual VisitResult Visit(const std::shared_ptr<resource::data::b3d::NodeGroupUnknown2>& node, VisitMode visitMode) override;

    virtual VisitResult Visit(const std::shared_ptr<resource::data::b3d::NodeGroupObjects5>& node, VisitMode visitMode) override;

    virtual VisitResult Visit(const std::shared_ptr<resource::data::b3d::NodeSimpleFaces8>& node, VisitMode visitMode) override;

    virtual VisitResult Visit(const std::shared_ptr<resource::data::b3d::NodeSimpleUnknown14>& node, VisitMode visitMode) override;

    virtual VisitResult Visit(const std::shared_ptr<resource::data::b3d::NodeSimpleObjectConnector18>& node, VisitMode visitMode) override;

    virtual VisitResult Visit(const std::shared_ptr<resource::data::b3d::NodeGroupObjects21>& node, VisitMode visitMode) override;

    virtual VisitResult Visit(const std::shared_ptr<resource::data::b3d::NodeSimpleVolumeCollision23>& node, VisitMode visitMode) override;

    virtual VisitResult Visit(const std::shared_ptr<resource::data::b3d::NodeSimpleUnknown25>& node, VisitMode visitMode) override;

    virtual VisitResult Visit(const std::shared_ptr<resource::data::b3d::NodeSimpleFaces28>& node, VisitMode visitMode) override;

    virtual VisitResult Visit(const std::shared_ptr<resource::data::b3d::NodeGroupLightingObjects33>& node, VisitMode visitMode) override;
    
    virtual VisitResult Visit(const std::shared_ptr<resource::data::b3d::NodeSimpleFaces35>& node, VisitMode visitMode) override;

    std::string_view GetB3dId() const;
    
    std::string_view GetBlockName() const;

    Ogre::MeshManager* GetMeshManager();

    resource::archive::res::OgreMaterialProvider* GetMaterialProvider();
    
    const scene_node::SceneNodeBaseList& GetRootSceneNodes() const;

    Ogre::MeshPtr GetMesh() const;
private:
    const std::string_view m_b3dId;
    const std::string_view m_blockName;

    resource::archive::res::OgreMaterialProvider* m_ogreMaterialProvider;
    Ogre::MeshManager* m_meshManager;

    Ogre::MeshPtr m_mesh;
    scene_node::SceneNodeBaseList m_rootSceneNodes;

    std::stack<scene_node::SceneNodeBasePtr> m_sceneNodesStack;
    std::vector<resource::data::b3d::TransformList> m_transformEntries;

    void PushToSceneNodeStack(const scene_node::SceneNodeBasePtr& node);

    void PopFromSceneNodeStack();

    scene_node::SceneNodeBasePtr GetParentSceneNode();

    static Ogre::MeshPtr CreateMesh(std::string_view b3dId, std::string_view blockName, Ogre::MeshManager* meshManager);

    size_t CreateSubMesh(const Ogre::MaterialPtr& material);

    size_t AddSimpleMeshInfo(const common::SimpleMeshInfo& meshInfo, const std::string& materialName);

    void ManagePositions(Ogre::VertexData* vertexData, const common::PositionList& positions, unsigned short bufferIndex);

    void ManageTexCoords(Ogre::VertexData* vertexData, const common::TexCoordList& texCoords, unsigned short bufferIndex);

    void ManageNormals(Ogre::VertexData* vertexData, const common::NormalList& normals, unsigned short bufferIndex);

    void ApplyTransformations(const common::PositionList& original, common::PositionList& transformed);

    template <typename FacesType>
    VisitResult VisitFaces(FacesType& node, VisitMode visitMode);
};


class WheelVisitor : public GameObjectVisitorBase
{
public:

    using VisitResult = resource::data::b3d::VisitResult;
    using VisitMode = resource::data::b3d::VisitMode;

    WheelVisitor(std::string_view b3dId,
                 std::string_view blockName,
                 Ogre::MeshManager* meshManager,
                 resource::archive::res::OgreMaterialProvider* ogreMaterialProvider);

    virtual VisitResult Visit(const std::shared_ptr<resource::data::b3d::NodeGroupObjects5>& node, VisitMode visitMode) override;

    virtual VisitResult Visit(const std::shared_ptr<resource::data::b3d::NodeSimpleObjectConnector18>& node, VisitMode visitMode) override;

    const WheelData& GetWheelData();

private:
    WheelData m_wheelData;
    bool m_topLevelBlockConnectorPreVisited;
    bool m_topLevelBlockConnectorPostVisited;
};


class WheelBasedMoveableObjectVisitor : public GameObjectVisitorBase
{
public:

    using VisitResult = resource::data::b3d::VisitResult;
    using VisitMode = resource::data::b3d::VisitMode;

    WheelBasedMoveableObjectVisitor(std::string_view b3dId,
                                    std::string_view blockName,
                                    Ogre::MeshManager* meshManager,
                                    resource::archive::res::OgreMaterialProvider* ogreMaterialProvider);

    virtual VisitResult Visit(const std::shared_ptr<resource::data::b3d::NodeGroupObjects5>& node, VisitMode visitMode) override;

    const std::vector<WheelData>& GetWheelData() const;

private:
    std::vector<WheelData> m_wheelRootSceneNodes;
};


} // namespace app
} // namespace d2_hack

#endif /* D2_HACK_APP_B3D_TREE_VISITOR_H */
