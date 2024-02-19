#include <d2_hack/resource/data/b3d_tree.h>

#include <d2_hack/common/utils.h>
#include <d2_hack/common/resource_mgmt.h>

#include "b3d_reader_visitors.h"

namespace d2_hack
{
namespace resource
{
namespace data
{
namespace b3d
{
//#define USE_ALL

const B3dRegistry SinglePlayerRegistry
{
	D2_ROOT_DIR,
	"ENV",
	{
#ifdef USE_ALL
		"aa",
		"ab",
		"ac",
		"ad",
		"ae",
		"af",
		"ag",
#endif // USE_ALL
		"ah",
#ifdef USE_ALL
		"aj",
		"ak",
		"al",
		"am",
		"ap",
		"aq",
		"ar",
		"as",
		"at",
		"au",
		"av",
		"aw",
		"ax",
		"ba",
		"bb",
		"bc",
		"bd",
		"be",
		"bf",
		"bg",
		"ca",
		"cb",
		"cc",
		"ce",
		"cf",
		"ch",
		"da",
		"db",
		"dc",
		"dq",
		"dr"
#endif //USE_ALL
	}
};

std::string B3dTree::GetMaterialNameByIndex(std::uint32_t materialIndex) const
{
	return common::GetResourceName(id, common::ResourceNameToString(materials[materialIndex]));
}

void AddTransformMatrix(const NodePtr& root, TransformationMap& transformations)
{
	TransformVisitor visitor{transformations};
	VisitNode(root, visitor);
}

void B3dTree::AddRootNode(const NodePtr& root)
{
	switch (root->GetType())
	{
	case block_data::EmptyBlock0:
		// skip empty block
		break;

	case block_data::SimpleObjectConnectorBlock1:
	case block_data::GroupRoadInfraObjectsBlock4:
	case block_data::GroupObjectsBlock5:
	case block_data::GroupVertexDataBlock7:
	case block_data::GroupObjectsBlock19:
	case block_data::GroupObjectsBlock21:
	case block_data::GroupLightingObjectBlock33:
	case block_data::GroupVertexDataBlock37:
		rootNodes.push_back(root);
		break;

	case block_data::SimpleVolumeCollisionBlock23:
		volumeCollisions.push_back(root->NodeCast<NodeSimpleVolumeCollision23>()->GetBlockData());
		break;

	case block_data::HierarchyBreakerBlockXxx:
		// skip hierarchy breaker for root nodes
		break;

	case block_data::GroupTransformMatrixBlock24:
		AddTransformMatrix(root, transformations);
		break;

	default:
		OGRE_EXCEPT(Ogre::Exception::ERR_INVALID_STATE, "unsupported root node type: " + std::to_string(root->GetType()), "B3dTree::AddRootNode");
	}
}



Node::Node(const B3dTreeWeakPtr& originalRoot, const block_data::BlockHeader& blockHeader)
	: m_name(common::ResourceNameToString(blockHeader.name))
	, m_type(blockHeader.type)
	, m_originalRoot(originalRoot)
	, m_parent()
{
}

const std::string& Node::GetName() const
{
	return m_name;
}

std::uint32_t Node::GetType() const
{
	return m_type;
}

NodeList& Node::GetChildNodeList()
{
	return m_childNodeList;
}

void Node::SetChildNodes(NodeList&& childNodes)
{
	m_childNodeList = std::move(childNodes);
	for (auto child : m_childNodeList)
	{
		child->SetParent(weak_from_this());
	}
}

void Node::AddChildNode(const NodePtr& node)
{
	m_childNodeList.push_back(node);
	node->SetParent(weak_from_this());
}

bool Node::HasParent() const
{
	return !m_parent.expired();
}

NodePtr Node::GetParent()
{
	return m_parent.lock();
}

void Node::SetParent(const WeakNodePtr& parent)
{
	m_parent = parent;
}

B3dTreePtr Node::GetOriginalRoot() const
{
	return m_originalRoot.lock();
}

} // namespace b3d
} // namespace data
} // namespace resource
} // namespace d2_hack
