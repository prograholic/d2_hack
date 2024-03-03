#ifndef D2_HACK_APP_TOOL_SIMPLE_B3D_MESH_RENDERER_B3D_TREE_VISITOR_H
#define D2_HACK_APP_TOOL_SIMPLE_B3D_MESH_RENDERER_B3D_TREE_VISITOR_H

#include <d2_hack/resource/data/b3d_visitor.h>

#include "b3d_scene_builder.h"

namespace d2_hack
{
namespace app
{

class B3dTreeVisitor : public resource::data::b3d::NodeVisitorInterface, protected B3dSceneBuilder
{
public:
    B3dTreeVisitor(const std::string& b3dId,
                   Ogre::SceneManager* sceneManager,
                   Ogre::SceneNode* rootNode,
                   Ogre::MeshManager* meshManager);

    using VisitResult = resource::data::b3d::VisitResult;
    using VisitMode = resource::data::b3d::VisitMode;

    virtual VisitResult Visit(resource::data::b3d::NodeHierarchyBreaker& node, VisitMode visitMode) override;

    virtual VisitResult Visit(resource::data::b3d::NodeEventEntry& node, VisitMode visitMode) override;
    
    virtual VisitResult Visit(resource::data::b3d::NodeEmpty0& node, VisitMode visitMode) override;

    virtual VisitResult Visit(resource::data::b3d::NodeSimpleObjectConnector1& node, VisitMode visitMode) override;

    virtual VisitResult Visit(resource::data::b3d::NodeGroupUnknown2& node, VisitMode visitMode) override;

    virtual VisitResult Visit(resource::data::b3d::NodeGroupObjects5& node, VisitMode visitMode) override;

    virtual VisitResult Visit(resource::data::b3d::NodeSimpleFaces8& node, VisitMode visitMode) override;

    virtual VisitResult Visit(resource::data::b3d::NodeGroupTrigger9& node, VisitMode visitMode) override;

    virtual VisitResult Visit(resource::data::b3d::NodeGroupLodParameters10& node, VisitMode visitMode) override;

    virtual VisitResult Visit(resource::data::b3d::NodeGroupUnknown12& node, VisitMode visitMode) override;

    virtual VisitResult Visit(resource::data::b3d::NodeSimpleTrigger13& node, VisitMode visitMode) override;

    virtual VisitResult Visit(resource::data::b3d::NodeSimpleUnknown14& node, VisitMode visitMode) override;

    virtual VisitResult Visit(resource::data::b3d::NodeSimpleObjectConnector18& node, VisitMode visitMode) override;

    virtual VisitResult Visit(resource::data::b3d::NodeGroupObjects19& node, VisitMode visitMode) override;

    virtual VisitResult Visit(resource::data::b3d::NodeSimpleFlatCollision20& node, VisitMode visitMode) override;

    virtual VisitResult Visit(resource::data::b3d::NodeGroupObjects21& node, VisitMode visitMode) override;

    virtual VisitResult Visit(resource::data::b3d::NodeSimpleVolumeCollision23& node, VisitMode visitMode) override;

    virtual VisitResult Visit(resource::data::b3d::NodeGroupTransformMatrix24& node, VisitMode visitMode) override;

    virtual VisitResult Visit(resource::data::b3d::NodeSimpleUnknown25& node, VisitMode visitMode) override;

    virtual VisitResult Visit(resource::data::b3d::NodeSimpleFaces28& node, VisitMode visitMode) override;

    virtual VisitResult Visit(resource::data::b3d::NodeGroupUnknown29& node, VisitMode visitMode) override;

    virtual VisitResult Visit(resource::data::b3d::NodeSimplePortal30& node, VisitMode visitMode) override;

    virtual VisitResult Visit(resource::data::b3d::NodeGroupLightingObjects33& node, VisitMode visitMode) override;

    virtual VisitResult Visit(resource::data::b3d::NodeSimpleUnknown34& node, VisitMode visitMode) override;

    virtual VisitResult Visit(resource::data::b3d::NodeSimpleFaces35& node, VisitMode visitMode) override;

    virtual VisitResult Visit(resource::data::b3d::NodeGroupUnknown39& node, VisitMode visitMode) override;

    virtual VisitResult Visit(resource::data::b3d::NodeSimpleGeneratedObjects40& node, VisitMode visitMode) override;

private:

    template <typename FacesNode>
    void VisitFaces(FacesNode& block, VisitMode visitMode);
};


} // namespace app
} // namespace d2_hack

#endif /* D2_HACK_APP_TOOL_SIMPLE_B3D_MESH_RENDERER_B3D_TREE_VISITOR_H */
