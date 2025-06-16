#include <d2_hack/common/node_base.h>


namespace d2_hack
{
namespace common
{

size_t NodeBase::m_count = 0;

NodeBase::NodeBase(const std::string_view& name, std::uint32_t type)
	: m_childNodeList()
	, m_parent()
	, m_name(name)
	, m_type(type)
{
	m_count += 1;
}

NodeBase::~NodeBase()
{
	assert(m_count > 0);
	m_count -= 1;
}

size_t NodeBase::GetNodeBaseCount()
{
	return m_count;
}

std::string_view NodeBase::GetName() const
{
	return m_name;
}

std::uint32_t NodeBase::GetType() const
{
	return m_type;
}


const NodeList& NodeBase::GetChildNodeList() const
{
	return m_childNodeList;
}

void NodeBase::SetChildNodes(NodeList&& childNodes)
{
	m_childNodeList = std::move(childNodes);
	for (auto child : m_childNodeList)
	{
		child->SetParent(weak_from_this());
	}
}

void NodeBase::AddChildNode(const NodePtr& node)
{
	m_childNodeList.push_back(node);
	node->SetParent(weak_from_this());
}

bool NodeBase::HasParent() const
{
	return !m_parent.expired();
}

NodePtr NodeBase::GetParent()
{
	return m_parent.lock();
}

void NodeBase::SetParent(const WeakNodePtr& parent)
{
	m_parent = parent;
}

} // namespace common
} // namespace d2_hack
