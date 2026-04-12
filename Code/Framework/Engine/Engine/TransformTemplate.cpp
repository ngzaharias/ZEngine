#include "EnginePCH.h"
#include "Engine/TransformTemplate.h"

#include "Serialize/Visitor.h"

namespace
{
	const str::StringView strRotate = "m_Rotate";
	const str::StringView strScale = "m_Scale";
	const str::StringView strTranslate = "m_Translate";
}

Matrix4x4 eng::TransformTemplate::ToTransform() const
{
	return Matrix4x4::FromTransform(m_Translate, m_Rotate, m_Scale);
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