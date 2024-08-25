#include "EnginePCH.h"
#include "Engine/CameraPrototype.h"

#include "ECS/EntityWorld.h"
#include "Engine/CameraComponent.h"
#include "Engine/Visitor.h"

namespace
{
	const str::StringView strBound = "m_Bound";
	const str::StringView strClippingFar = "m_ClippingFar";
	const str::StringView strClippingNear = "m_ClippingNear";
	const str::StringView strFieldOfView = "m_FieldOfView";
	const str::StringView strFoVAxis = "m_FoVAxis";
	const str::StringView strMax = "m_Max";
	const str::StringView strMin = "m_Min";
	const str::StringView strMove = "m_Move";
	const str::StringView strProjection = "m_Projection";
	const str::StringView strSize = "m_Size";
	const str::StringView strZoom = "m_Zoom";
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

	auto& behaviour = world.AddComponent<eng::camera::BehaviourComponent>(entity);
	behaviour.m_Bound = prototype.m_Bound;
	behaviour.m_Move = prototype.m_Move;
	behaviour.m_Zoom = prototype.m_Zoom;
}

void eng::camera::Loader::Load(eng::camera::Prototype& prototype, eng::Visitor& visitor) const
{
	visitor.Read(strProjection, prototype.m_Projection, prototype.m_Projection);
	visitor.Read(strBound, prototype.m_Bound, prototype.m_Bound);
	visitor.Read(strMove, prototype.m_Move, prototype.m_Move);
	visitor.Read(strZoom, prototype.m_Zoom, prototype.m_Zoom);
}