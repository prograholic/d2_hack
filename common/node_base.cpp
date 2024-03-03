#include <d2_hack/common/node_base.h>


namespace d2_hack
{
namespace common
{

NodeBase::NodeBase(const std::string& name, std::uint32_t type)
	: m_childNodeList()
	, m_parent()
	, m_name(name)
	, m_type(type)
{
}

const std::string& NodeBase::GetName() const
{
	return m_name;
}

std::uint32_t NodeBase::GetType() const
{
	return m_type;
}


NodeList& NodeBase::GetChildNodeList()
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
