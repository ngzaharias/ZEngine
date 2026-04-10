#include "EnginePCH.h"
#include "Engine/VisibilityTemplate.h"

#include "Serialize/Visitor.h"

namespace
{
	const str::StringView strIsVisible = "m_IsVisible";
}

template<>
void Visitor::ReadCustom(eng::VisibilityTemplate& value) const
{
	Read(strIsVisible, value.m_IsVisible, value.m_IsVisible);
}
template<>
void Visitor::WriteCustom(const eng::VisibilityTemplate& value)
{
	Write(strIsVisible, value.m_IsVisible);
}