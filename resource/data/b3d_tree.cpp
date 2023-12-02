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
	{
		{"COMMON", "common"},
		{"ENV", "aa"},
		{"ENV", "ab"},
		{"ENV", "ac"},
		{"ENV", "ad"},
		{"ENV", "ae"},
		{"ENV", "af"},
		{"ENV", "ag"},
		{"ENV", "ah"},
		{"ENV", "aj"},
		{"ENV", "ak"},
		{"ENV", "al"},
		{"ENV", "am"},
		{"ENV", "ap"},
		{"ENV", "aq"},
		{"ENV", "ar"},
		{"ENV", "as"},
		{"ENV", "at"},
		{"ENV", "au"},
		{"ENV", "av"},
		{"ENV", "aw"},
		{"ENV", "ax"},

		{"ENV", "ba"},
		{"ENV", "bb"},
		{"ENV", "bc"},
		{"ENV", "bd"},
		{"ENV", "be"},
		{"ENV", "bf"},
		{"ENV", "bg"},

		{"ENV", "ca"},
		{"ENV", "cb"},
		{"ENV", "cc"},
		{"ENV", "ce"},
		{"ENV", "cf"},
		{"ENV", "ch"},

		{"ENV", "da"},
		{"ENV", "db"},
		{"ENV", "dc"},
		{"ENV", "dq"},
		{"ENV", "dr"}
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
