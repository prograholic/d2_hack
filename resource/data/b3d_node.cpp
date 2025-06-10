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

size_t B3dNode::m_b3dCount = 0;

B3dNode::B3dNode(const B3dTreeWeakPtr& originalRoot, const block_data::BlockHeader& blockHeader)
	: NodeBase(common::ResourceNameToString(blockHeader.name), blockHeader.type)
	, m_originalRoot(originalRoot)
{
	m_b3dCount += 1;
}

B3dNode::~B3dNode()
{
	assert(m_b3dCount > 0);
	m_b3dCount -= 1;
}

size_t B3dNode::GetB3dNodeCount()
{
	return m_b3dCount;
}

NodeCategory B3dNode::GetNodeCategory() const
{
	switch (GetType())
	{
	case block_data::GroupObjectsBlock19:
		if (GetName().starts_with(RoomNodeNamePrefix))
		{
			return NodeCategory::RoomNode;
		}
		else if (GetName().starts_with(CarNodeNamePrefix))
		{
			return NodeCategory::CarNode;
		}
		break;
		
	case block_data::GroupObjectsBlock5:
		if (GetName().starts_with(RoadNodeNamePrefix))
		{
			return NodeCategory::RoadNode;
		}
		if (GetName().starts_with(RoadHitNodeNamePrefix))
		{
			return NodeCategory::RoadHitNode;
		}
		else if (GetName().starts_with(RoadObjNodeNamePrefix))
		{
			return NodeCategory::RoadObjNode;
		}
		break;
	}

	return NodeCategory::Generic;
}

B3dTreePtr B3dNode::GetOriginalRoot() const
{
	return m_originalRoot.lock();
}

B3dNodePtr B3dNode::ExtractFirstNodeWithCategory(NodeCategory nodeCategory)
{
	auto& childNodeList = GetChildNodeList();
	auto pos = childNodeList.begin();
	while (pos != childNodeList.end())
	{
		B3dNodePtr b3dNode = std::static_pointer_cast<B3dNode>(*pos);
		if (b3dNode->GetNodeCategory() == nodeCategory)
		{
			childNodeList.erase(pos);

			return b3dNode;
		}
		++pos;
	}

	return B3dNodePtr{};
}

} // namespace b3d
} // namespace data
} // namespace resource
} // namespace d2_hack
