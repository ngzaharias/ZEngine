#include "CameraPCH.h"
#include "Camera/CameraZoom2DComponent.h"

#include "Engine/Visitor.h"
#include "imgui/Inspector.h"

namespace
{
	const str::StringView strMax = "m_Max";
	const str::StringView strMin = "m_Min";
}

template<>
void eng::Visitor::ReadCustom(camera::Zoom2DComponent& value) const
{
	Read(strMax, value.m_Max, value.m_Max);
	Read(strMin, value.m_Min, value.m_Min);
}
template<>
void eng::Visitor::WriteCustom(const camera::Zoom2DComponent& value)
{
	Write(strMax, value.m_Max);
	Write(strMin, value.m_Min);
}
template<>
bool imgui::Inspector::WriteCustom(camera::Zoom2DComponent& value)
{
	bool result = false;
	result |= Write("m_Max", value.m_Max);
	result |= Write("m_Min", value.m_Min);
	return result;
}