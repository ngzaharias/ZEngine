#include "EnginePCH.h"
#include "Engine/ThemeTable.h"

#include "Engine/Visitor.h"

namespace 
{
	const str::StringView strBackground0 = "m_Background0";
	const str::StringView strBackground1 = "m_Background1";
	const str::StringView strBackground2 = "m_Background2";
	const str::StringView strHighlight = "m_Highlight";
	const str::StringView strOutline = "m_Outline";
	const str::StringView strName = "m_Name";

	const eng::Theme s_Fallback = { 
		NAME("_Fallback"), 
		Colour::White,
		Colour::Gray,
		Colour::Magenta,
		Colour::Black };
}

template<>
void eng::Visitor::ReadCustom(eng::Theme& value) const
{
	Read(strName, value.m_Name, value.m_Name);
	Read(strBackground0, value.m_Background0, value.m_Background0);
	Read(strHighlight, value.m_Highlight, value.m_Highlight);
	Read(strOutline, value.m_Outline, value.m_Outline);

	value.m_Background1 = value.m_Background0;
	value.m_Background1.r *= 0.9f;
	value.m_Background1.g *= 0.9f;
	value.m_Background1.b *= 0.9f;
	Read(strBackground1, value.m_Background1, value.m_Background1);

	value.m_Background2 = value.m_Background0;
	value.m_Background2.r *= 0.75f;
	value.m_Background2.g *= 0.75f;
	value.m_Background2.b *= 0.75f;
	Read(strBackground2, value.m_Background2, value.m_Background2);
}

const eng::Theme& eng::ThemeTable::GetTheme(const str::Guid& value) const
{
	const auto find = m_ObjectMap.Find(value);
	if (find != m_ObjectMap.end())
		return find->second;
	return s_Fallback;
}
