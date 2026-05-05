#include "NodeGraph/NodeRegistry.h"

#include "Core/Path.h"
#include "NodeGraph/Node.h"
#include "Serialize/Visitor.h"

namespace
{
	const ngraph::NodeDefinition s_Fallback = { .m_Label = "unknown", .m_Tooltip = "unknown" };
}

void ngraph::NodeRegistry::Initialise()
{
	const str::Path path = str::Path(str::EPath::Config, "NodeDefinitions.toml");

	Visitor visitor;
	visitor.LoadFromFile(path);
	for (const str::StringView key : visitor)
	{
		const str::Name name = NAME(key);
		ngraph::NodeDefinition& node = m_NodeMap[name];
		visitor.Read(node);
	}
}

auto ngraph::NodeRegistry::GetDefinitions() const -> const NodeMap&
{
	return m_NodeMap;
}

ngraph::Node ngraph::NodeRegistry::Create(const str::Name& type)
{
	const ngraph::NodeDefinition& nodeDef = m_NodeMap.Get(type, s_Fallback);

	ngraph::Node node;
	node.m_Type = type;
	node.m_Label = nodeDef.m_Label;
	node.m_Tooltip = nodeDef.m_Tooltip;
	node.m_Colour = nodeDef.m_Colour;

	for (const ngraph::FieldDefinition& fieldDef : nodeDef.m_Inputs)
	{
		ngraph::Field& field = node.m_Inputs.Emplace();
		field.m_Type = fieldDef.m_Type;
		field.m_Label = fieldDef.m_Label;
		field.m_Tooltip = fieldDef.m_Tooltip;
	}

	for (const ngraph::FieldDefinition& fieldDef : nodeDef.m_Outputs)
	{
		ngraph::Field& field = node.m_Outputs.Emplace();
		field.m_Type = fieldDef.m_Type;
		field.m_Label = fieldDef.m_Label;
		field.m_Tooltip = fieldDef.m_Tooltip;
	}

	return node;
}
