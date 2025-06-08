#ifndef D2_HACK_APP_TOOL_SIMPLE_B3D_MESH_RENDERER_B3D_SCENE_BUILDER_H
#define D2_HACK_APP_TOOL_SIMPLE_B3D_MESH_RENDERER_B3D_SCENE_BUILDER_H

#include <d2_hack/common/platform.h>

#include <stack>

#include <d2_hack/scene_node/scene_node_base.h>

#include <d2_hack/resource/data/b3d_node.h>
#include <d2_hack/resource/archive/ogre_material_provider.h>

namespace d2_hack
{
namespace app
{


class B3dSceneBuilder
{
public:

    using VisitResult = resource::data::b3d::VisitResult;
    using VisitMode = resource::data::b3d::VisitMode;

    B3dSceneBuilder(const std::string& b3dId,
                    Ogre::SceneManager* sceneManager,
                    Ogre::SceneNode* ogreRootNode,
                    Ogre::MeshManager* meshManager,
                    resource::archive::res::OgreMaterialProvider* ogreMaterialProvider,
                    scene_node::SceneNodeBaseList& rootSceneNodes);

    ~B3dSceneBuilder();

    std::string GetB3dId() const;
    
    Ogre::SceneManager* GetSceneManager() const;
    
    Ogre::MeshManager* GetMeshManager() const;
    
    Ogre::SceneNode* GetCurrentOgreSceneNode() const;

    resource::archive::res::OgreMaterialProvider* GetOgreMaterialProvider() const;

    void ProcessLight(const resource::data::b3d::NodeGroupLightingObjects33& node, VisitMode visitMode);

    void ProcessObjectConnector(const resource::data::b3d::NodeSimpleObjectConnector18& node);

    void ProcessObjectConnector(const resource::data::b3d::NodeSimpleObjectConnector1& node, VisitMode visitMode);

    Ogre::SceneNode* ProcessOgreSceneNode(const std::string& name, resource::data::b3d::VisitMode visitMode);

    void CreateMesh(const std::string& blockName, const common::SimpleMeshInfo& meshInfo, const std::string& materialName);

    scene_node::SceneNodeBasePtr GetParentSceneNode();

    void PushToSceneNodeStack(const scene_node::SceneNodeBasePtr& node);

    void PopFromSceneNodeStack();

private:
    const std::string m_b3dId;
    Ogre::SceneManager* m_sceneManager;
    Ogre::SceneNode* m_ogreRootNode;
    Ogre::MeshManager* m_meshManager;
    resource::archive::res::OgreMaterialProvider* m_ogreMaterialProvider;
    scene_node::SceneNodeBaseList& m_rootSceneNodes;
    std::stack<Ogre::SceneNode*> m_ogreSceneNodes;
    std::stack<scene_node::SceneNodeBasePtr> m_sceneNodesStack;

    std::string GetB3dResourceId(const std::string& name) const;

    std::string GetB3dResourceId(const common::ResourceName& name) const;

    std::string GetNameImpl(const std::string& blockName, const std::string& subName, bool forceUnique) const;

    Ogre::SubMesh* CreateSubMesh(const Ogre::MeshPtr& mesh, const Ogre::MaterialPtr& material);

    void SetMeshInfo(const Ogre::MeshPtr& mesh, const common::SimpleMeshInfo& meshInfo, const Ogre::MaterialPtr& material);

    void ManagePositions(Ogre::VertexData* vertexData, const common::PositionList& positions, unsigned short bufferIndex);

    void ManageTexCoords(Ogre::VertexData* vertexData, const common::TexCoordList& texCoords, unsigned short bufferIndex);

    void ManageNormals(Ogre::VertexData* vertexData, const common::NormalList& normals, unsigned short bufferIndex);
};

} // namespace app
} // namespace d2_hack

#endif /* D2_HACK_APP_TOOL_SIMPLE_B3D_MESH_RENDERER_B3D_SCENE_BUILDER_H */
