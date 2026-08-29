#ifndef D2_HACK_APP_ROOM_VISITOR_H
#define D2_HACK_APP_ROOM_VISITOR_H

#include "base_game_object_visitor.h"

namespace d2_hack
{
namespace app
{

struct TreeData
{
    scene_node::SceneNodeBaseList rootNodes;
    Ogre::MeshPtr mesh;
    std::string name;
    std::string materialName;
    Ogre::Vector3 location;
};

class RoomVisitor : public GameObjectVisitorBase
{
public:
    using VisitResult = resource::data::b3d::VisitResult;
    using VisitMode = resource::data::b3d::VisitMode;

    RoomVisitor(std::string_view b3dId,
                std::string_view blockName,
                Ogre::MeshManager* meshManager,
                Ogre::SceneManager* sceneManager,
                resource::archive::res::OgreMaterialProvider* ogreMaterialProvider);

    virtual VisitResult Visit(const std::shared_ptr<resource::data::b3d::NodeGroupRoadInfraObjects4>& node, VisitMode visitMode) override;

    virtual VisitResult Visit(const std::shared_ptr<resource::data::b3d::NodeGroupTrigger9>& node, VisitMode visitMode) override;

    virtual VisitResult Visit(const std::shared_ptr<resource::data::b3d::NodeGroupUnknown12>& node, VisitMode visitMode) override;

    virtual VisitResult Visit(const std::shared_ptr<resource::data::b3d::NodeSimpleTrigger13>& node, VisitMode visitMode) override;
    
    virtual VisitResult Visit(const std::shared_ptr<resource::data::b3d::NodeGroupObjects19>& node, VisitMode visitMode) override;

    virtual VisitResult Visit(const std::shared_ptr<resource::data::b3d::NodeSimpleFlatCollision20>& node, VisitMode visitMode) override;

    virtual VisitResult Visit(const std::shared_ptr<resource::data::b3d::NodeGroupUnknown29>& node, VisitMode visitMode) override;

    virtual VisitResult Visit(const std::shared_ptr<resource::data::b3d::NodeSimplePortal30>& node, VisitMode visitMode) override;

    virtual VisitResult Visit(const std::shared_ptr<resource::data::b3d::NodeSimpleUnknown34>& node, VisitMode visitMode) override;

    virtual VisitResult Visit(const std::shared_ptr<resource::data::b3d::NodeSimpleGeneratedObjects40>& node, VisitMode visitMode) override;

    const std::vector<TreeData>& GetTreeData();

private:
    Ogre::SceneManager* m_sceneManager;

    std::vector<TreeData> m_trees;

    void FillTreeDataMeshAndMaterial(std::string_view b3dId, const resource::data::b3d::block_data::SimpleGeneratedObjects40& data, TreeData& treeData);
};


} // namespace app
} // namespace d2_hack

#endif /* D2_HACK_APP_B3D_TREE_VISITOR_H */
