#include "EnginePCH.h"
#include "Engine/CameraTypes.h"

#include "Engine/Visitor.h"
#include "imgui/Inspector.h"

namespace
{
	const str::StringView strClippingFar = "m_ClippingFar";
	const str::StringView strClippingNear = "m_ClippingNear";
	const str::StringView strFieldOfView = "m_FieldOfView";
	const str::StringView strFoVAxis = "m_FoVAxis";
	const str::StringView strSize = "m_Size";
}

template<>
void eng::Visitor::ReadCustom(eng::camera::Cinematic& value) const
{
	Read(strClippingNear, value.m_ClippingNear, value.m_ClippingNear);
	Read(strClippingFar, value.m_ClippingFar, value.m_ClippingFar);
}

template<>
void eng::Visitor::WriteCustom(const eng::camera::Cinematic& value)
{
	Write(strClippingNear, value.m_ClippingNear);
	Write(strClippingFar, value.m_ClippingFar);
}

template<>
bool imgui::Inspector::WriteCustom(eng::camera::Cinematic& value)
{
	bool result = false;
	result |= Write("m_ClippingNear", value.m_ClippingNear);
	result |= Write("m_ClippingFar", value.m_ClippingFar);
	return result;
}

template<>
void eng::Visitor::ReadCustom(eng::camera::Orthographic& value) const
{
	Read(strClippingNear, value.m_ClippingNear, value.m_ClippingNear);
	Read(strClippingFar, value.m_ClippingFar, value.m_ClippingFar);
	Read(strSize, value.m_Size, value.m_Size);
}

template<>
void eng::Visitor::WriteCustom(const eng::camera::Orthographic& value)
{
	Write(strClippingNear, value.m_ClippingNear);
	Write(strClippingFar, value.m_ClippingFar);
	Write(strSize, value.m_Size);
}

template<>
bool imgui::Inspector::WriteCustom(eng::camera::Orthographic& value)
{
	bool result = false;
	result |= Write("m_ClippingNear", value.m_ClippingNear);
	result |= Write("m_ClippingFar", value.m_ClippingFar);
	result |= Write("m_Size", value.m_Size);
	return result;
}

template<>
void eng::Visitor::ReadCustom(eng::camera::Perspective& value) const
{
	Read(strClippingNear, value.m_ClippingNear, value.m_ClippingNear);
	Read(strClippingFar, value.m_ClippingFar, value.m_ClippingFar);
	Read(strFieldOfView, value.m_FieldOfView, value.m_FieldOfView);
	Read(strFoVAxis, value.m_FoVAxis, value.m_FoVAxis);
}

template<>
void eng::Visitor::WriteCustom(const eng::camera::Perspective& value)
{
	Write(strClippingNear, value.m_ClippingNear);
	Write(strClippingFar, value.m_ClippingFar);
	Write(strFieldOfView, value.m_FieldOfView);
	Write(strFoVAxis, value.m_FoVAxis);
}

template<>
bool imgui::Inspector::WriteCustom(eng::camera::Perspective& value)
{
	bool result = false;
	result |= Write("m_ClippingNear", value.m_ClippingNear);
	result |= Write("m_ClippingFar", value.m_ClippingFar);
	result |= Write("m_FieldOfView", value.m_FieldOfView);
	result |= Write("m_FoVAxis", value.m_FoVAxis);
	return result;
}