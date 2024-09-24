#include "EnginePCH.h"
#include "Engine/SpriteComponent.h"

#include "Engine/AssetManager.h"
#include "Engine/InspectorHelpers.h"
#include "Engine/Visitor.h"
#include "imgui/Inspector.h"

namespace
{
	const str::StringView m_Size = "m_Size";
	const str::StringView m_Sprite = "m_Sprite";
}

template<>
void eng::Visitor::ReadCustom(eng::SpriteComponent& value) const
{
	Read(m_Sprite, value.m_Sprite, value.m_Sprite);
	Read(m_Size, value.m_Size, value.m_Size);
}
template<>
void eng::Visitor::WriteCustom(const eng::SpriteComponent& value)
{
	Write(m_Sprite, value.m_Sprite);
	Write(m_Size, value.m_Size);
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
	result |= Write("m_Size", value.m_Size);
	return result;
}