#include "EditorPCH.h"
#include "Editor/GizmoSettings.h"

#include "Engine/Visitor.h"
#include "imgui/Inspector.h"

namespace
{
	const str::StringView strCoordAxes = "m_CoordAxes";
	const str::StringView strCrosshair = "m_Crosshair";
	const str::StringView strFloorGrid = "m_FloorGrid";
	const str::StringView strIsEnabled = "m_IsEnabled";
	const str::StringView strMajorColour = "m_MajorColour";
	const str::StringView strMajorScale = "m_MajorScale";
	const str::StringView strMajorWidth = "m_MajorWidth";
	const str::StringView strShowMajor = "m_ShowMajor";
	const str::StringView strShowMinor = "m_ShowMinor";
	const str::StringView strShowX = "m_ShowX";
	const str::StringView strShowY = "m_ShowY";
	const str::StringView strShowZ = "m_ShowZ";
	const str::StringView strTransform = "m_Transform";
}

template<>
void eng::Visitor::ReadCustom(editor::settings::Gizmos& value) const
{
	Read(strIsEnabled, value.m_IsEnabled, value.m_IsEnabled);
	Read(strCoordAxes, value.m_CoordAxes, value.m_CoordAxes);
	Read(strCrosshair, value.m_Crosshair, value.m_Crosshair);
	Read(strFloorGrid, value.m_FloorGrid, value.m_FloorGrid);
	Read(strTransform, value.m_Transform, value.m_Transform);
}
template<>
void eng::Visitor::WriteCustom(const editor::settings::Gizmos& value)
{
	Write(strIsEnabled, value.m_IsEnabled);
	Write(strCoordAxes, value.m_CoordAxes);
	Write(strCrosshair, value.m_Crosshair);
	Write(strFloorGrid, value.m_FloorGrid);
	Write(strTransform, value.m_Transform);
}
template<>
bool imgui::Inspector::WriteCustom(editor::settings::Gizmos& value)
{
	bool result = false;
	result |= Write("IsEnabled", value.m_IsEnabled);
	result |= Write("m_CoordAxes", value.m_CoordAxes);
	result |= Write("m_Crosshair", value.m_Crosshair);
	result |= Write("m_FloorGrid", value.m_FloorGrid);
	result |= Write("m_Transform", value.m_Transform);
	return result;
}

template<>
void eng::Visitor::ReadCustom(editor::settings::Gizmos::CoordAxes& value) const
{
	Read(strIsEnabled, value.m_IsEnabled, value.m_IsEnabled);
	Read(strShowX, value.m_ShowX, value.m_ShowX);
	Read(strShowY, value.m_ShowY, value.m_ShowY);
	Read(strShowZ, value.m_ShowZ, value.m_ShowZ);
}
template<>
void eng::Visitor::WriteCustom(const editor::settings::Gizmos::CoordAxes& value)
{
	Write(strIsEnabled, value.m_IsEnabled);
	Write(strShowX, value.m_ShowX);
	Write(strShowY, value.m_ShowY);
	Write(strShowZ, value.m_ShowZ);
}
template<>
bool imgui::Inspector::WriteCustom(editor::settings::Gizmos::CoordAxes& value)
{
	bool result = false;
	result |= Write("IsEnabled", value.m_IsEnabled);
	result |= Write("m_ShowX", value.m_ShowX);
	result |= Write("m_ShowY", value.m_ShowY);
	result |= Write("m_ShowZ", value.m_ShowZ);
	return result;
}

template<>
void eng::Visitor::ReadCustom(editor::settings::Gizmos::Crosshair& value) const
{
	Read(strIsEnabled, value.m_IsEnabled, value.m_IsEnabled);
}
template<>
void eng::Visitor::WriteCustom(const editor::settings::Gizmos::Crosshair& value)
{
	Write(strIsEnabled, value.m_IsEnabled);
}
template<>
bool imgui::Inspector::WriteCustom(editor::settings::Gizmos::Crosshair& value)
{
	bool result = false;
	result |= Write("IsEnabled", value.m_IsEnabled);
	return result;
}

template<>
void eng::Visitor::ReadCustom(editor::settings::Gizmos::FloorGrid& value) const
{
	Read(strIsEnabled, value.m_IsEnabled, value.m_IsEnabled);
	Read(strShowMajor, value.m_ShowMajor, value.m_ShowMajor);
	Read(strShowMinor, value.m_ShowMinor, value.m_ShowMinor);
	Read(strMajorScale, value.m_MajorScale, value.m_MajorScale);
	Read(strMajorWidth, value.m_MajorWidth, value.m_MajorWidth);
	Read(strMajorColour, value.m_MajorColour, value.m_MajorColour);
}
template<>
void eng::Visitor::WriteCustom(const editor::settings::Gizmos::FloorGrid& value)
{
	Write(strIsEnabled, value.m_IsEnabled);
	Write(strShowMajor, value.m_ShowMajor);
	Write(strShowMinor, value.m_ShowMinor);
	Write(strMajorScale, value.m_MajorScale);
	Write(strMajorWidth, value.m_MajorWidth);
	Write(strMajorColour, value.m_MajorColour);
}
template<>
bool imgui::Inspector::WriteCustom(editor::settings::Gizmos::FloorGrid& value)
{
	bool result = false;
	result |= Write("IsEnabled", value.m_IsEnabled);
	result |= Write("m_ShowMajor", value.m_ShowMajor);
	result |= Write("m_ShowMinor", value.m_ShowMinor);
	result |= Write("m_MajorScale", value.m_MajorScale);
	result |= Write("m_MajorWidth", value.m_MajorWidth);
	result |= Write("m_MajorColour", value.m_MajorColour);
	return result;
}

template<>
void eng::Visitor::ReadCustom(editor::settings::Gizmos::Transform& value) const
{
	Read(strIsEnabled, value.m_IsEnabled, value.m_IsEnabled);
}
template<>
void eng::Visitor::WriteCustom(const editor::settings::Gizmos::Transform& value)
{
	Write(strIsEnabled, value.m_IsEnabled);
}
template<>
bool imgui::Inspector::WriteCustom(editor::settings::Gizmos::Transform& value)
{
	bool result = false;
	result |= Write("IsEnabled", value.m_IsEnabled);
	return result;
}