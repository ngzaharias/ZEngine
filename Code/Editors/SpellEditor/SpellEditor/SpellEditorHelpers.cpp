#include "SpellEditorPCH.h"
#include "SpellEditor/SpellEditorHelpers.h"

#include "Core/Colour.h"
#include "Core/Name.h"
#include "Core/TypeName.h"
#include "NodeGraph/Node.h"

#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "imgui/imgui_user.h"
#include "imnodes/imnodes.h"

namespace
{
	uint32 ToColour(const Colour& colour)
	{
		return IM_COL32(
			colour.r * 255.f,
			colour.g * 255.f,
			colour.b * 255.f,
			colour.a * 255.f);
	}

	uint32 ToColour(const str::Name& name)
	{
		if (name == TypeName<bool>::m_WithNamespace)
			return 0xFFFF0000;

		if (name == TypeName<float>::m_WithNamespace)
			return 0xFF00FF00;

		if (name == TypeName<int32>::m_WithNamespace)
			return 0xFF0000FF;
		if (name == TypeName<uint32>::m_WithNamespace)
			return 0xFF0000FF;

		if (name == TypeName<Vector2f>::m_WithNamespace)
			return 0xFFFFFF00;
		if (name == TypeName<Vector3f>::m_WithNamespace)
			return 0xFFFFFF00;
		if (name == TypeName<Vector4f>::m_WithNamespace)
			return 0xFFFFFF00;

		return 0xFFFFFFFF;
	}

}

void editor::spell::PushFieldColour(const ngraph::Field& field)
{
	const uint32 normal = ToColour(field.m_Type) - 0x0F000000;
	const uint32 highlight = ToColour(field.m_Type);
	ImNodes::PushColorStyle(ImNodesCol_Pin, normal);
	ImNodes::PushColorStyle(ImNodesCol_PinHovered, highlight);
}

void editor::spell::PopFieldColour()
{
	ImNodes::PopColorStyle();
	ImNodes::PopColorStyle();
}

void editor::spell::PushNodeColour(const ngraph::Node& node)
{
	const uint32 normal = ToColour(node.m_Colour) - 0x0F000000;
	const uint32 highlight = ToColour(node.m_Colour);
	ImNodes::PushColorStyle(ImNodesCol_TitleBar, normal);
	ImNodes::PushColorStyle(ImNodesCol_TitleBarHovered, highlight);
	ImNodes::PushColorStyle(ImNodesCol_TitleBarSelected, highlight);
}

void editor::spell::PopNodeColour()
{
	ImNodes::PopColorStyle();
	ImNodes::PopColorStyle();
	ImNodes::PopColorStyle();
}