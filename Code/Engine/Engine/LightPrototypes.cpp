#include "EnginePCH.h"
#include "Engine/LightPrototypes.h"

#include <ECS/EntityWorld.h>

#include "Engine/LightComponents.h"
#include "Engine/Visitor.h"

namespace
{
	const str::Name strColour = NAME("m_Colour");
	const str::Name strRange = NAME("m_Range");
}

void eng::LightPrototypeLoader::Add(ecs::EntityWorld& world, const ecs::Entity& entity, const AmbientLightPrototype& prototype) const
{
	auto& component = world.AddComponent<eng::LightAmbientComponent>(entity);
	component.m_Colour = prototype.m_Colour;
}

void eng::LightPrototypeLoader::Add(ecs::EntityWorld& world, const ecs::Entity& entity, const DirectionalLightPrototype& prototype) const
{
	auto& component = world.AddComponent<eng::LightDirectionalComponent>(entity);
	component.m_Colour = prototype.m_Colour;
}

void eng::LightPrototypeLoader::Add(ecs::EntityWorld& world, const ecs::Entity& entity, const PointLightPrototype& prototype) const
{
	auto& component = world.AddComponent<eng::LightPointComponent>(entity);
	component.m_Range = prototype.m_Range;
	component.m_Colour = prototype.m_Colour;
}

void eng::LightPrototypeLoader::Load(eng::AmbientLightPrototype& prototype, eng::Visitor& visitor) const
{
	visitor.Visit(strColour, prototype.m_Colour, Vector3f::One);
}

void eng::LightPrototypeLoader::Load(eng::DirectionalLightPrototype& prototype, eng::Visitor& visitor) const
{
	visitor.Visit(strColour, prototype.m_Colour, Vector3f::One);
}

void eng::LightPrototypeLoader::Load(eng::PointLightPrototype& prototype, eng::Visitor& visitor) const
{
	visitor.Visit(strRange, prototype.m_Range, 500.f);
	visitor.Visit(strColour, prototype.m_Colour, Vector3f::One);
}