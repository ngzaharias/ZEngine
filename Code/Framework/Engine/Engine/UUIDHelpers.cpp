#include "EnginePCH.h"
#include "Engine/UUIDHelpers.h"

#include "ECS/EntityWorld.h"
#include "Engine/UUIDComponent.h"
#include "Engine/UUIDMapComponent.h"

ecs::Entity eng::ToEntity(ToEntityWorld& world, const str::Guid& guid)
{
	if (!guid.IsValid())
		return {};

	const auto& mapComponent = world.ReadComponent<eng::UUIDMapComponent>();
	return mapComponent.m_ToEntity.Get(guid);
}

ecs::Entity eng::ToEntity(ecs::EntityWorld& world, const str::Guid& guid)
{
	if (!guid.IsValid())
		return {};

	const auto& mapComponent = world.ReadComponent<eng::UUIDMapComponent>();
	return mapComponent.m_ToEntity.Get(guid);
}

str::Guid eng::ToUUID(ToUUIDWorld& world, const ecs::Entity& entity)
{
	if (entity.IsUnassigned())
		return {};

	const auto& uuidComponent = world.ReadComponent<eng::UUIDComponent>(entity);
	return uuidComponent.m_UUID;
}

str::Guid eng::ToUUID(ecs::EntityWorld& world, const ecs::Entity& entity)
{
	if (entity.IsUnassigned())
		return {};
	if (!world.IsAlive(entity))
		return {};

	const auto& uuidComponent = world.ReadComponent<eng::UUIDComponent>(entity);
	return uuidComponent.m_UUID;
}