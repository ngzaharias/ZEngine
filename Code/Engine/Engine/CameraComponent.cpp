#include "EnginePCH.h"
#include "Engine/CameraComponent.h"

#include "Engine/Visitor.h"
#include "imgui/Inspector.h"

namespace
{
	const str::StringView strBounded = "m_Bounded";
	const str::StringView strClippingFar = "m_ClippingFar";
	const str::StringView strClippingNear = "m_ClippingNear";
	const str::StringView strFieldOfView = "m_FieldOfView";
	const str::StringView strFoVAxis = "m_FoVAxis";
	const str::StringView strMax = "m_Max";
	const str::StringView strMin = "m_Min";
	const str::StringView strMovement = "m_Movement";
	const str::StringView strPanning = "m_Panning";
	const str::StringView strProjection = "m_Projection";
	const str::StringView strSize = "m_Size";
	const str::StringView strZooming = "m_Zooming";
}

template<>
void eng::Visitor::ReadCustom(camera::Bound2DComponent& value) const
{
	Read(strMax, value.m_Max, value.m_Max);
	Read(strMin, value.m_Min, value.m_Min);
}

template<>
void eng::Visitor::ReadCustom(camera::Move2DComponent& value) const { }

template<>
void eng::Visitor::ReadCustom(camera::Move3DComponent& value) const { }

template<>
void eng::Visitor::ReadCustom(camera::Pan3DComponent& value) const { }

template<>
void eng::Visitor::ReadCustom(camera::Zoom2DComponent& value) const
{
	Read(strMax, value.m_Max, value.m_Max);
	Read(strMin, value.m_Min, value.m_Min);
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