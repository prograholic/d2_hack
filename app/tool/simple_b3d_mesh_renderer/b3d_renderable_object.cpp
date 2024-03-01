#include "b3d_renderable_object.h"

#include <d2_hack/resource/data/b3d_visitor.h>
#include <d2_hack/common/utils.h>
#include <d2_hack/common/log.h>

#include "b3d_scene_builder.h"

namespace d2_hack
{
namespace app
{

using namespace resource::data::b3d;

class RenderableVisitor: public RaiseExceptionVisitor, private B3dSceneBuilder
{
public:

    RenderableVisitor(const std::string& b3dId,
                      Ogre::SceneManager* sceneManager,
                      Ogre::SceneNode* rootNode,
                      Ogre::MeshManager* meshManager,
                      Ogre::SceneNode*& selfSceneNode)
        : B3dSceneBuilder(b3dId, sceneManager, rootNode, meshManager)
        , m_selfSceneNode(selfSceneNode)
    {
    }

    virtual VisitResult Visit(NodeGroupObjects5& /* node */, VisitMode /* visitMode */) override
    {
        return VisitResult::Continue;
    }

    virtual VisitResult Visit(NodeGroupUnknown2& /* node */, VisitMode /* visitMode */) override
    {
        return VisitResult::Continue;
    }

    virtual VisitResult Visit(NodeGroupVertexData7& node, VisitMode visitMode) override
    {
        auto sceneNode = ProcessSceneNode(node.GetName(), visitMode);
        if (visitMode == VisitMode::PostOrder)
        {
            m_selfSceneNode = sceneNode;
        }

        return VisitResult::Continue;
    }

    virtual VisitResult Visit(NodeSimpleFaces8& node, VisitMode visitMode) override
    {
        VisitFaces(node, visitMode);

        return VisitResult::Continue;
    }

    virtual VisitResult Visit(NodeGroupTrigger9& /* node */, VisitMode /* visitMode */) override
    {
        return VisitResult::Continue;
    }

    virtual VisitResult Visit(NodeGroupLodParameters10& /* node */, VisitMode /* visitMode */) override
    {
        return VisitResult::Continue;
    }

    virtual VisitResult Visit(NodeSimpleFlatCollision20& /* node */, VisitMode /* visitMode */) override
    {
        return VisitResult::Continue;
    }

    virtual VisitResult Visit(NodeGroupObjects21& /* node */, VisitMode /* visitMode */) override
    {
        return VisitResult::Continue;
    }

    virtual VisitResult Visit(NodeSimpleFaces28& node, VisitMode visitMode) override
    {
        VisitFaces(node, visitMode);

        return VisitResult::Continue;
    }

    VisitResult Visit(NodeSimpleFaces35& node, VisitMode visitMode) override
    {
        VisitFaces(node, visitMode);

        return VisitResult::Continue;
    }

    virtual VisitResult Visit(NodeGroupVertexData37& node, VisitMode visitMode) override
    {
        auto sceneNode = ProcessSceneNode(node.GetName(), visitMode);
        if (visitMode == VisitMode::PostOrder)
        {
            m_selfSceneNode = sceneNode;
        }

        return VisitResult::Continue;
    }

    virtual VisitResult Visit(NodeSimpleGeneratedObjects40& node, VisitMode visitMode)
    {
        if (visitMode == VisitMode::PreOrder)
        {
            auto generatorName = common::ResourceNameToString(node.GetBlockData().name);
            if (generatorName == "$$GeneratorOfTerrain")
            {
                OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS, "Terrain generator should NOT be in renderable object, node: " + node.GetName());
            }
            else
            {
                D2_HACK_LOG(Visit) << "unsupported generator `" << generatorName << "`";
            }
        }

        return VisitResult::Continue;
    }

private:
    Ogre::SceneNode*& m_selfSceneNode;;

    template <typename FacesNode>
    void VisitFaces(FacesNode& node, VisitMode visitMode)
    {
        if (visitMode == VisitMode::PreOrder)
        {
            const auto& block = node.GetBlockData();
            for (const auto& face : block.faces)
            {
                CreateMesh(node.GetName(), face.meshInfo, node.GetOriginalRoot()->GetMaterialNameByIndex(face.materialIndex));
            }
        }
    }
};


B3dRenderableObject::B3dRenderableObject(const std::string& b3dId,
                                         const resource::data::b3d::NodePtr& b3dNode,
                                         Ogre::SceneManager* sceneManager,
                                         Ogre::MeshManager* meshManager,
                                         Ogre::SceneNode* rootSceneNode)
    : m_b3dNode(b3dNode)
    , m_sceneNode(nullptr)
{
    RenderableVisitor visitor{b3dId, sceneManager, rootSceneNode, meshManager, m_sceneNode};

    auto visitResult = VisitNode(m_b3dNode, visitor);
    (void)visitResult;
}


} // namespace app
} // namespace d2_hack
