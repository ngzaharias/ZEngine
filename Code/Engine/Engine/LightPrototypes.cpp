#include "EnginePCH.h"
#include "Engine/LightPrototypes.h"

#include "ECS/EntityWorld.h"
#include "Engine/LightComponents.h"
#include "Engine/Visitor.h"

namespace
{
	const str::StringView strColour = "m_Colour";
	const str::StringView strRange = "m_Range";
}

void eng::LightLoader::Add(ecs::EntityWorld& world, const ecs::Entity& entity, const eng::AmbientLightPrototype& prototype) const
{
	auto& component = world.AddComponent<eng::LightAmbientComponent>(entity);
	component.m_Colour = prototype.m_Colour;
}

void eng::LightLoader::Add(ecs::EntityWorld& world, const ecs::Entity& entity, const eng::DirectionalLightPrototype& prototype) const
{
	auto& component = world.AddComponent<eng::LightDirectionalComponent>(entity);
	component.m_Colour = prototype.m_Colour;
}

void eng::LightLoader::Add(ecs::EntityWorld& world, const ecs::Entity& entity, const eng::PointLightPrototype& prototype) const
{
	auto& component = world.AddComponent<eng::LightPointComponent>(entity);
	component.m_Range = prototype.m_Range;
	component.m_Colour = prototype.m_Colour;
}

void eng::LightLoader::Load(eng::AmbientLightPrototype& prototype, eng::Visitor& visitor) const
{
	visitor.Read(strColour, prototype.m_Colour, Vector3f::One);
}

void eng::LightLoader::Load(eng::DirectionalLightPrototype& prototype, eng::Visitor& visitor) const
{
	visitor.Read(strColour, prototype.m_Colour, Vector3f::One);
}

void eng::LightLoader::Load(eng::PointLightPrototype& prototype, eng::Visitor& visitor) const
{
	visitor.Read(strRange, prototype.m_Range, 500.f);
	visitor.Read(strColour, prototype.m_Colour, Vector3f::One);
}