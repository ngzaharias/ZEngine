#include "GameClientPCH.h"
#include "GameClient/HiddenObjectComponents.h"

#include "Engine/Visitor.h"
#include "imgui/Inspector.h"

namespace
{
	const str::StringView strGroup = "m_Group";
	const str::StringView strObjects = "m_Objects";
	const str::StringView strRevealed = "m_Revealed";
	const str::StringView strSize = "m_Size";
	const str::StringView strSprite = "m_Sprite";
}

template<>
void eng::Visitor::ReadCustom(hidden::GroupComponent& value) const
{
	Read(strSprite, value.m_Sprite, value.m_Sprite);
	Read(strSize, value.m_Size, value.m_Size);
	Read(strObjects, value.m_Objects, value.m_Objects);
	Read(strRevealed, value.m_Revealed, value.m_Revealed);
}

template<>
void eng::Visitor::WriteCustom(const hidden::GroupComponent& value)
{
	Write(strSprite, value.m_Sprite);
	Write(strSize, value.m_Size);
	Write(strObjects, value.m_Objects);
	Write(strRevealed, value.m_Revealed);
}

template<>
bool imgui::Inspector::WriteCustom(hidden::GroupComponent& value)
{
	bool result = false;
	result |= Write("m_Sprite", value.m_Sprite);
	result |= Write("m_Size", value.m_Size);
	result |= Write("m_Objects", value.m_Objects);
	result |= Write("m_Revealed", value.m_Revealed);
	return result;
}

template<>
void eng::Visitor::ReadCustom(hidden::ObjectComponent& value) const
{
	Read(strSize, value.m_Size, value.m_Size);
	Read(strGroup, value.m_Group, value.m_Group);
	Read(strSprite, value.m_Sprite, value.m_Sprite);
}

template<>
void eng::Visitor::WriteCustom(const hidden::ObjectComponent& value)
{
	Write(strSize, value.m_Size);
	Write(strGroup, value.m_Group);
	Write(strSprite, value.m_Sprite);
}

template<>
bool imgui::Inspector::WriteCustom(hidden::ObjectComponent& value)
{
	bool result = false;
	result |= Write("m_Size", value.m_Size);
	result |= Write("m_Group", value.m_Group);
	result |= Write("m_Sprite", value.m_Sprite);
	return result;
}