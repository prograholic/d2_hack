#ifndef D2_HACK_APP_TOOL_SIMPLE_B3D_MESH_RENDERER_B3D_SCENE_BUILDER_H
#define D2_HACK_APP_TOOL_SIMPLE_B3D_MESH_RENDERER_B3D_SCENE_BUILDER_H

#include <d2_hack/common/platform.h>

#include <stack>
#include <list>

D2_HACK_DISABLE_WARNING_BEGIN(4251)
#include <OgreRenderOperation.h>
#include <OgreMatrix3.h>
D2_HACK_DISABLE_WARNING_END() // 4251

#include <d2_hack/common/types.h>
#include <d2_hack/resource/data/b3d_types.h>

namespace d2_hack
{
namespace app
{


struct Transform
{
    Ogre::Matrix3 matrix;
    Ogre::Vector3 position;
};
typedef std::vector<Transform> TransformList;

class B3dSceneBuilder
{
public:
    B3dSceneBuilder(const char* b3dId,
                   const std::string& b3dName,
                   Ogre::SceneManager* sceneManager,
                   Ogre::SceneNode* rootNode,
                   Ogre::MeshManager* meshManager,
                   const common::Materials& materials);

    ~B3dSceneBuilder();

    void ProcessLod(const resource::data::b3d::block_data::GroupLodParameters10& block, resource::data::b3d::VisitMode visitMode);

    void ProcessTransformQueue(const std::string& name, const resource::data::b3d::block_data::GroupTransformMatrix24& block, resource::data::b3d::VisitMode visitMode);

    void ProcessLight(const std::string& name, const resource::data::b3d::block_data::GroupLightingObjects33& block, resource::data::b3d::VisitMode visitMode);

    void ProcessObjectConnector(const resource::data::b3d::block_data::SimpleObjectConnector18& block, resource::data::b3d::VisitMode visitMode);

    Ogre::SceneNode* ProcessSceneNode(const std::string& name, resource::data::b3d::VisitMode visitMode);

    Ogre::MeshPtr ProcessMesh(const std::string& name, resource::data::b3d::VisitMode visitMode);

    void AddVertexData(const Ogre::MeshPtr& mesh, const common::PositionWithTexCoordList& data);

    void AddVertexData(const Ogre::MeshPtr& mesh, const common::PositionWithNormalList& data);

    void AddVertexData(const Ogre::MeshPtr& mesh, const common::PositionWithTexCoordNormalList& data);

    void AddVertexData(const Ogre::MeshPtr& mesh, const std::vector<resource::data::b3d::block_data::GroupVertexData37::Unknown514>& data);

    void AddVertexData(const Ogre::MeshPtr& mesh, const std::vector<resource::data::b3d::block_data::GroupVertexData37::Unknown258Or515>& data);

    Ogre::SubMesh* CreateSubMesh(std::uint32_t materialIndex);

    void SetSubMeshData(Ogre::SubMesh* subMesh, const std::vector<resource::data::b3d::block_data::Face28::Unknown>& data, const common::IndexList& indices);

    void SetSubMeshData(Ogre::SubMesh* subMesh, const common::IndexList& data, const common::IndexList& indices);

    void SetSubMeshData(Ogre::SubMesh* subMesh, const common::IndexWithTexCoordList& data, const common::IndexList& indices);

    void SetSubMeshData(Ogre::SubMesh* subMesh, const common::IndexWithNormalList& data, const common::IndexList& indices);

    void SetSubMeshData(Ogre::SubMesh* subMesh, const common::IndexWithTexCoordNormalList& data, const common::IndexList& indices);

    void SetSubMeshData(Ogre::SubMesh* subMesh, const std::vector<resource::data::b3d::block_data::Face35::Unknown49>& data, const common::IndexList& indices);

    void SetSubMeshData(Ogre::SubMesh* subMesh, const std::vector<resource::data::b3d::block_data::Face8::Unknown177>& data, const common::IndexList& indices);
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

    std::map<std::string, TransformList> m_transformMap;


    std::string GetB3dResourceId(const std::string& name) const;

    std::string GetB3dResourceId(const common::ResourceName& name) const;

    std::string GetMaterialName(const std::uint32_t materialIndex) const;

    std::string GetNameImpl(const std::string& blockName, const std::string& subName, bool forceUnique) const;

    void ManageSubMeshIndexBuffer(Ogre::SubMesh* subMesh, const common::IndexList& indices);

    Ogre::VertexData* GetSubMeshNonSharedVertexBuffer(Ogre::SubMesh* subMesh);

    static unsigned short ResetElementBySemantic(Ogre::VertexData* vertexData, Ogre::VertexElementSemantic sem, unsigned short index = 0);

    template <typename SubMeshDataList>
    void ResetTexCoords(Ogre::SubMesh* subMesh, const SubMeshDataList& data);

    template <typename SubMeshDataList>
    void ResetNormals(Ogre::SubMesh* subMesh, const SubMeshDataList& data);
};

} // namespace app
} // namespace d2_hack

#endif /* D2_HACK_APP_TOOL_SIMPLE_B3D_MESH_RENDERER_B3D_SCENE_BUILDER_H */
