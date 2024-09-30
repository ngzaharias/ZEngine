#include "GameClientPCH.h"
#include "GameClient/HiddenObjectComponent.h"

#include "Engine/AssetManager.h"
#include "Engine/InspectorHelpers.h"
#include "Engine/Visitor.h"
#include "imgui/Inspector.h"

namespace
{
	const str::StringView strColour = "m_Colour";
	const str::StringView strEffects = "m_Effects";
	const str::StringView strGroup = "m_Group";
	const str::StringView strSprite = "m_Sprite";
}

template<>
void eng::Visitor::ReadCustom(hidden::SetColour& value) const
{
	Read(strColour, value.m_Colour, value.m_Colour);
}
template<>
void eng::Visitor::WriteCustom(const hidden::SetColour& value)
{
	Write(strColour, value.m_Colour);
}
template<>
bool imgui::Inspector::WriteCustom(hidden::SetColour& value)
{
	bool result = false;
	result |= Write("m_Colour", value.m_Colour);
	return result;
}

template<>
void eng::Visitor::ReadCustom(hidden::SetSprite& value) const
{
	Read(strSprite, value.m_Sprite, value.m_Sprite);
}
template<>
void eng::Visitor::WriteCustom(const hidden::SetSprite& value)
{
	Write(strSprite, value.m_Sprite);
}
template<>
bool imgui::Inspector::WriteCustom(hidden::SetSprite& value)
{
	bool result = false;
	result |= Write("m_Sprite", value.m_Sprite);
	return result;
}

template<>
void eng::Visitor::ReadCustom(hidden::ObjectComponent& value) const
{
	Read(strGroup, value.m_Group, value.m_Group);
	Read(strEffects, value.m_Effects, value.m_Effects);
}
template<>
void eng::Visitor::WriteCustom(const hidden::ObjectComponent& value)
{
	Write(strGroup, value.m_Group);
	Write(strEffects, value.m_Effects);
}
template<>
bool imgui::Inspector::WriteCustom(hidden::ObjectComponent& value)
{
	bool result = false;
	result |= Write("m_Group", value.m_Group);
	result |= Write("m_Effects", value.m_Effects);
	return result;
}