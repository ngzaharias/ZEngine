#include "EnginePCH.h"
#include "Engine/CameraPrototype.h"

#include "Core/VariantHelpers.h"
#include "ECS/EntityWorld.h"
#include "Engine/CameraComponent.h"
#include "Engine/Visitor.h"

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
void eng::Visitor::ReadCustom(camera::Bound2D& value) const
{
	Read(strMax, value.m_Max, value.m_Max);
	Read(strMin, value.m_Min, value.m_Min);
}

template<>
void eng::Visitor::ReadCustom(camera::Move2D& value) const { }

template<>
void eng::Visitor::ReadCustom(camera::Move3D& value) const { }

template<>
void eng::Visitor::ReadCustom(camera::Pan3D& value) const { }

template<>
void eng::Visitor::ReadCustom(camera::Zoom2D& value) const
{
	Read(strMax, value.m_Max, value.m_Max);
	Read(strMin, value.m_Min, value.m_Min);
}

template<>
void eng::Visitor::ReadCustom(camera::Orthographic& value) const
{
	Read(strClippingNear, value.m_ClippingNear, value.m_ClippingNear);
	Read(strClippingFar, value.m_ClippingFar, value.m_ClippingFar);
	Read(strSize, value.m_Size, value.m_Size);
}

template<>
void eng::Visitor::ReadCustom(camera::Perspective& value) const
{
	Read(strClippingNear, value.m_ClippingNear, value.m_ClippingNear);
	Read(strClippingFar, value.m_ClippingFar, value.m_ClippingFar);
	Read(strFieldOfView, value.m_FieldOfView, value.m_FieldOfView);
	Read(strFoVAxis, value.m_FoVAxis, value.m_FoVAxis);
}

void eng::camera::Loader::Add(ecs::EntityWorld& world, const ecs::Entity& entity, const eng::camera::Prototype& prototype) const
{
	auto& projection = world.AddComponent<eng::camera::ProjectionComponent>(entity);
	projection.m_Projection = prototype.m_Projection;

	if (prototype.m_Bounded)
	{
		core::VariantMatch(*prototype.m_Bounded,
			[&](const eng::camera::Bound2D& data)
			{
				auto& component = world.AddComponent<eng::camera::Bound2DComponent>(entity);
				component.m_Max = data.m_Max;
				component.m_Min = data.m_Min;
			});
	}

	if (prototype.m_Movement)
	{
		core::VariantMatch(*prototype.m_Movement,
			[&](const eng::camera::Move2D& data)
			{
				world.AddComponent<eng::camera::Move2DComponent>(entity);
			},
			[&](const eng::camera::Move3D& data)
			{
				world.AddComponent<eng::camera::Move3DComponent>(entity);
			});
	}

	if (prototype.m_Panning)
	{
		core::VariantMatch(*prototype.m_Panning,
			[&](const eng::camera::Pan3D& data)
			{
				world.AddComponent<eng::camera::Pan3DComponent>(entity);
			});
	}

	if (prototype.m_Zooming)
	{
		core::VariantMatch(*prototype.m_Zooming,
			[&](const eng::camera::Zoom2D& data)
			{
				auto& component = world.AddComponent<eng::camera::Zoom2DComponent>(entity);
				component.m_Max = data.m_Max;
				component.m_Min = data.m_Min;
			});
	}
}

void eng::camera::Loader::Load(eng::camera::Prototype& prototype, eng::Visitor& visitor) const
{
	visitor.Read(strProjection, prototype.m_Projection, prototype.m_Projection);
	visitor.Read(strBounded, prototype.m_Bounded, prototype.m_Bounded);
	visitor.Read(strMovement, prototype.m_Movement, prototype.m_Movement);
	visitor.Read(strPanning, prototype.m_Panning, prototype.m_Panning);
	visitor.Read(strZooming, prototype.m_Zooming, prototype.m_Zooming);
}