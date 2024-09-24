#include "EnginePCH.h"
#include "Engine/TextComponent.h"

#include "Engine/Visitor.h"

namespace 
{
	const str::StringView strText = "m_Text";
	const str::StringView strFont = "m_Font";
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