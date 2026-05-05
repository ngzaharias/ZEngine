#include "SpellEditorPCH.h"
#include "SpellEditor/SpellEditorHelpers.h"

#include "Core/Colour.h"
#include "Core/Name.h"
#include "Core/TypeName.h"
#include "NodeGraph/Link.h"
#include "NodeGraph/Node.h"

#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "imgui/imgui_user.h"
#include "imnodes/imnodes.h"

namespace
{
	constexpr Colour s_LightText = Colour(0xfff7e4);
	constexpr Colour s_DarkText = Colour(0x28282e);

	constexpr Colour s_NodeColours[] = {
		// https://lospec.com/palette-list/apollo
		Colour(0x172038),
		Colour(0x253a5e),
		Colour(0x3c5e8b),
		Colour(0x4f8fba),
		Colour(0x19332d),
		Colour(0x25562e),
		Colour(0x468232),
		Colour(0x75a743),
		Colour(0x4d2b32),
		Colour(0x7a4841),
		Colour(0xad7757),
		Colour(0xc09473),
		Colour(0x341c27),
		Colour(0x602c2c),
		Colour(0x884b2b),
		Colour(0xbe772b),
		Colour(0x241527),
		Colour(0x411d31),
		Colour(0x752438),
		Colour(0xa53030),
		Colour(0x1e1d39),
		Colour(0x402751),
		Colour(0x7a367b),
		Colour(0xa23e8c),
		Colour(0x090a14),
		Colour(0x10141f),
		Colour(0x151d28),
		Colour(0x202e37) };

	constexpr Colour s_PinColours[] = {
		// https://lospec.com/palette-list/apollo
		Colour(0xc7cfcc),
		Colour(0xebede9),
		Colour(0x73bed3),
		Colour(0xa4dddb),
		Colour(0xa8ca58),
		Colour(0xd0da91),
		Colour(0xd7b594),
		Colour(0xe7d5b3),
		Colour(0xde9e41),
		Colour(0xe8c170),
		Colour(0xcf573c),
		Colour(0xda863e),
		Colour(0xc65197),
		Colour(0xdf84a5) };

	constexpr uint32 s_NodeColoursCount = std::extent<decltype(s_NodeColours)>::value;
	constexpr uint32 s_PinColoursCount = std::extent<decltype(s_PinColours)>::value;

	uint32 ToColour(const Colour& colour)
	{
		return IM_COL32(
			std::min(colour.r * 255.f, 255.f),
			std::min(colour.g * 255.f, 255.f),
			std::min(colour.b * 255.f, 255.f),
			std::min(colour.a * 255.f, 255.f));
	}

	Colour ToNodeColour(const str::Name& value)
	{
		const str::Hash hash = value.ToHash();
		const uint32 index = hash % s_NodeColoursCount;
		return s_NodeColours[index];
	}

	Colour ToPinColour(const str::Name& value)
	{
		const str::Hash hash = value.ToHash();
		const uint32 index = hash % s_PinColoursCount;
		return s_PinColours[index];
	}
}

void editor::spell::PushFieldColour(const ngraph::Field& field)
{
	const Colour colour = !field.m_Colour
		? ToPinColour(field.m_Type)
		: *field.m_Colour;

	const uint32 normal = ToColour(colour);
	const uint32 highlight = ToColour(colour * 1.1f);
	ImNodes::PushColorStyle(ImNodesCol_Pin, normal);
	ImNodes::PushColorStyle(ImNodesCol_PinHovered, highlight);
}

void editor::spell::PopFieldColour()
{
	ImNodes::PopColorStyle();
	ImNodes::PopColorStyle();
}

void editor::spell::PushLinkColour(const ngraph::Link& link)
{
	const Colour colour = ToPinColour(link.m_Type);
	const uint32 normal = ToColour(colour);
	const uint32 highlight = ToColour(colour * 1.1f);
	ImNodes::PushColorStyle(ImNodesCol_Link, normal);
	ImNodes::PushColorStyle(ImNodesCol_LinkHovered, highlight);
	ImNodes::PushColorStyle(ImNodesCol_LinkSelected, highlight);
}

void editor::spell::PopLinkColour()
{
	ImNodes::PopColorStyle();
	ImNodes::PopColorStyle();
	ImNodes::PopColorStyle();
}

void editor::spell::PushNodeColour(const ngraph::Node& node)
{
	const Colour colour = !node.m_Colour
		? ToNodeColour(node.m_Type)
		: *node.m_Colour;

	const uint32 normal = ToColour(colour);
	const uint32 highlight = ToColour(colour * 1.1f);
	ImNodes::PushColorStyle(ImNodesCol_TitleBar, normal);
	ImNodes::PushColorStyle(ImNodesCol_TitleBarHovered, highlight);
	ImNodes::PushColorStyle(ImNodesCol_TitleBarSelected, highlight);

	const uint32 text = ToColour(s_LightText);
	ImGui::PushStyleColor(ImGuiCol_Text, text);
}

void editor::spell::PopNodeColour()
{
	ImGui::PopStyleColor();
	ImNodes::PopColorStyle();
	ImNodes::PopColorStyle();
	ImNodes::PopColorStyle();
}