#include <d2_hack/resource/data/b3d_node.h>

#include <d2_hack/common/utils.h>

namespace d2_hack
{
namespace resource
{
namespace data
{
namespace b3d
{

const char* RoomNodeNamePrefix = "room_";
const char* RoadNodeNamePrefix = "road_";
const char* RoadHitNodeNamePrefix = "hit_road_";
const char* RoadObjNodeNamePrefix = "obj_";
const char* CarNodeNamePrefix = "car_";

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

NodeCategory Node::GetNodeCategory() const
{
	switch (m_type)
	{
	case block_data::GroupObjectsBlock19:
		if (m_name.starts_with(RoomNodeNamePrefix))
		{
			return NodeCategory::RoomNode;
		}
		else if (m_name.starts_with(CarNodeNamePrefix))
		{
			return NodeCategory::CarNode;
		}
		break;
		
	case block_data::GroupObjectsBlock5:
		if (m_name.starts_with(RoadNodeNamePrefix))
		{
			return NodeCategory::RoadNode;
		}
		if (m_name.starts_with(RoadHitNodeNamePrefix))
		{
			return NodeCategory::RoadHitNode;
		}
		else if (m_name.starts_with(RoadObjNodeNamePrefix))
		{
			return NodeCategory::RoadObjNode;
		}
		break;
	}

	return NodeCategory::Generic;
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

NodePtr Node::ExtractFirstNodeWithCategory(NodeCategory nodeCategory)
{
	auto pos = m_childNodeList.begin();
	while (pos != m_childNodeList.end())
	{
		if ((*pos)->GetNodeCategory() == nodeCategory)
		{
			NodePtr res = *pos;
			m_childNodeList.erase(pos);

			return res;
		}
		++pos;
	}

	return NodePtr{};
}

} // namespace b3d
} // namespace data
} // namespace resource
} // namespace d2_hack
