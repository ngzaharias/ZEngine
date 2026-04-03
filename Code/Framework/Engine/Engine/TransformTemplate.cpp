#include "EnginePCH.h"
#include "Engine/TransformTemplate.h"

#include "Serialize/Visitor.h"

#include "imgui/Inspector.h"

namespace
{
	const str::StringView strRotate = "m_Rotate";
	const str::StringView strScale = "m_Scale";
	const str::StringView strTranslate = "m_Translate";
}

template<>
void Visitor::ReadCustom(eng::TransformTemplate& value) const
{
	Read(strTranslate, value.m_Translate, value.m_Translate);
	Read(strRotate, value.m_Rotate, value.m_Rotate);
	Read(strScale, value.m_Scale, value.m_Scale);
}
template<>
void Visitor::WriteCustom(const eng::TransformTemplate& value)
{
	Write(strTranslate, value.m_Translate);
	Write(strRotate, value.m_Rotate);
	Write(strScale, value.m_Scale);
}
template<>
bool imgui::Inspector::WriteCustom(eng::TransformTemplate& value)
{
	bool result = false;
	result |= Write("m_Translate", value.m_Translate);
	result |= Write("m_Rotate", value.m_Rotate);
	result |= Write("m_Scale", value.m_Scale);
	return result;
}