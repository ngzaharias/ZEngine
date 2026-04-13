#include "EnginePCH.h"
#include "Engine/TextTemplate.h"

#include "Serialize/Visitor.h"

namespace 
{
	const str::StringView strFont = "m_Font";
	const str::StringView strText = "m_Text";
}

template<>
void Visitor::ReadCustom(eng::TextTemplate& value) const
{
	Read(strText, value.m_Text, value.m_Text);
	Read(strFont, value.m_Font, value.m_Font);
}
template<>
void Visitor::WriteCustom(const eng::TextTemplate& value)
{
	Write(strText, value.m_Text);
	Write(strFont, value.m_Font);
}