#include "car_visitor.h"


namespace d2_hack
{
namespace app
{

using namespace resource::data::b3d;

WheelVisitor::WheelVisitor(std::string_view b3dId,
                           std::string_view blockName,
                           Ogre::MeshManager* meshManager,
                           resource::archive::res::OgreMaterialProvider* ogreMaterialProvider)
    : GameObjectVisitorBase(b3dId, blockName, Ogre::Vector3::ZERO, meshManager, ogreMaterialProvider)
    , m_wheelData()
    , m_topLevelBlockConnectorPreVisited(false)
    , m_topLevelBlockConnectorPostVisited(false)
{
    m_wheelData.id = MapObjectIdToWheelId(blockName);
    m_wheelData.mesh = GetMesh();
    m_wheelData.name = blockName;
}

VisitResult WheelVisitor::Visit(const std::shared_ptr<NodeGroupObjects5>& /* node */, VisitMode /* visitMode */)
{
    return VisitResult::Continue;
}

VisitResult WheelVisitor::Visit(const std::shared_ptr<NodeSimpleObjectConnector18>& node, VisitMode visitMode)
{
    // Забираем первый узел для трансформаций (он будет применен к Ogre::SceneNode позже)
    if (visitMode == VisitMode::PreOrder)
    {
        if (!m_topLevelBlockConnectorPreVisited)
        {
            m_wheelData.transformations = node->GetBlockData().transformation;
            m_topLevelBlockConnectorPreVisited = true;
        }
        else
        {
            return GameObjectVisitorBase::Visit(node, visitMode);
        }
    }
    else
    {
        if (!m_topLevelBlockConnectorPostVisited)
        {
            m_topLevelBlockConnectorPreVisited = true;
        }
        else
        {
            return GameObjectVisitorBase::Visit(node, visitMode);
        }
    }

    return VisitResult::Continue;
}

const WheelData& WheelVisitor::GetWheelData()
{
    m_wheelData.rootNodes = GetRootSceneNodes();

    return m_wheelData;
}

/////////////////////////////////////////////////////////////////////////////////////


class HitBoxVisitor : public RaiseExceptionVisitor
{
public:

    virtual VisitResult Visit(const std::shared_ptr<NodeGroupObjects5>& /* node */, VisitMode /* visitMode */) override
    {
        return VisitResult::Continue;
    }

    virtual VisitResult Visit(const std::shared_ptr<NodeSimpleUnknown14>& /* node */, VisitMode /* visitMode */) override
    {
        return VisitResult::Continue;
    }

    virtual VisitResult Visit(const std::shared_ptr<NodeGroupObjects21>& /* node */, VisitMode /* visitMode */) override
    {
        return VisitResult::Continue;
    }

    virtual VisitResult Visit(const std::shared_ptr<NodeSimpleVolumeCollision23>& node, VisitMode visitMode) override
    {
        if (visitMode == VisitMode::PreOrder)
        {
            for (const auto& polygon : node->GetBlockData().polygons)
            {
                for (const auto& vertex : polygon)
                {
                    m_hitBox.boundingBox.merge(vertex);
                }
            }
        }

        return VisitResult::Continue;
    }

    virtual VisitResult Visit(const std::shared_ptr<NodeEventEntry>& /* node */, VisitMode /* visitMode */) override
    {
        return VisitResult::Continue;
    }

    const HitBox& GetHitBox() const
    {
        return m_hitBox;
    }
private:
    HitBox m_hitBox;
};

MoveableObjectVisitor::MoveableObjectVisitor(std::string_view b3dId,
                                             std::string_view blockName,
                                             const Ogre::Vector3& centerOffset,
                                             Ogre::MeshManager* meshManager,
                                             resource::archive::res::OgreMaterialProvider* ogreMaterialProvider)
    : GameObjectVisitorBase(b3dId, blockName, centerOffset, meshManager, ogreMaterialProvider)
    , m_hitBox()
{
}

VisitResult MoveableObjectVisitor::Visit(const std::shared_ptr<NodeGroupObjects5>& node, VisitMode visitMode)
{
    if (visitMode == VisitMode::PreOrder)
    {
        if (node->GetName() == std::format("hit_{}", GetBlockName()))
        {
            HitBoxVisitor hitboxVisitor{};

            auto visitResult = VisitNode(node, hitboxVisitor);
            (void)visitResult;

            m_hitBox = hitboxVisitor.GetHitBox();

            return VisitResult::SkipChildren;
        }
    }

    return GameObjectVisitorBase::Visit(node, visitMode);
}

const HitBox& MoveableObjectVisitor::GetHitBox() const
{
    return m_hitBox;
}


WheelBasedMoveableObjectVisitor::WheelBasedMoveableObjectVisitor(std::string_view b3dId,
                                                                 std::string_view blockName,
                                                                 const Ogre::Vector3& centerOffset,
                                                                 Ogre::MeshManager* meshManager,
                                                                 resource::archive::res::OgreMaterialProvider* ogreMaterialProvider)
    : MoveableObjectVisitor(b3dId, blockName, centerOffset, meshManager, ogreMaterialProvider)
    , m_wheelRootSceneNodes()
{
}

VisitResult WheelBasedMoveableObjectVisitor::Visit(const std::shared_ptr<NodeGroupObjects5>& node, VisitMode visitMode)
{
    if (visitMode == VisitMode::PreOrder)
    {
        std::string wheelName = std::string{ GetBlockName() } + "wheel";
        if (node->GetName().starts_with(wheelName))
        {
            WheelVisitor wheelVisitor{ GetB3dId(), node->GetName(), GetMeshManager(), GetMaterialProvider() };

            auto visitResult = VisitNode(node, wheelVisitor);
            (void)visitResult;

            m_wheelRootSceneNodes.push_back(wheelVisitor.GetWheelData());

            return VisitResult::SkipChildren;
        }
    }

    return MoveableObjectVisitor::Visit(node, visitMode);
}

const std::vector<WheelData>& WheelBasedMoveableObjectVisitor::GetWheelData() const
{
    return m_wheelRootSceneNodes;
}


} // namespace app
} // namespace d2_hack
