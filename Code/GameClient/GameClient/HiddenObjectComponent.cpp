#include "GameClientPCH.h"
#include "GameClient/HiddenObjectComponent.h"

#include "Engine/AssetManager.h"
#include "Engine/InspectorHelpers.h"
#include "Engine/Visitor.h"
#include "imgui/Inspector.h"

namespace
{
	const str::StringView strGroup = "m_Group";
	const str::StringView strSize = "m_Size";
	const str::StringView strSprite = "m_Sprite";
}

template<>
void eng::Visitor::ReadCustom(hidden::ObjectComponent& value) const
{
	Read(strSprite, value.m_Sprite, value.m_Sprite);
	Read(strSize, value.m_Size, value.m_Size);
	Read(strGroup, value.m_Group, value.m_Group);
}
template<>
void eng::Visitor::WriteCustom(const hidden::ObjectComponent& value)
{
	Write(strSprite, value.m_Sprite);
	Write(strSize, value.m_Size);
	Write(strGroup, value.m_Group);
}
template<>
bool imgui::Inspector::WriteCustom(hidden::ObjectComponent& value)
{
	const auto& manager = GetPayload<eng::AssetManager>();

	bool result = false;
	result |= WriteAsset("m_Sprite", value.m_Sprite, manager);
	result |= Write("m_Size", value.m_Size);
	result |= Write("m_Group", value.m_Group);
	return result;
}