#include <d2_hack/resource/data/b3d_tree.h>

#include <d2_hack/common/utils.h>

namespace d2_hack
{
namespace resource
{
namespace data
{
namespace b3d
{

const B3dRegistry SinglePlayerRegistry
{
	D2_ROOT_DIR,
	"ENV",
	{
		"aa",
		"ab",
		"ac",
		"ad",
		"ae",
		"af",
		"ag",
		"ah",
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
	}
};

Node::Node(const block_data::BlockHeader& blockHeader)
	: m_name(common::ResourceNameToString(blockHeader.name))
	, m_type(blockHeader.type)
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

const NodeList& Node::GetChildNodeList() const
{
	return m_childNodeList;
}

void Node::SetChildNodes(NodeList&& childNodes)
{
	m_childNodeList = std::move(childNodes);
}


} // namespace b3d
} // namespace data
} // namespace resource
} // namespace d2_hack
