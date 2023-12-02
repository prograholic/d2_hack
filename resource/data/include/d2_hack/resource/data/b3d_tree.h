#ifndef D2_HACK_RESOURCE_DATA_B3D_TREE_H
#define D2_HACK_RESOURCE_DATA_B3D_TREE_H

#include <d2_hack/common/platform.h>

#include <memory>
#include <vector>
#include <string>

#include <d2_hack/resource/data/b3d_visitor.h>

namespace d2_hack
{
namespace resource
{
namespace data
{
namespace b3d
{


typedef std::shared_ptr<class Node> NodePtr;
typedef std::weak_ptr<class Node> WeakNodePtr;
typedef std::vector<NodePtr> NodeList;

struct B3dTree
{
    common::Materials materials;
    NodeList rootNodes;
};


class Node
{
    Node(const Node&) = delete;
    Node& operator=(const Node&) = delete;
public:
    Node(const block_data::BlockHeader& blockHeader);

    const std::string& GetName() const;
    std::uint32_t GetType() const;

    const NodeList& GetChildNodeList() const;

    void SetChildNodes(NodeList&& childNodes);

    virtual void Visit(NodeVisitorInterface& visitor, VisitMode visitMode) = 0;

    //virtual const common::BoundingSphere& GetBoundingSphere() const = 0;

    virtual std::string GetTypeName() const = 0;

    template <typename TypedNode>
    TypedNode* NodeCast()
    {
        assert(GetType() == TypedNode::Value);

        return static_cast<TypedNode*>(this);
    }

    template <typename TypedNode>
    const TypedNode* NodeCast() const
    {
        assert(GetType() == TypedNode::Value);

        return static_cast<const TypedNode*>(this);
    }

private:
    const std::string m_name;
    const std::uint32_t m_type;

    NodeList m_childNodeList;
};


template <typename BlockType>
class NodeWithData : public Node
{
public:

    static constexpr auto Value = BlockType::Value;

    NodeWithData(const block_data::BlockHeader& blockHeader, const BlockType& block)
        : Node(blockHeader)
        , m_block(block)
    {
    }

    virtual void Visit(NodeVisitorInterface& visitor, VisitMode visitMode) override
    {
        visitor.Visit(GetName(), m_block, visitMode);
    }

    //virtual const common::BoundingSphere& GetBoundingSphere() const override
    //{
    //    return m_block.boundingSphere;
    //}

    virtual std::string GetTypeName() const override
    {
        return BlockType::Name;
    }

    const BlockType& GetBlockData() const
    {
        return m_block;
    }

    BlockType& GetBlockData()
    {
        return m_block;
    }

private:
    BlockType m_block;
};


using NodeEmpty0 = NodeWithData<block_data::Empty0>;
using NodeSimpleObjectConnector1 = NodeWithData<block_data::SimpleObjectConnector1>;
using NodeGroupUnknown2 = NodeWithData<block_data::GroupUnknown2>;
using NodeGroupRoadInfraObjects4 = NodeWithData<block_data::GroupRoadInfraObjects4>;
using NodeGroupObjects5 = NodeWithData<block_data::GroupObjects5>;
using NodeGroupVertexData7 = NodeWithData<block_data::GroupVertexData7>;
using NodeSimpleFaces8 = NodeWithData<block_data::SimpleFaces8>;
using NodeGroupTrigger9 = NodeWithData<block_data::GroupTrigger9>;
using NodeGroupLodParameters10 = NodeWithData<block_data::GroupLodParameters10>;
using NodeGroupUnknown12 = NodeWithData<block_data::GroupUnknown12>;
using NodeSimpleTrigger13 = NodeWithData<block_data::SimpleTrigger13>;
using NodeSimpleUnknown14 = NodeWithData<block_data::SimpleUnknown14>;
using NodeSimpleObjectConnector18 = NodeWithData<block_data::SimpleObjectConnector18>;
using NodeGroupObjects19 = NodeWithData<block_data::GroupObjects19>;
using NodeSimpleFlatCollision20 = NodeWithData<block_data::SimpleFlatCollision20>;
using NodeGroupObjects21 = NodeWithData<block_data::GroupObjects21>;
using NodeSimpleVolumeCollision23 = NodeWithData<block_data::SimpleVolumeCollision23>;
using NodeGroupTransformMatrix24 = NodeWithData<block_data::GroupTransformMatrix24>;
using NodeSimpleUnknown25 = NodeWithData<block_data::SimpleUnknown25>;
using NodeSimpleFaces28 = NodeWithData<block_data::SimpleFaces28>;
using NodeGroupUnknown29 = NodeWithData<block_data::GroupUnknown29>;
using NodeSimplePortal30 = NodeWithData<block_data::SimplePortal30>;
using NodeGroupLightingObjects33 = NodeWithData<block_data::GroupLightingObjects33>;
using NodeSimpleUnknown34 = NodeWithData<block_data::SimpleUnknown34>;
using NodeSimpleFaces35 = NodeWithData<block_data::SimpleFaces35>;
using NodeGroupVertexData36 = NodeWithData<block_data::GroupVertexData36>;
using NodeGroupVertexData37 = NodeWithData<block_data::GroupVertexData37>;
using NodeGroupUnknown39 = NodeWithData<block_data::GroupUnknown39>;
using NodeSimpleGeneratedObjects40 = NodeWithData<block_data::SimpleGeneratedObjects40>;

} // namespace b3d
} // namespace data
} // namespace resource
} // namespace d2_hack


#endif /* D2_HACK_RESOURCE_DATA_B3D_TREE_H */
