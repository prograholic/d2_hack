#ifndef D2_HACK_APP_TOOL_SIMPLE_B3D_MESH_RENDERER_B3D_SCENE_BUILDER_H
#define D2_HACK_APP_TOOL_SIMPLE_B3D_MESH_RENDERER_B3D_SCENE_BUILDER_H

#include <d2_hack/common/platform.h>

#include <stack>
#include <list>

D2_HACK_DISABLE_WARNING_BEGIN(4251)
#include <OgreRenderOperation.h>
#include <OgreSceneNode.h>
#include <OgreMatrix3.h>
D2_HACK_DISABLE_WARNING_END() // 4251

#include <d2_hack/common/types.h>
#include <d2_hack/resource/data/b3d_node.h>

namespace d2_hack
{
namespace app
{

class B3dSceneNodeBase : public common::NodeBase
{
public:
    B3dSceneNodeBase(const std::string& name, std::uint32_t type);

    virtual void SetVisible(bool visible) = 0;
};

typedef std::shared_ptr<B3dSceneNodeBase> B3dSceneNodeBasePtr;
typedef std::list<B3dSceneNodeBasePtr> B3dSceneNodeBaseList;


template <std::uint32_t NodeTypeId, typename SceneNodeBaseType>
class B3dSceneNode : public SceneNodeBaseType
{
public:
    static constexpr std::uint32_t Value = NodeTypeId;

    template<typename... Args>
    B3dSceneNode(const std::string& name, Args&&... args)
        : SceneNodeBaseType(name, Value, std::forward<Args&&>(args)...)
    {
    }
};



template <std::uint32_t NodeTypeId, typename SceneNodeBaseType = B3dSceneNodeBase>
class B3dOgreSceneNode : public B3dSceneNode<NodeTypeId, SceneNodeBaseType>
{
public:

    template<typename... Args>
    B3dOgreSceneNode(const std::string& name, Ogre::SceneNode* sceneNode, Args&&... args)
        : B3dSceneNode<NodeTypeId, SceneNodeBaseType>(name, std::forward<Args&&>(args)...)
        , m_sceneNode(sceneNode)
    {
    }

    virtual void SetVisible(bool visible) override
    {
        m_sceneNode->setVisible(visible);
    }

private:
    Ogre::SceneNode* m_sceneNode;
};

template <typename Node, typename... Args>
static B3dSceneNodeBasePtr CreateB3dSceneNode(const B3dSceneNodeBasePtr& parent, Args&&... args)
{
    auto res = std::make_shared<Node>(std::forward<Args&&>(args)...);
    if (parent)
    {
        parent->AddChildNode(res);
    }

    return res;
}

class B3dSceneBuilder
{
public:

    using VisitResult = resource::data::b3d::VisitResult;
    using VisitMode = resource::data::b3d::VisitMode;

    B3dSceneBuilder(const std::string& b3dId,
                    Ogre::SceneManager* sceneManager,
                    Ogre::SceneNode* rootNode,
                    Ogre::MeshManager* meshManager,
                    B3dSceneNodeBaseList& rootB3dSceneNodes);

    ~B3dSceneBuilder();

    std::string GetB3dId() const;
    
    Ogre::SceneManager* GetSceneManager() const;
    
    Ogre::MeshManager* GetMeshManager() const;
    
    Ogre::SceneNode* GetCurrentSceneNode() const;

    void ProcessLight(const resource::data::b3d::NodeGroupLightingObjects33& node, VisitMode visitMode);

    void ProcessObjectConnector(const resource::data::b3d::NodeSimpleObjectConnector18& node);

    void ProcessObjectConnector(const resource::data::b3d::NodeSimpleObjectConnector1& node, VisitMode visitMode);

    Ogre::SceneNode* ProcessSceneNode(const std::string& name, resource::data::b3d::VisitMode visitMode);

    void CreateMesh(const std::string& blockName, const common::SimpleMeshInfo& meshInfo, const std::string& materialName);

    B3dSceneNodeBasePtr GetParentB3dSceneNode();

    void PushToSceneNodeStack(const B3dSceneNodeBasePtr& node);

    void PopFromSceneNodeStack();

private:
    const std::string m_b3dId;
    Ogre::SceneManager* m_sceneManager;
    Ogre::SceneNode* m_rootNode;
    Ogre::MeshManager* m_meshManager;
    B3dSceneNodeBaseList& m_rootB3dSceneNodes;
    std::stack<Ogre::SceneNode*> m_sceneNodes;
    std::stack<B3dSceneNodeBasePtr> m_b3dSceneNodesStack;

    std::string GetB3dResourceId(const std::string& name) const;

    std::string GetB3dResourceId(const common::ResourceName& name) const;

    std::string GetNameImpl(const std::string& blockName, const std::string& subName, bool forceUnique) const;

    Ogre::SubMesh* CreateSubMesh(const Ogre::MeshPtr& mesh, const std::string& materialName);

    void SetMeshInfo(const Ogre::MeshPtr& mesh, const common::SimpleMeshInfo& meshInfo, const std::string& materialName);

    void ManagePositions(Ogre::VertexData* vertexData, const common::PositionList& positions, unsigned short bufferIndex);

    void ManageTexCoords(Ogre::VertexData* vertexData, const common::TexCoordList& texCoords, unsigned short bufferIndex);

    void ManageNormals(Ogre::VertexData* vertexData, const common::NormalList& normals, unsigned short bufferIndex);
};

} // namespace app
} // namespace d2_hack

#endif /* D2_HACK_APP_TOOL_SIMPLE_B3D_MESH_RENDERER_B3D_SCENE_BUILDER_H */
