#include "NodeGraph/Graph.h"

#include "Core/Path.h"
#include "Serialize/Visitor.h"

void ngraph::Graph::Initialise()
{
	m_Registry.Initialise();
}

auto ngraph::Graph::GetField(const GraphId& graphId) const -> const ngraph::Field&
{
	const str::Guid& uuid = m_GraphMap.Get(graphId);
	return m_FieldMap.Get(uuid);
}

auto ngraph::Graph::GetLink(const str::Guid& uuid) const -> const ngraph::Link&
{
	return m_LinkMap.Get(uuid);
}

auto ngraph::Graph::GetLinks() const -> const LinkMap&
{
	return m_LinkMap;
}

auto ngraph::Graph::GetNode(const GraphId& graphId) const -> const ngraph::Node&
{
	const str::Guid& uuid = m_GraphMap.Get(graphId);
	return m_NodeMap.Get(uuid);
}

auto ngraph::Graph::GetNode(const str::Guid& uuid) const -> const ngraph::Node&
{
	return m_NodeMap.Get(uuid);
}

auto ngraph::Graph::GetNodes() const -> const NodeMap&
{
	return m_NodeMap;
}

auto ngraph::Graph::GetRegistry() const -> const ngraph::NodeRegistry&
{
	return m_Registry;
}

void ngraph::Graph::CreateLink(const GraphId sourceId, const GraphId targetId)
{
	const str::Guid uuid = str::Guid::Generate();
	Link& link = m_LinkMap[uuid];
	link.m_UUID = uuid;
	link.m_SourceId = sourceId;
	link.m_TargetId = targetId;
}

void ngraph::Graph::DestroyLink(const int32 index)
{
	m_LinkMap.RemoveAt(index);
}

void ngraph::Graph::CreateNode(const str::Name& name)
{
	Node node = m_Registry.Create(name);
	node.m_GraphId = m_GraphId++;
	node.m_UUID = str::Guid::Generate();

	for (ngraph::Field& field : node.m_Inputs)
	{
		field.m_GraphId = m_GraphId++;
		field.m_UUID = str::Guid::Generate();

		m_FieldMap.Insert(field.m_UUID, field);
		m_GraphMap.Insert(field.m_GraphId, field.m_UUID);
	}

	for (ngraph::Field& field : node.m_Outputs)
	{
		field.m_GraphId = m_GraphId++;
		field.m_UUID = str::Guid::Generate();

		m_FieldMap.Insert(field.m_UUID, field);
		m_GraphMap.Insert(field.m_GraphId, field.m_UUID);
	}

	m_NodeMap.Insert(node.m_UUID, node);
	m_GraphMap.Insert(node.m_GraphId, node.m_UUID);
}

void ngraph::Graph::DestroyNode(const str::Guid& uuid)
{
	m_NodeMap.Remove(uuid);
}