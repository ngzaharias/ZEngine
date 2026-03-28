#include "EnginePCH.h"
#include "Engine/CameraComponent.h"

#include "Serialize/Visitor.h"

#include "imgui/Inspector.h"

namespace
{
	const str::StringView strProjection = "m_Projection";
}

template<>
void Visitor::ReadCustom(eng::CameraComponent& value) const
{
	Read(strProjection, value.m_Projection, value.m_Projection);
}
template<>
void Visitor::WriteCustom(const eng::CameraComponent& value)
{
	Write(strProjection, value.m_Projection);
}
template<>
bool imgui::Inspector::WriteCustom(eng::CameraComponent& value)
{
	return Write("m_Projection", value.m_Projection);
}