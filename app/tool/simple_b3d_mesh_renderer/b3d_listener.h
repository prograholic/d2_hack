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

struct AssertB3dAction
{
    static void OnBlockBegin(const resource::data::b3d::block_data::BlockHeader& /* blockHeader */)
    {
        assert(0 && "OnBlockBegin is not implemented");
    }

    static void OnBlockEnd(const resource::data::b3d::block_data::BlockHeader& /* blockHeader */)
    {
        assert(0 && "OnBlockEnd is not implemented");
    }

    static void OnNestedBlockBegin(std::uint32_t /* nestedBlockNumber */)
    {
        assert(0 && "OnNestedBlockBegin is not implemented");
    }

    static void OnNestedBlockEnd(std::uint32_t /* nestedBlockNumber */)
    {
        assert(0 && "OnNestedBlockEnd is not implemented");
    }

    static void OnMaterials(resource::data::b3d::Materials&& /* materials */)
    {
        assert(0 && "OnMaterials is not implemented");
    }

    template <typename T>
    static void OnBlock(const T& /* block */)
    {
        assert(0 && "OnBlock is not implemented");
    }

    template <typename T>
    static void OnData(T&& /* data */)
    {
        assert(0 && "OnData is not implemented");
    }
};

struct B3dMeshListener : public resource::data::b3d::SimpleActionB3dListener<AssertB3dAction>
{
    B3dMeshListener(const char* b3dId,
                    const std::string& b3dName,
                    Ogre::SceneManager* sceneManager,
                    Ogre::SceneNode* rootNode,
                    Ogre::MeshManager* meshManager);

    ~B3dMeshListener();
    
    virtual void OnMaterials(resource::data::b3d::Materials&& materials) override;

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

    std::stack<Ogre::MeshPtr> m_meshQueue;

    struct Transform
    {
        Ogre::Matrix3 matrix;
        Ogre::Vector3 position;
    };
    typedef std::list<Transform> TransformList;

    std::map<std::string, TransformList> m_transformMap;
    std::list<resource::data::b3d::block_data::GroupTransformMatrix24> m_transformQueue;

    std::stack<std::string> m_blockNames;

    std::stack<resource::data::b3d::block_data::GroupLodParameters10> m_currentLods;

    std::stack<Ogre::SceneNode*> m_sceneNodes;

    resource::data::b3d::Materials m_materials;

    std::queue<std::uint32_t> m_data35TypeQueue;
    
    std::string GetB3dResourceId(const std::string& name) const;

    std::string GetB3dResourceId(const common::ResourceName& name) const;

    std::string GetMaterialName(const std::uint32_t materialIndex) const;

    static std::string GetNameImpl(const std::string& blockName, const std::string& subName, bool forceUnique);

    std::string GetName(const std::string& subname, bool forceUnique) const;

    void ProcessTransformQueue();

    void BeginSceneNode();
    
    void EndSceneNode();

    void BeginMesh(bool shouldHasName);

    void EndMesh();

    void CreateSubMesh(bool useSharedVertices, std::uint32_t materialIndex, Ogre::RenderOperation::OperationType operationType);
};
    

} // namespace app
} // namespace d2_hack

#endif /* D2_HACK_APP_TOOL_SIMPLE_B3D_MESH_RENDERER_B3D_LISTENER_H */
