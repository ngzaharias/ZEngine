#include "EnginePCH.h"
#include "Engine/TemplateHelpers.h"

#include "ECS/EntityWorld.h"
#include "Engine/TemplateComponent.h"
#include "Engine/TemplateMapComponent.h"

ecs::Entity eng::ToEntity(ToEntityWorld& world, const str::Guid& guid)
{
	const auto& mapComponent = world.ReadComponent<eng::TemplateMapComponent>();
	return mapComponent.m_GuidToEntity.Get(guid);
}

ecs::Entity eng::ToEntity(ecs::EntityWorld& world, const str::Guid& guid)
{
	const auto& mapComponent = world.ReadComponent<eng::TemplateMapComponent>();
	return mapComponent.m_GuidToEntity.Get(guid);
}

str::Guid eng::ToGuid(ToGuidWorld& world, const ecs::Entity& entity)
{
	const auto& templateComponent = world.ReadComponent<eng::TemplateComponent>(entity);
	return templateComponent.m_Guid;
}

str::Guid eng::ToGuid(ecs::EntityWorld& world, const ecs::Entity& entity)
{
	const auto& templateComponent = world.ReadComponent<eng::TemplateComponent>(entity);
	return templateComponent.m_Guid;
}