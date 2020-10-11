#ifndef D2_HACK_APP_TOOL_SIMPLE_B3D_MESH_RENDERER_B3D_LISTENER_H
#define D2_HACK_APP_TOOL_SIMPLE_B3D_MESH_RENDERER_B3D_LISTENER_H

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

    virtual void Visit(const std::string& /* name */, const resource::data::b3d::block_data::Empty0& /* block */, resource::data::b3d::VisitMode /* visitMode */) override;

    virtual void Visit(const std::string& /* name */, const resource::data::b3d::block_data::GroupRoadInfraObjects4& /* block */, resource::data::b3d::VisitMode /* visitMode */) override;

    virtual void Visit(const std::string& /* name */, const resource::data::b3d::block_data::GroupObjects5& /* block */, resource::data::b3d::VisitMode /* visitMode */) override;

    virtual void Visit(const std::string& /* name */, const resource::data::b3d::block_data::GroupVertex7& /* block */, resource::data::b3d::VisitMode /* visitMode */) override;

    virtual void Visit(const std::string& /* name */, const resource::data::b3d::block_data::SimpleFaces8& /* block */, resource::data::b3d::VisitMode /* visitMode */) override;

    virtual void Visit(const std::string& /* name */, const resource::data::b3d::block_data::GroupTrigger9& /* block */, resource::data::b3d::VisitMode /* visitMode */) override;

    virtual void Visit(const std::string& /* name */, const resource::data::b3d::block_data::GroupLodParameters10& /* block */, resource::data::b3d::VisitMode /* visitMode */) override;

    virtual void Visit(const std::string& /* name */, const resource::data::b3d::block_data::GroupUnknown12& /* block */, resource::data::b3d::VisitMode /* visitMode */) override;

    virtual void Visit(const std::string& /* name */, const resource::data::b3d::block_data::SimpleTrigger13& /* block */, resource::data::b3d::VisitMode /* visitMode */) override;

    virtual void Visit(const std::string& /* name */, const resource::data::b3d::block_data::SimpleUnknown14& /* block */, resource::data::b3d::VisitMode /* visitMode */) override;

    virtual void Visit(const std::string& /* name */, const resource::data::b3d::block_data::SimpleObjectConnector18& /* block */, resource::data::b3d::VisitMode /* visitMode */) override;

    virtual void Visit(const std::string& /* name */, const resource::data::b3d::block_data::GroupObjects19& /* block */, resource::data::b3d::VisitMode /* visitMode */) override;

    virtual void Visit(const std::string& /* name */, const resource::data::b3d::block_data::SimpleFlatCollision20& /* block */, resource::data::b3d::VisitMode /* visitMode */) override;

    virtual void Visit(const std::string& /* name */, const resource::data::b3d::block_data::GroupObjects21& /* block */, resource::data::b3d::VisitMode /* visitMode */) override;

    virtual void Visit(const std::string& /* name */, const resource::data::b3d::block_data::SimpleVolumeCollision23& /* block */, resource::data::b3d::VisitMode /* visitMode */) override;

    virtual void Visit(const std::string& /* name */, const resource::data::b3d::block_data::GroupTransformMatrix24& /* block */, resource::data::b3d::VisitMode /* visitMode */) override;

    virtual void Visit(const std::string& /* name */, const resource::data::b3d::block_data::SimpleFaces28& /* block */, resource::data::b3d::VisitMode /* visitMode */) override;

    virtual void Visit(const std::string& /* name */, const resource::data::b3d::block_data::GroupUnknown29& /* block */, resource::data::b3d::VisitMode /* visitMode */) override;

    virtual void Visit(const std::string& /* name */, const resource::data::b3d::block_data::SimplePortal30& /* block */, resource::data::b3d::VisitMode /* visitMode */) override;

    virtual void Visit(const std::string& /* name */, const resource::data::b3d::block_data::GroupLightingObjects33& /* block */, resource::data::b3d::VisitMode /* visitMode */) override;

    virtual void Visit(const std::string& /* name */, const resource::data::b3d::block_data::SimpleFaceData35& /* block */, resource::data::b3d::VisitMode /* visitMode */) override;

    virtual void Visit(const std::string& /* name */, const resource::data::b3d::block_data::GroupVertexData37& /* block */, resource::data::b3d::VisitMode /* visitMode */) override;

    virtual void Visit(const std::string& /* name */, const resource::data::b3d::block_data::SimpleGeneratedObjects40& /* block */, resource::data::b3d::VisitMode /* visitMode */) override;

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

    void BeginSceneNode(const std::string& name);

    void EndSceneNode();

    Ogre::MeshPtr BeginMesh(const std::string& name);

    void EndMesh();

    void AddVertexData(const Ogre::MeshPtr& mesh, const common::PositionWithTexCoordList& data);

    void AddVertexData(const Ogre::MeshPtr& mesh, const common::PositionWithNormalList& data);

    void AddVertexData(const Ogre::MeshPtr& mesh, const common::PositionWithTexCoordNormalList& data);

    void AddVertexData(const Ogre::MeshPtr& mesh, const std::vector<resource::data::b3d::block_data::GroupVertexData37::Unknown514>& data);

    void AddVertexData(const Ogre::MeshPtr& mesh, const std::vector<resource::data::b3d::block_data::GroupVertexData37::Unknown258>& data);

    Ogre::SubMesh* CreateSubMesh(bool useSharedVertices, std::uint32_t materialIndex, Ogre::RenderOperation::OperationType operationType);

    void SetSubMeshData(Ogre::SubMesh* subMesh, const resource::data::b3d::block_data::Face28Entry& face);

    void SetSubMeshData(Ogre::SubMesh* subMesh, const common::IndexList& data);

    void SetSubMeshData(Ogre::SubMesh* subMesh, const common::IndexWithTexCoordList& data);

    void SetSubMeshData(Ogre::SubMesh* subMesh, const common::IndexWithPositionList& data);

    void SetSubMeshData(Ogre::SubMesh* subMesh, const common::IndexWithPositionTexCoordList& data);

    void SetSubMeshData(Ogre::SubMesh* subMesh, const std::vector<resource::data::b3d::block_data::Mesh35::Unknown49>& data);

    void ManageSubMeshIndexBuffer(Ogre::SubMesh* subMesh, const common::IndexList& indices);
};

#if 0
struct B3dMeshListener : public resource::data::b3d::SimpleActionB3dListener<AssertB3dAction>
{
    B3dMeshListener(const char* b3dId,
                    const std::string& b3dName,
                    Ogre::SceneManager* sceneManager,
                    Ogre::SceneNode* rootNode,
                    Ogre::MeshManager* meshManager);

    ~B3dMeshListener();
    
    virtual void OnMaterials(common::Materials&& materials) override;

    ///////////////////////////////////////////////////////////////////////////

    virtual void OnBlockBegin(const resource::data::b3d::block_data::BlockHeader& blockHeader) override;

    virtual void OnBlockEnd(const resource::data::b3d::block_data::BlockHeader& blockHeader) override;

    virtual void OnNestedBlockBegin(std::uint32_t nestedBlockNumber) override;

    virtual void OnNestedBlockEnd(std::uint32_t nestedBlockNumber) override;

    virtual void OnBlock(const resource::data::b3d::block_data::Empty0& block) override;

    virtual void OnBlock(const resource::data::b3d::block_data::GroupRoadInfraObjects4& block) override;

    virtual void OnBlock(const resource::data::b3d::block_data::GroupObjects5& block) override;

    virtual void OnBlock(const resource::data::b3d::block_data::GroupVertex7& /* block */) override;

    virtual void OnBlock(const resource::data::b3d::block_data::SimpleFaces8& /* block */) override;

    virtual void OnBlock(const resource::data::b3d::block_data::GroupTrigger9& /* block */) override;

    virtual void OnBlock(const resource::data::b3d::block_data::GroupLodParameters10& block) override;

    virtual void OnBlock(const resource::data::b3d::block_data::GroupUnknown12& /* block */) override;

    virtual void OnBlock(const resource::data::b3d::block_data::SimpleTrigger13& /* block */) override;

    virtual void OnBlock(const resource::data::b3d::block_data::SimpleUnknown14& /* block */) override;

    virtual void OnBlock(const resource::data::b3d::block_data::SimpleObjectConnector18& block) override;

    virtual void OnBlock(const resource::data::b3d::block_data::GroupObjects19& /* block */) override;

    virtual void OnBlock(const resource::data::b3d::block_data::SimpleFlatCollision20& /* block */) override;

    virtual void OnBlock(const resource::data::b3d::block_data::GroupObjects21& /* block */) override;

    virtual void OnBlock(const resource::data::b3d::block_data::SimpleVolumeCollision23& /* block */) override;

    virtual void OnBlock(const resource::data::b3d::block_data::GroupTransformMatrix24& block) override;

    virtual void OnBlock(const resource::data::b3d::block_data::SimpleFaces28& /* block */) override;

    virtual void OnBlock(const resource::data::b3d::block_data::GroupUnknown29& /* block */) override;

    virtual void OnBlock(const resource::data::b3d::block_data::SimplePortal30& /* block */) override;

    virtual void OnBlock(const resource::data::b3d::block_data::GroupLightingObjects33& block) override;

    virtual void OnBlock(const resource::data::b3d::block_data::SimpleFaceData35& block) override;

    virtual void OnBlock(const resource::data::b3d::block_data::GroupVertexData37& /* block */) override;

    virtual void OnBlock(const resource::data::b3d::block_data::SimpleGeneratedObjects40& /* block */) override;

    ///////////////////////////////////////////////////////////////////////////

    virtual void OnData(common::PositionWithTexCoordNormalList&& data) override;

    virtual void OnData(common::PositionWithTexCoordList&& data) override;

    virtual void OnData(common::PositionWithNormalList&& data) override;

    virtual void OnData(resource::data::b3d::block_data::Mesh35&& data) override;

    virtual void OnData(common::IndexList&& data) override;

    virtual void OnData(common::IndexWithTexCoordList&& data) override;

    virtual void OnData(common::IndexWithPositionList&& data) override;

    virtual void OnData(common::IndexWithPositionTexCoordList&& data) override;

    virtual void OnData(resource::data::b3d::block_data::Face8&& data) override;

    virtual void OnData(common::CollisionPositionList&& /* data */) override;

    virtual void OnData(common::CollisionUnknownList&& /* data */) override;

    virtual void OnData(common::TriggerInfoList&& /* data */) override;

    virtual void OnData(common::GeneratedObjectInfo&& /* data */) override;

    virtual void OnData(resource::data::b3d::block_data::Face28Entry&& data) override;

    virtual void OnData(std::vector<resource::data::b3d::block_data::Face28Entry::Unknown>&& data) override;

    virtual void OnData(std::vector<resource::data::b3d::block_data::GroupVertexData37::Unknown258>&& data) override;

    virtual void OnData(std::vector<resource::data::b3d::block_data::GroupVertexData37::Unknown514>&& data) override;

    virtual void OnData(std::vector<resource::data::b3d::block_data::Mesh35::Unknown49>&& data) override;

private:
    std::string m_b3dId;
    std::string m_b3dName;
    Ogre::SceneManager* m_sceneManager;
    Ogre::SceneNode* m_rootNode;
    Ogre::MeshManager* m_meshManager;

    


    

    std::stack<std::string> m_blockNames;

    

    

    

    std::queue<std::uint32_t> m_data35TypeQueue;
    

    void ProcessTransformQueue();

    void BeginSceneNode();
    
    void EndSceneNode();

    void BeginMesh(bool shouldHasName);

    void EndMesh();

    void CreateSubMesh(bool useSharedVertices, std::uint32_t materialIndex, Ogre::RenderOperation::OperationType operationType);

    void ManageSubMeshIndexBuffer(common::IndexList&& indices, Ogre::SubMesh* subMesh);
};
    
#endif //0

} // namespace app
} // namespace d2_hack

#endif /* D2_HACK_APP_TOOL_SIMPLE_B3D_MESH_RENDERER_B3D_LISTENER_H */
