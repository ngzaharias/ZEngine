#include "EnginePCH.h"
#include "Engine/CameraComponent.h"

#include "Engine/Visitor.h"
#include "imgui/Inspector.h"

namespace
{
	const str::StringView strMax = "m_Max";
	const str::StringView strMin = "m_Min";
	const str::StringView strProjection = "m_Projection";
}

template<>
void eng::Visitor::ReadCustom(eng::camera::Bound2DComponent& value) const
{
	Read(strMax, value.m_Max, value.m_Max);
	Read(strMin, value.m_Min, value.m_Min);
}
template<>
void eng::Visitor::WriteCustom(const eng::camera::Bound2DComponent& value) 
{ 
	Write(strMax, value.m_Max);
	Write(strMin, value.m_Min);
}
template<>
bool imgui::Inspector::WriteCustom(eng::camera::Bound2DComponent& value)
{
	bool result = false;
	result |= Write("m_Max", value.m_Max);
	result |= Write("m_Min", value.m_Min);
	return result;
}

template<>
void eng::Visitor::ReadCustom(eng::camera::Move2DComponent& value) const { }
template<>
void eng::Visitor::WriteCustom(const eng::camera::Move2DComponent& value) { }
template<>
bool imgui::Inspector::WriteCustom(eng::camera::Move2DComponent& value) { return false; }

template<>
void eng::Visitor::ReadCustom(eng::camera::Move3DComponent& value) const { }
template<>
void eng::Visitor::WriteCustom(const eng::camera::Move3DComponent& value) { }
template<>
bool imgui::Inspector::WriteCustom(eng::camera::Move3DComponent& value) { return false; }

template<>
void eng::Visitor::ReadCustom(eng::camera::Pan3DComponent& value) const { }
template<>
void eng::Visitor::WriteCustom(const eng::camera::Pan3DComponent& value) { }
template<>
bool imgui::Inspector::WriteCustom(eng::camera::Pan3DComponent& value) { return false; }

template<>
void eng::Visitor::ReadCustom(eng::camera::Zoom2DComponent& value) const
{
	Read(strMax, value.m_Max, value.m_Max);
	Read(strMin, value.m_Min, value.m_Min);
}
template<>
void eng::Visitor::WriteCustom(const eng::camera::Zoom2DComponent& value)
{
	Write(strMax, value.m_Max);
	Write(strMin, value.m_Min);
}
template<>
bool imgui::Inspector::WriteCustom(eng::camera::Zoom2DComponent& value)
{
	bool result = false;
	result |= Write("m_Max", value.m_Max);
	result |= Write("m_Min", value.m_Min);
	return result;
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