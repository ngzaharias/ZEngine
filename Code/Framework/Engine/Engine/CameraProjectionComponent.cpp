#include "EnginePCH.h"
#include "Engine/CameraProjectionComponent.h"

#include "Engine/Visitor.h"
#include "imgui/Inspector.h"

namespace
{
	const str::StringView strProjection = "m_Projection";
}

template<>
void eng::Visitor::ReadCustom(eng::camera::ProjectionComponent& value) const
{
	Read(strProjection, value.m_Projection, value.m_Projection);
}
template<>
void eng::Visitor::WriteCustom(const eng::camera::ProjectionComponent& value)
{
	Write(strProjection, value.m_Projection);
}
template<>
bool imgui::Inspector::WriteCustom(eng::camera::ProjectionComponent& value)
{
	return Write("m_Projection", value.m_Projection);
}