#include "EnginePCH.h"
#include "Engine/CameraPrototype.h"

#include "ECS/EntityWorld.h"
#include "Engine/CameraComponent.h"
#include "Engine/Visitor.h"

namespace
{
	const camera::Orthographic s_DefaultOrthographic = { };
	const camera::Perspective s_DefaultPerspective = { };
	const eng::CameraPrototype s_DefaultPrototype = {};

	const str::StringView strBehaviour = "m_Behaviour";
	const str::StringView strClippingFar = "m_ClippingFar";
	const str::StringView strClippingNear = "m_ClippingNear";
	const str::StringView strFieldOfView = "m_FieldOfView";
	const str::StringView strFoVAxis = "m_FoVAxis";
	const str::StringView strProjection = "m_Projection";
	const str::StringView strSize = "m_Size";
}

template<>
void eng::Visitor::VisitCustom<camera::Orthographic>(camera::Orthographic& value)
{
	Visit(strClippingNear, value.m_ClippingNear, s_DefaultOrthographic.m_ClippingNear);
	Visit(strClippingFar, value.m_ClippingFar, s_DefaultOrthographic.m_ClippingFar);
	Visit(strSize, value.m_Size, s_DefaultOrthographic.m_Size);
}

template<>
void eng::Visitor::VisitCustom<camera::Perspective>(camera::Perspective& value)
{
	Visit(strClippingNear, value.m_ClippingNear, s_DefaultPerspective.m_ClippingNear);
	Visit(strClippingFar, value.m_ClippingFar, s_DefaultPerspective.m_ClippingFar);
	Visit(strFieldOfView, value.m_FieldOfView, s_DefaultPerspective.m_FieldOfView);
	Visit(strFoVAxis, value.m_FoVAxis, s_DefaultPerspective.m_FoVAxis);
}

void eng::CameraLoader::Add(ecs::EntityWorld& world, const ecs::Entity& entity, const eng::CameraPrototype& prototype) const
{
	auto& cameraComponent = world.AddComponent<eng::CameraComponent>(entity);
	cameraComponent.m_Projection = prototype.m_Projection;
	cameraComponent.m_Behaviour = prototype.m_Behaviour;
}

void eng::CameraLoader::Load(eng::CameraPrototype& prototype, eng::Visitor& visitor) const
{
	visitor.Visit(strBehaviour, prototype.m_Behaviour, s_DefaultPrototype.m_Behaviour);
	visitor.Visit(strProjection, prototype.m_Projection, s_DefaultPrototype.m_Projection);
}