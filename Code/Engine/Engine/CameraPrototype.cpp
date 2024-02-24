#include "EnginePCH.h"
#include "Engine/CameraPrototype.h"

#include <ECS/EntityWorld.h>

#include "Engine/CameraComponent.h"
#include "Engine/Visitor.h"

namespace
{
	const camera::Orthographic s_DefaultOrthographic = { };
	const camera::Perspective s_DefaultPerspective = { };
	const camera::Projection s_DefaultProjection = s_DefaultPerspective;
}

template<>
void eng::Visitor::VisitCustom<camera::Orthographic>(camera::Orthographic& value)
{
	Visit("m_ClippingNear", value.m_ClippingNear, s_DefaultOrthographic.m_ClippingNear);
	Visit("m_ClippingFar", value.m_ClippingFar, s_DefaultOrthographic.m_ClippingFar);
	Visit("m_Size", value.m_Size, s_DefaultOrthographic.m_Size);
}

template<>
void eng::Visitor::VisitCustom<camera::Perspective>(camera::Perspective& value)
{
	Visit("m_ClippingNear", value.m_ClippingNear, s_DefaultPerspective.m_ClippingNear);
	Visit("m_ClippingFar", value.m_ClippingFar, s_DefaultPerspective.m_ClippingFar);
	Visit("m_FieldOfView", value.m_FieldOfView, s_DefaultPerspective.m_FieldOfView);
	Visit("m_FoVAxis", value.m_FoVAxis, s_DefaultPerspective.m_FoVAxis);
}

void eng::CameraPrototypeLoader::Add(ecs::EntityWorld& world, const ecs::Entity& entity, const CameraPrototype& prototype) const
{
	auto& cameraComponent = world.AddComponent<eng::CameraComponent>(entity);
	cameraComponent.m_Projection = prototype.m_Projection;
}

void eng::CameraPrototypeLoader::Load(eng::CameraPrototype& prototype, eng::Visitor& visitor) const
{
	visitor.Visit("m_Projection", prototype.m_Projection, s_DefaultProjection);
}