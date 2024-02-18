#ifndef D2_HACK_RESOURCE_DATA_B3D_READER_VISITORS_H
#define D2_HACK_RESOURCE_DATA_B3D_READER_VISITORS_H

#include <d2_hack/resource/data/b3d_visitor.h>

namespace d2_hack
{
namespace resource
{
namespace data
{
namespace b3d
{

class TransformVisitor : public NoOpNodeVisitor
{
public:

    explicit TransformVisitor(TransformationMap& transformations);

    ~TransformVisitor() noexcept;

    virtual void Visit(NodeGroupTransformMatrix24& node, VisitMode visitMode) override;

private:
    std::list<block_data::GroupTransformMatrix24> m_transformQueue;

    TransformationMap& m_transformMap;

};


} // namespace b3d
} // namespace data
} // namespace resource
} // namespace d2_hack



#endif /* D2_HACK_RESOURCE_DATA_B3D_READER_VISITORS_H */
