#ifndef D2_HACK_APP_TOOL_SIMPLE_B3D_MESH_RENDERER_B3D_TREE_VISITOR_H
#define D2_HACK_APP_TOOL_SIMPLE_B3D_MESH_RENDERER_B3D_TREE_VISITOR_H

#include <d2_hack/resource/data/b3d_tree.h>

#include "b3d_scene_builder.h"


namespace d2_hack
{
namespace app
{

class B3dTreeVisitor : public resource::data::b3d::NodeVisitorInterface, private B3dSceneBuilder
{
public:
    B3dTreeVisitor(const char* b3dId,
                   const std::string& b3dName,
                   Ogre::SceneManager* sceneManager,
                   Ogre::SceneNode* rootNode,
                   Ogre::MeshManager* meshManager,
                   const common::Materials& materials);

    virtual void Visit(const std::string& name, resource::data::b3d::block_data::Empty0& block, resource::data::b3d::VisitMode visitMode) override;

    virtual void Visit(const std::string& name, resource::data::b3d::block_data::GroupRoadInfraObjects4& block, resource::data::b3d::VisitMode visitMode) override;

    virtual void Visit(const std::string& name, resource::data::b3d::block_data::GroupObjects5& block, resource::data::b3d::VisitMode visitMode) override;

    virtual void Visit(const std::string& name, resource::data::b3d::block_data::GroupVertex7& block, resource::data::b3d::VisitMode visitMode) override;

    virtual void Visit(const std::string& name, resource::data::b3d::block_data::SimpleFaces8& block, resource::data::b3d::VisitMode visitMode) override;

    virtual void Visit(const std::string& name, resource::data::b3d::block_data::GroupTrigger9& block, resource::data::b3d::VisitMode visitMode) override;

    virtual void Visit(const std::string& name, resource::data::b3d::block_data::GroupLodParameters10& block, resource::data::b3d::VisitMode visitMode) override;

    virtual void Visit(const std::string& name, resource::data::b3d::block_data::GroupUnknown12& block, resource::data::b3d::VisitMode visitMode) override;

    virtual void Visit(const std::string& name, resource::data::b3d::block_data::SimpleTrigger13& block, resource::data::b3d::VisitMode visitMode) override;

    virtual void Visit(const std::string& name, resource::data::b3d::block_data::SimpleUnknown14& block, resource::data::b3d::VisitMode visitMode) override;

    virtual void Visit(const std::string& name, resource::data::b3d::block_data::SimpleObjectConnector18& block, resource::data::b3d::VisitMode visitMode) override;

    virtual void Visit(const std::string& name, resource::data::b3d::block_data::GroupObjects19& block, resource::data::b3d::VisitMode visitMode) override;

    virtual void Visit(const std::string& name, resource::data::b3d::block_data::SimpleFlatCollision20& block, resource::data::b3d::VisitMode visitMode) override;

    virtual void Visit(const std::string& name, resource::data::b3d::block_data::GroupObjects21& block, resource::data::b3d::VisitMode visitMode) override;

    virtual void Visit(const std::string& name, resource::data::b3d::block_data::SimpleVolumeCollision23& block, resource::data::b3d::VisitMode visitMode) override;

    virtual void Visit(const std::string& name, resource::data::b3d::block_data::GroupTransformMatrix24& block, resource::data::b3d::VisitMode visitMode) override;

    virtual void Visit(const std::string& name, resource::data::b3d::block_data::SimpleFaces28& block, resource::data::b3d::VisitMode visitMode) override;

    virtual void Visit(const std::string& name, resource::data::b3d::block_data::GroupUnknown29& block, resource::data::b3d::VisitMode visitMode) override;

    virtual void Visit(const std::string& name, resource::data::b3d::block_data::SimplePortal30& block, resource::data::b3d::VisitMode visitMode) override;

    virtual void Visit(const std::string& name, resource::data::b3d::block_data::GroupLightingObjects33& block, resource::data::b3d::VisitMode visitMode) override;

    virtual void Visit(const std::string& name, resource::data::b3d::block_data::SimpleFaces35& block, resource::data::b3d::VisitMode visitMode) override;

    virtual void Visit(const std::string& name, resource::data::b3d::block_data::GroupVertexData37& block, resource::data::b3d::VisitMode visitMode) override;

    virtual void Visit(const std::string& name, resource::data::b3d::block_data::SimpleGeneratedObjects40& block, resource::data::b3d::VisitMode visitMode) override;


private:
    template <typename Faces>
    void VisitFaces(const std::string& name, Faces& block, resource::data::b3d::VisitMode visitMode);
};


namespace optimizations
{

void MergeFaces(resource::data::b3d::B3dTree& tree);

void RemoveEmptyNodes(resource::data::b3d::B3dTree& tree);

} // namespace optimizations


} // namespace app
} // namespace d2_hack

#endif /* D2_HACK_APP_TOOL_SIMPLE_B3D_MESH_RENDERER_B3D_TREE_VISITOR_H */
