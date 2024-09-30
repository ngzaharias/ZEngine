#include "EnginePCH.h"
#include "Engine/SpriteComponent.h"

#include "Engine/AssetManager.h"
#include "Engine/InspectorHelpers.h"
#include "Engine/Visitor.h"
#include "imgui/Inspector.h"

namespace
{
	const str::StringView strColour = "m_Colour";
	const str::StringView strSize = "m_Size";
	const str::StringView strSprite = "m_Sprite";
}

template<>
void eng::Visitor::ReadCustom(eng::SpriteComponent& value) const
{
	Read(strSprite, value.m_Sprite, value.m_Sprite);
	Read(strColour, value.m_Colour, value.m_Colour);
	Read(strSize, value.m_Size, value.m_Size);
}
template<>
void eng::Visitor::WriteCustom(const eng::SpriteComponent& value)
{
	Write(strSprite, value.m_Sprite);
	Write(strColour, value.m_Colour);
	Write(strSize, value.m_Size);
}
template<>
bool imgui::Inspector::WriteCustom(eng::SpriteComponent& value)
{
	bool result = false;
	if (!HasPayload<eng::AssetManager>())
	{
		result |= Write("m_Sprite", value.m_Sprite);
	}
	else
	{
		const auto& manager = GetPayload<eng::AssetManager>();
		result |= WriteAsset("m_Sprite", value.m_Sprite, manager);
	}
	result |= Write("m_Colour", value.m_Colour);
	result |= Write("m_Size", value.m_Size);
	return result;
}