#include "NodeGraph/NodeDefinition.h"

#include "Serialize/Visitor.h"

namespace
{
	const str::StringView strColour = "m_Colour";
	const str::StringView strInputs = "m_Inputs";
	const str::StringView strLabel = "m_Label";
	const str::StringView strOutputs = "m_Outputs";
	const str::StringView strTooltip = "m_Tooltip";
	const str::StringView strType = "m_Type";
}

template<>
void Visitor::ReadCustom(ngraph::FieldDefinition& value) const
{
	Read(strType, value.m_Type, value.m_Type);
	Read(strLabel, value.m_Label, value.m_Label);
	Read(strTooltip, value.m_Tooltip, value.m_Tooltip);
}
template<>
void Visitor::WriteCustom(const ngraph::FieldDefinition& value)
{
	Write(strType, value.m_Type);
	Write(strLabel, value.m_Label);
	Write(strTooltip, value.m_Tooltip);
}

template<>
void Visitor::ReadCustom(ngraph::NodeDefinition& value) const
{
	Read(strColour, value.m_Colour, Colour(0.16f, 0.29f, 0.47f, 1.f));
	Read(strLabel, value.m_Label, value.m_Label);
	Read(strTooltip, value.m_Tooltip, value.m_Tooltip);
	Read(strInputs, value.m_Inputs, value.m_Inputs);
	Read(strOutputs, value.m_Outputs, value.m_Outputs);
}
template<>
void Visitor::WriteCustom(const ngraph::NodeDefinition& value)
{
	Write(strColour, value.m_Colour);
	Write(strLabel, value.m_Label);
	Write(strTooltip, value.m_Tooltip);
	Write(strInputs, value.m_Inputs);
	Write(strOutputs, value.m_Outputs);
}

