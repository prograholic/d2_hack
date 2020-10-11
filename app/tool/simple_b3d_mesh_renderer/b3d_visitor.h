#ifndef D2_HACK_APP_TOOL_SIMPLE_B3D_MESH_RENDERER_B3D_VISITOR_H
#define D2_HACK_APP_TOOL_SIMPLE_B3D_MESH_RENDERER_B3D_VISITOR_H

#include <cassert>
#include <stack>

#include <d2_hack/common/platform.h>

D2_HACK_DISABLE_WARNING_BEGIN(4251)

#include <OgreMatrix3.h>
#include <OgreRenderOperation.h>

D2_HACK_DISABLE_WARNING_END() // 4251


#include <d2_hack/resource/data/b3d_reader.h>

namespace d2_hack
{
namespace app
{


struct SubMeshInfo
{
    bool useSharedVertices;
    std::uint32_t materialIndex;
    Ogre::RenderOperation::OperationType operationType;
};



class B3dTreeVisitor : public resource::data::b3d::NodeVisitorInterface
{
public:
    B3dTreeVisitor(const char* b3dId,
                   const std::string& b3dName,
                   Ogre::SceneManager* sceneManager,
                   Ogre::SceneNode* rootNode,
                   Ogre::MeshManager* meshManager,
                   const common::Materials& materials);

    ~B3dTreeVisitor();

    virtual void Visit(const std::string& name, const resource::data::b3d::block_data::Empty0& block, resource::data::b3d::VisitMode visitMode) override;

    virtual void Visit(const std::string& name, const resource::data::b3d::block_data::GroupRoadInfraObjects4& block, resource::data::b3d::VisitMode visitMode) override;

    virtual void Visit(const std::string& name, const resource::data::b3d::block_data::GroupObjects5& block, resource::data::b3d::VisitMode visitMode) override;

    virtual void Visit(const std::string& name, const resource::data::b3d::block_data::GroupVertex7& block, resource::data::b3d::VisitMode visitMode) override;

    virtual void Visit(const std::string& name, const resource::data::b3d::block_data::SimpleFaces8& block, resource::data::b3d::VisitMode visitMode) override;

    virtual void Visit(const std::string& name, const resource::data::b3d::block_data::GroupTrigger9& block, resource::data::b3d::VisitMode visitMode) override;

    virtual void Visit(const std::string& name, const resource::data::b3d::block_data::GroupLodParameters10& block, resource::data::b3d::VisitMode visitMode) override;

    virtual void Visit(const std::string& name, const resource::data::b3d::block_data::GroupUnknown12& block, resource::data::b3d::VisitMode visitMode) override;

    virtual void Visit(const std::string& name, const resource::data::b3d::block_data::SimpleTrigger13& block, resource::data::b3d::VisitMode visitMode) override;

    virtual void Visit(const std::string& name, const resource::data::b3d::block_data::SimpleUnknown14& block, resource::data::b3d::VisitMode visitMode) override;

    virtual void Visit(const std::string& name, const resource::data::b3d::block_data::SimpleObjectConnector18& block, resource::data::b3d::VisitMode visitMode) override;

    virtual void Visit(const std::string& name, const resource::data::b3d::block_data::GroupObjects19& block, resource::data::b3d::VisitMode visitMode) override;

    virtual void Visit(const std::string& name, const resource::data::b3d::block_data::SimpleFlatCollision20& block, resource::data::b3d::VisitMode visitMode) override;

    virtual void Visit(const std::string& name, const resource::data::b3d::block_data::GroupObjects21& block, resource::data::b3d::VisitMode visitMode) override;

    virtual void Visit(const std::string& name, const resource::data::b3d::block_data::SimpleVolumeCollision23& block, resource::data::b3d::VisitMode visitMode) override;

    virtual void Visit(const std::string& name, const resource::data::b3d::block_data::GroupTransformMatrix24& block, resource::data::b3d::VisitMode visitMode) override;

    virtual void Visit(const std::string& name, const resource::data::b3d::block_data::SimpleFaces28& block, resource::data::b3d::VisitMode visitMode) override;

    virtual void Visit(const std::string& name, const resource::data::b3d::block_data::GroupUnknown29& block, resource::data::b3d::VisitMode visitMode) override;

    virtual void Visit(const std::string& name, const resource::data::b3d::block_data::SimplePortal30& block, resource::data::b3d::VisitMode visitMode) override;

    virtual void Visit(const std::string& name, const resource::data::b3d::block_data::GroupLightingObjects33& block, resource::data::b3d::VisitMode visitMode) override;

    virtual void Visit(const std::string& name, const resource::data::b3d::block_data::SimpleFaceData35& block, resource::data::b3d::VisitMode visitMode) override;

    virtual void Visit(const std::string& name, const resource::data::b3d::block_data::GroupVertexData37& block, resource::data::b3d::VisitMode visitMode) override;

    virtual void Visit(const std::string& name, const resource::data::b3d::block_data::SimpleGeneratedObjects40& block, resource::data::b3d::VisitMode visitMode) override;

private:
    std::string m_b3dId;
    std::string m_b3dName;
    Ogre::SceneManager* m_sceneManager;
    Ogre::SceneNode* m_rootNode;
    Ogre::MeshManager* m_meshManager;
    const common::Materials m_materials;

    std::stack<Ogre::SceneNode*> m_sceneNodes;
    std::stack<Ogre::MeshPtr> m_meshStack;
    std::stack<resource::data::b3d::block_data::GroupLodParameters10> m_currentLods;
    std::list<resource::data::b3d::block_data::GroupTransformMatrix24> m_transformQueue;

    struct Transform
    {
        Ogre::Matrix3 matrix;
        Ogre::Vector3 position;
    };
    typedef std::list<Transform> TransformList;
    std::map<std::string, TransformList> m_transformMap;

    std::string GetB3dResourceId(const std::string& name) const;

    std::string GetB3dResourceId(const common::ResourceName& name) const;

    std::string GetMaterialName(const std::uint32_t materialIndex) const;

    std::string GetNameImpl(const std::string& blockName, const std::string& subName, bool forceUnique) const;

    Ogre::SceneNode* ProcessSceneNode(const std::string& name, resource::data::b3d::VisitMode visitMode);

    Ogre::MeshPtr ProcessMesh(const std::string& name, resource::data::b3d::VisitMode visitMode);

    void AddVertexData(const Ogre::MeshPtr& mesh, const common::PositionWithTexCoordList& data);

    void AddVertexData(const Ogre::MeshPtr& mesh, const common::PositionWithNormalList& data);

    void AddVertexData(const Ogre::MeshPtr& mesh, const common::PositionWithTexCoordNormalList& data);

    void AddVertexData(const Ogre::MeshPtr& mesh, const std::vector<resource::data::b3d::block_data::GroupVertexData37::Unknown514>& data);

    void AddVertexData(const Ogre::MeshPtr& mesh, const std::vector<resource::data::b3d::block_data::GroupVertexData37::Unknown258>& data);

    Ogre::SubMesh* CreateSubMesh(const SubMeshInfo& subMeshInfo);

    void SetSubMeshData(Ogre::SubMesh* subMesh, const resource::data::b3d::block_data::Face28Entry& face);

    void SetSubMeshData(Ogre::SubMesh* subMesh, const common::IndexList& data);

    void SetSubMeshData(Ogre::SubMesh* subMesh, const common::IndexWithTexCoordList& data);

    void SetSubMeshData(Ogre::SubMesh* subMesh, const common::IndexWithPositionList& data);

    void SetSubMeshData(Ogre::SubMesh* subMesh, const common::IndexWithPositionTexCoordList& data);

    void SetSubMeshData(Ogre::SubMesh* subMesh, const std::vector<resource::data::b3d::block_data::Mesh35::Unknown49>& data);

    void ManageSubMeshIndexBuffer(Ogre::SubMesh* subMesh, const common::IndexList& indices);
};

} // namespace app
} // namespace d2_hack

#endif /* D2_HACK_APP_TOOL_SIMPLE_B3D_MESH_RENDERER_B3D_VISITOR_H */
