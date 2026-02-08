#include "EnginePCH.h"
#include "Engine/TextComponent.h"

#include "Engine/InspectorHelpers.h"
#include "Engine/Visitor.h"
#include "imgui/Inspector.h"

namespace 
{
	const str::StringView strFont = "m_Font";
	const str::StringView strText = "m_Text";
}

template<>
void eng::Visitor::ReadCustom(eng::TextComponent& value) const
{
	Read(strText, value.m_Text, value.m_Text);
	Read(strFont, value.m_Font, value.m_Font);
}
template<>
void eng::Visitor::WriteCustom(const eng::TextComponent& value)
{
	Write(strText, value.m_Text);
	Write(strFont, value.m_Font);
}
template<>
bool imgui::Inspector::WriteCustom(eng::TextComponent& value)
{
	bool result = false;
	result |= Write("m_Text", value.m_Text);
	if (!HasPayload<eng::AssetManager>())
	{
		result |= Write("m_Font", value.m_Font);
	}
	else
	{
		const auto& manager = GetPayload<eng::AssetManager>();
		result |= WriteFont(manager, "m_Font", value.m_Font);
	}
	return result;
}