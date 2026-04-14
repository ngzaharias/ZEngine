#include "EnginePCH.h"
#include "Engine/TemplateManager.h"

#include "ECS/EntityWorld.h"
#include "ECS/NameComponent.h"
#include "Engine/UUIDComponent.h"

namespace
{
	const str::StringView strName = "m_Name";
	const str::StringView strUUID = "m_UUID";
}

void eng::TemplateManager::ReadEntity(ecs::EntityWorld& world, const ecs::Entity& entity, Visitor& visitor) const
{
	const auto& nameComponent = world.ReadComponent<ecs::NameComponent>(entity);
	const auto& uuidComponent = world.ReadComponent<eng::UUIDComponent>(entity);

	visitor.Write(strName, nameComponent.m_Name);
	visitor.Write(strUUID, uuidComponent.m_UUID);
	for (auto&& [key, entry] : m_EntryMap)
	{
		entry.m_Read(world, entity, visitor);
	}
}

void eng::TemplateManager::WriteEntity(ecs::EntityWorld& world, const ecs::Entity& entity, Visitor& visitor) const
{
	PROFILE_FUNCTION();

	auto& nameComponent = world.HasComponent<ecs::NameComponent>(entity)
		? world.WriteComponent<ecs::NameComponent>(entity)
		: world.AddComponent<ecs::NameComponent>(entity);
	auto& uuidComponent = world.HasComponent<eng::UUIDComponent>(entity)
		? world.WriteComponent<eng::UUIDComponent>(entity)
		: world.AddComponent<eng::UUIDComponent>(entity);

	visitor.Read(strName, nameComponent.m_Name, {});
	visitor.Read(strUUID, uuidComponent.m_UUID, {});
	for (str::StringView key : visitor)
	{
		const str::Name typeName = NAME(key);
		if (!m_EntryMap.Contains(typeName))
			continue;

		const eng::TemplateEntry& entry = m_EntryMap.Get(typeName);
		entry.m_Write(world, entity, visitor);
	}
}