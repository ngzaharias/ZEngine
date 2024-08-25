#include "EnginePCH.h"
#include "Engine/CameraPrototype.h"

#include "ECS/EntityWorld.h"
#include "Engine/CameraComponent.h"
#include "Engine/Visitor.h"

namespace
{
	const eng::camera::Orthographic s_DefaultOrthographic = { };
	const eng::camera::Perspective s_DefaultPerspective = { };
	const eng::camera::Prototype s_DefaultPrototype = {};

	const str::StringView strBehaviour = "m_Behaviour";
	const str::StringView strClippingFar = "m_ClippingFar";
	const str::StringView strClippingNear = "m_ClippingNear";
	const str::StringView strFieldOfView = "m_FieldOfView";
	const str::StringView strFoVAxis = "m_FoVAxis";
	const str::StringView strFrustrumEdgeMax = "m_FrustrumEdgeMax";
	const str::StringView strFrustrumEdgeMin = "m_FrustrumEdgeMin";
	const str::StringView strProjection = "m_Projection";
	const str::StringView strSize = "m_Size";
	const str::StringView strZoomMax = "m_ZoomMax";
	const str::StringView strZoomMin = "m_ZoomMin";
}

template<>
void eng::Visitor::ReadCustom(camera::Orthographic& value) const
{
	Read(strClippingNear, value.m_ClippingNear, s_DefaultOrthographic.m_ClippingNear);
	Read(strClippingFar, value.m_ClippingFar, s_DefaultOrthographic.m_ClippingFar);
	Read(strSize, value.m_Size, s_DefaultOrthographic.m_Size);
}

template<>
void eng::Visitor::ReadCustom(camera::Perspective& value) const
{
	Read(strClippingNear, value.m_ClippingNear, s_DefaultPerspective.m_ClippingNear);
	Read(strClippingFar, value.m_ClippingFar, s_DefaultPerspective.m_ClippingFar);
	Read(strFieldOfView, value.m_FieldOfView, s_DefaultPerspective.m_FieldOfView);
	Read(strFoVAxis, value.m_FoVAxis, s_DefaultPerspective.m_FoVAxis);
}

void eng::camera::Loader::Add(ecs::EntityWorld& world, const ecs::Entity& entity, const eng::camera::Prototype& prototype) const
{
	auto& projection = world.AddComponent<eng::camera::ProjectionComponent>(entity);
	projection.m_Projection = prototype.m_Projection;

	auto& behaviour = world.AddComponent<eng::camera::BehaviourComponent>(entity);
	behaviour.m_Behaviour = prototype.m_Behaviour;
	behaviour.m_FrustrumEdgeMax = prototype.m_FrustrumEdgeMax;
	behaviour.m_FrustrumEdgeMin = prototype.m_FrustrumEdgeMin;
	behaviour.m_ZoomMax = prototype.m_ZoomMax;
	behaviour.m_ZoomMin = prototype.m_ZoomMin;
}

void eng::camera::Loader::Load(eng::camera::Prototype& prototype, eng::Visitor& visitor) const
{
	visitor.Read(strProjection, prototype.m_Projection, s_DefaultPrototype.m_Projection);
	visitor.Read(strBehaviour, prototype.m_Behaviour, s_DefaultPrototype.m_Behaviour);
	visitor.Read(strFrustrumEdgeMax, prototype.m_FrustrumEdgeMax, s_DefaultPrototype.m_FrustrumEdgeMax);
	visitor.Read(strFrustrumEdgeMin, prototype.m_FrustrumEdgeMin, s_DefaultPrototype.m_FrustrumEdgeMin);
	visitor.Read(strZoomMax, prototype.m_ZoomMax, s_DefaultPrototype.m_ZoomMax);
	visitor.Read(strZoomMin, prototype.m_ZoomMin, s_DefaultPrototype.m_ZoomMin);
}