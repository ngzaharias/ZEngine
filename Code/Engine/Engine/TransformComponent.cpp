#include "EnginePCH.h"
#include "Engine/TransformComponent.h"

#include "Engine/Visitor.h"
#include "Math/Matrix.h"
#include "imgui/Inspector.h"

namespace
{
	const str::StringView strTranslate = "m_Translate";
	const str::StringView strRotate = "m_Rotate";
	const str::StringView strScale = "m_Scale";
}

template<>
void eng::Visitor::ReadCustom(eng::TransformComponent& value) const
{
	Read(strTranslate, value.m_Translate, value.m_Translate);
	Read(strRotate, value.m_Rotate, value.m_Rotate);
	Read(strScale, value.m_Scale, value.m_Scale);
}

template<>
void eng::Visitor::WriteCustom(const eng::TransformComponent& value)
{
	Write(strTranslate, value.m_Translate);
	Write(strRotate, value.m_Rotate);
	Write(strScale, value.m_Scale);
}

template<>
bool imgui::Inspector::WriteCustom(eng::TransformComponent& value)
{
	bool result = false;
	result |= Write("m_Translate", value.m_Translate);
	result |= Write("m_Rotate", value.m_Rotate);
	result |= Write("m_Scale", value.m_Scale);
	return result;
}

Matrix4x4 eng::TransformComponent::ToTransform() const
{
	return Matrix4x4::FromTransform(m_Translate, m_Rotate, m_Scale);
}
