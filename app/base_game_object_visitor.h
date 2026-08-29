#ifndef D2_HACK_APP_BASE_GAME_OBJECT_VISITOR_H
#define D2_HACK_APP_BASE_GAME_OBJECT_VISITOR_H

#include <d2_hack/common/platform.h>

#include <stack>

#include <d2_hack/resource/data/b3d_visitor.h>
#include <d2_hack/resource/archive/ogre_material_provider.h>

#include <d2_hack/scene_node/scene_node_base.h>

namespace d2_hack
{
namespace app
{

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

    const scene_node::SceneNodeBaseList& GetRootSceneNodes() const;

    Ogre::MeshPtr GetMesh() const;

protected:
    void PushToSceneNodeStack(const scene_node::SceneNodeBasePtr& node);

    void PopFromSceneNodeStack();

    Ogre::MeshManager* GetMeshManager();

    resource::archive::res::OgreMaterialProvider* GetMaterialProvider();

    scene_node::SceneNodeBasePtr GetParentSceneNode();

private:
    const std::string_view m_b3dId;
    const std::string_view m_blockName;

    resource::archive::res::OgreMaterialProvider* m_ogreMaterialProvider;
    Ogre::MeshManager* m_meshManager;

    Ogre::MeshPtr m_mesh;
    scene_node::SceneNodeBaseList m_rootSceneNodes;

    std::stack<scene_node::SceneNodeBasePtr> m_sceneNodesStack;
    std::vector<resource::data::b3d::TransformList> m_transformEntries;

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

} // namespace app
} // namespace d2_hack

#endif /* D2_HACK_APP_BASE_GAME_OBJECT_VISITOR_H */
