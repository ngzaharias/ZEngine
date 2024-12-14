#include "HiddenPCH.h"
#include "Hidden/HiddenGroupComponent.h"

#include "Engine/AssetManager.h"
#include "Engine/InspectorHelpers.h"
#include "Engine/Visitor.h"
#include "imgui/Inspector.h"

namespace
{
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
}
template<>
void eng::Visitor::WriteCustom(const hidden::GroupComponent& value)
{
	Write(strSprite, value.m_Sprite);
	Write(strSize, value.m_Size);
	Write(strObjects, value.m_Objects);
}
template<>
bool imgui::Inspector::WriteCustom(hidden::GroupComponent& value)
{
	const auto& manager = GetPayload<eng::AssetManager>();

	bool result = false;
	result |= WriteAsset("m_Sprite", value.m_Sprite, manager);
	result |= Write("m_Size", value.m_Size);
	result |= Write("m_Objects", value.m_Objects);
	Read("m_Revealed", value.m_Revealed);
	return result;
}