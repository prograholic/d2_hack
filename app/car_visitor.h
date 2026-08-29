#ifndef D2_HACK_APP_CAR_VISITOR_H
#define D2_HACK_APP_CAR_VISITOR_H

#include "base_game_object_visitor.h"

#include <d2_hack/app/b3d_car.h>

namespace d2_hack
{
namespace app
{


struct WheelData
{
    scene_node::SceneNodeBaseList rootNodes;
    Ogre::MeshPtr mesh;
    std::string name;
    WheelId id;
    resource::data::b3d::TransformList transformations;
};


class WheelVisitor : public GameObjectVisitorBase
{
public:

    using VisitResult = resource::data::b3d::VisitResult;
    using VisitMode = resource::data::b3d::VisitMode;

    WheelVisitor(std::string_view b3dId,
                 std::string_view blockName,
                 Ogre::MeshManager* meshManager,
                 resource::archive::res::OgreMaterialProvider* ogreMaterialProvider);

    virtual VisitResult Visit(const std::shared_ptr<resource::data::b3d::NodeGroupObjects5>& node, VisitMode visitMode) override;

    virtual VisitResult Visit(const std::shared_ptr<resource::data::b3d::NodeSimpleObjectConnector18>& node, VisitMode visitMode) override;

    const WheelData& GetWheelData();

private:
    WheelData m_wheelData;
    bool m_topLevelBlockConnectorPreVisited;
    bool m_topLevelBlockConnectorPostVisited;
};


class WheelBasedMoveableObjectVisitor : public GameObjectVisitorBase
{
public:

    using VisitResult = resource::data::b3d::VisitResult;
    using VisitMode = resource::data::b3d::VisitMode;

    WheelBasedMoveableObjectVisitor(std::string_view b3dId,
                                    std::string_view blockName,
                                    Ogre::MeshManager* meshManager,
                                    resource::archive::res::OgreMaterialProvider* ogreMaterialProvider);

    virtual VisitResult Visit(const std::shared_ptr<resource::data::b3d::NodeGroupObjects5>& node, VisitMode visitMode) override;

    const std::vector<WheelData>& GetWheelData() const;

private:
    std::vector<WheelData> m_wheelRootSceneNodes;
};


} // namespace app
} // namespace d2_hack

#endif /* D2_HACK_APP_CAR_VISITOR_H */
