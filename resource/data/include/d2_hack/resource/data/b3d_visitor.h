#ifndef D2_HACK_RESOURCE_DATA_INCLUDE_D2_HACK_RESOURCE_DATA_B3D_VISITOR_H
#define D2_HACK_RESOURCE_DATA_INCLUDE_D2_HACK_RESOURCE_DATA_B3D_VISITOR_H

#include <d2_hack/common/platform.h>

#include <d2_hack/resource/data/b3d_types.h>

namespace d2_hack
{
namespace resource
{
namespace data
{
namespace b3d
{

class NodeVisitorInterface
{
public:
    virtual ~NodeVisitorInterface() = default;

    virtual void Visit(const std::string& /* name */, block_data::Empty0& /* block */, VisitMode /* visitMode */) = 0;

    virtual void Visit(const std::string& /* name */, block_data::SimpleObjectConnector1& /* block */, VisitMode /* visitMode */) = 0;

    virtual void Visit(const std::string& /* name */, block_data::GroupUnknown2& /* block */, VisitMode /* visitMode */) = 0;

    virtual void Visit(const std::string& /* name */, block_data::GroupRoadInfraObjects4& /* block */, VisitMode /* visitMode */) = 0;

    virtual void Visit(const std::string& /* name */, block_data::GroupObjects5& /* block */, VisitMode /* visitMode */) = 0;

    virtual void Visit(const std::string& /* name */, block_data::GroupVertex7& /* block */, VisitMode /* visitMode */) = 0;

    virtual void Visit(const std::string& /* name */, block_data::SimpleFaces8& /* block */, VisitMode /* visitMode */) = 0;

    virtual void Visit(const std::string& /* name */, block_data::GroupTrigger9& /* block */, VisitMode /* visitMode */) = 0;

    virtual void Visit(const std::string& /* name */, block_data::GroupLodParameters10& /* block */, VisitMode /* visitMode */) = 0;

    virtual void Visit(const std::string& /* name */, block_data::GroupUnknown12& /* block */, VisitMode /* visitMode */) = 0;

    virtual void Visit(const std::string& /* name */, block_data::SimpleTrigger13& /* block */, VisitMode /* visitMode */) = 0;

    virtual void Visit(const std::string& /* name */, block_data::SimpleUnknown14& /* block */, VisitMode /* visitMode */) = 0;

    virtual void Visit(const std::string& /* name */, block_data::SimpleObjectConnector18& /* block */, VisitMode /* visitMode */) = 0;

    virtual void Visit(const std::string& /* name */, block_data::GroupObjects19& /* block */, VisitMode /* visitMode */) = 0;

    virtual void Visit(const std::string& /* name */, block_data::SimpleFlatCollision20& /* block */, VisitMode /* visitMode */) = 0;

    virtual void Visit(const std::string& /* name */, block_data::GroupObjects21& /* block */, VisitMode /* visitMode */) = 0;

    virtual void Visit(const std::string& /* name */, block_data::SimpleVolumeCollision23& /* block */, VisitMode /* visitMode */) = 0;

    virtual void Visit(const std::string& /* name */, block_data::GroupTransformMatrix24& /* block */, VisitMode /* visitMode */) = 0;

    virtual void Visit(const std::string& /* name */, block_data::SimpleUnknown25& /* block */, VisitMode /* visitMode */) = 0;

    virtual void Visit(const std::string& /* name */, block_data::SimpleFaces28& /* block */, VisitMode /* visitMode */) = 0;

    virtual void Visit(const std::string& /* name */, block_data::GroupUnknown29& /* block */, VisitMode /* visitMode */) = 0;

    virtual void Visit(const std::string& /* name */, block_data::SimplePortal30& /* block */, VisitMode /* visitMode */) = 0;

    virtual void Visit(const std::string& /* name */, block_data::GroupLightingObjects33& /* block */, VisitMode /* visitMode */) = 0;

    virtual void Visit(const std::string& /* name */, block_data::SimpleUnknown34& /* block */, VisitMode /* visitMode */) = 0;

    virtual void Visit(const std::string& /* name */, block_data::SimpleFaces35& /* block */, VisitMode /* visitMode */) = 0;

    virtual void Visit(const std::string& /* name */, block_data::GroupUnknown36& /* block */, VisitMode /* visitMode */) = 0;

    virtual void Visit(const std::string& /* name */, block_data::GroupVertexData37& /* block */, VisitMode /* visitMode */) = 0;

    virtual void Visit(const std::string& /* name */, block_data::GroupUnknown39& /* block */, VisitMode /* visitMode */) = 0;

    virtual void Visit(const std::string& /* name */, block_data::SimpleGeneratedObjects40& /* block */, VisitMode /* visitMode */) = 0;
};





class NoOpNodeVisitor: public NodeVisitorInterface
{
public:
    virtual void Visit(const std::string& /* name */, block_data::Empty0& /* block */, VisitMode /* visitMode */) override;

    virtual void Visit(const std::string& /* name */, block_data::GroupUnknown2& /* block */, VisitMode /* visitMode */) override;

    virtual void Visit(const std::string& /* name */, block_data::GroupRoadInfraObjects4& /* block */, VisitMode /* visitMode */) override;

    virtual void Visit(const std::string& /* name */, block_data::GroupObjects5& /* block */, VisitMode /* visitMode */) override;

    virtual void Visit(const std::string& /* name */, block_data::GroupVertex7& /* block */, VisitMode /* visitMode */) override;

    virtual void Visit(const std::string& /* name */, block_data::SimpleFaces8& /* block */, VisitMode /* visitMode */) override;

    virtual void Visit(const std::string& /* name */, block_data::GroupTrigger9& /* block */, VisitMode /* visitMode */) override;

    virtual void Visit(const std::string& /* name */, block_data::GroupLodParameters10& /* block */, VisitMode /* visitMode */) override;

    virtual void Visit(const std::string& /* name */, block_data::GroupUnknown12& /* block */, VisitMode /* visitMode */) override;

    virtual void Visit(const std::string& /* name */, block_data::SimpleTrigger13& /* block */, VisitMode /* visitMode */) override;

    virtual void Visit(const std::string& /* name */, block_data::SimpleUnknown14& /* block */, VisitMode /* visitMode */) override;

    virtual void Visit(const std::string& /* name */, block_data::SimpleObjectConnector18& /* block */, VisitMode /* visitMode */) override;

    virtual void Visit(const std::string& /* name */, block_data::GroupObjects19& /* block */, VisitMode /* visitMode */) override;

    virtual void Visit(const std::string& /* name */, block_data::SimpleFlatCollision20& /* block */, VisitMode /* visitMode */) override;

    virtual void Visit(const std::string& /* name */, block_data::GroupObjects21& /* block */, VisitMode /* visitMode */) override;

    virtual void Visit(const std::string& /* name */, block_data::SimpleVolumeCollision23& /* block */, VisitMode /* visitMode */) override;

    virtual void Visit(const std::string& /* name */, block_data::GroupTransformMatrix24& /* block */, VisitMode /* visitMode */) override;

    virtual void Visit(const std::string& /* name */, block_data::SimpleUnknown25& /* block */, VisitMode /* visitMode */) override;

    virtual void Visit(const std::string& /* name */, block_data::SimpleFaces28& /* block */, VisitMode /* visitMode */) override;

    virtual void Visit(const std::string& /* name */, block_data::GroupUnknown29& /* block */, VisitMode /* visitMode */) override;

    virtual void Visit(const std::string& /* name */, block_data::SimplePortal30& /* block */, VisitMode /* visitMode */) override;

    virtual void Visit(const std::string& /* name */, block_data::GroupLightingObjects33& /* block */, VisitMode /* visitMode */) override;

    virtual void Visit(const std::string& /* name */, block_data::SimpleUnknown34& /* block */, VisitMode /* visitMode */) override;

    virtual void Visit(const std::string& /* name */, block_data::SimpleFaces35& /* block */, VisitMode /* visitMode */) override;

    virtual void Visit(const std::string& /* name */, block_data::GroupVertexData37& /* block */, VisitMode /* visitMode */) override;

    virtual void Visit(const std::string& /* name */, block_data::SimpleGeneratedObjects40& /* block */, VisitMode /* visitMode */) override;
};

} // namespace b3d
} // namespace data
} // namespace resource
} // namespace d2_hack

#endif /* D2_HACK_RESOURCE_DATA_INCLUDE_D2_HACK_RESOURCE_DATA_B3D_VISITOR_H */
