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
    B3dSceneBuilder(const std::string& b3dId,
                    const std::string& b3dName,
                    Ogre::SceneManager* sceneManager,
                    Ogre::SceneNode* rootNode,
                    Ogre::MeshManager* meshManager,
                    const common::Materials& materials);

    ~B3dSceneBuilder();

    void ProcessTransformQueue(const std::string& name, const resource::data::b3d::block_data::GroupTransformMatrix24& block, resource::data::b3d::VisitMode visitMode);

    void ProcessLight(const std::string& name, const resource::data::b3d::block_data::GroupLightingObjects33& block, resource::data::b3d::VisitMode visitMode);

    void ProcessObjectConnector(const resource::data::b3d::block_data::SimpleObjectConnector18& block, resource::data::b3d::VisitMode visitMode);

    void ProcessObjectConnector(const resource::data::b3d::block_data::SimpleObjectConnector1& block, resource::data::b3d::VisitMode visitMode);

    Ogre::SceneNode* ProcessSceneNode(const std::string& name, resource::data::b3d::VisitMode visitMode);

    void CreateMesh(const std::string& blockName, const common::SimpleMeshInfo& meshInfo, std::uint32_t materialIndex);

private:
    std::string m_b3dId;
    std::string m_b3dName;
    Ogre::SceneManager* m_sceneManager;
    Ogre::SceneNode* m_rootNode;
    Ogre::MeshManager* m_meshManager;
    const common::Materials m_materials;

    std::stack<Ogre::SceneNode*> m_sceneNodes;
    std::list<resource::data::b3d::block_data::GroupTransformMatrix24> m_transformQueue;

    std::map<std::string, TransformList> m_transformMap;


    std::string GetB3dResourceId(const std::string& name) const;

    std::string GetB3dResourceId(const common::ResourceName& name) const;

    std::string GetMaterialName(const std::uint32_t materialIndex) const;

    std::string GetNameImpl(const std::string& blockName, const std::string& subName, bool forceUnique) const;

    Ogre::SubMesh* CreateSubMesh(const Ogre::MeshPtr& mesh, std::uint32_t materialIndex);

    void SetMeshInfo(const Ogre::MeshPtr& mesh, const common::SimpleMeshInfo& meshInfo, std::uint32_t materialIndex);

    void ManagePositions(Ogre::VertexData* vertexData, const common::PositionList& positions, unsigned short bufferIndex);

    void ManageTexCoords(Ogre::VertexData* vertexData, const common::TexCoordList& texCoords, unsigned short bufferIndex);

    void ManageNormals(Ogre::VertexData* vertexData, const common::NormalList& normals, unsigned short bufferIndex);
};

} // namespace app
} // namespace d2_hack

#endif /* D2_HACK_APP_TOOL_SIMPLE_B3D_MESH_RENDERER_B3D_SCENE_BUILDER_H */
