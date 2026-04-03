#include "EnginePCH.h"
#include "Engine/TemplateManager.h"

#include "ECS/EntityWorld.h"
#include "ECS/NameComponent.h"

namespace
{
	const str::StringView strGuid = "m_Guid";
	const str::StringView strName = "m_Name";
}

void eng::TemplateManager::LoadEntity(ecs::EntityWorld& world, const ecs::Entity& entity, const str::String& data) const
{
	PROFILE_FUNCTION();

	auto& nameComponent = world.AddComponent<ecs::NameComponent>(entity);
	auto& templateComponent = world.AddComponent<eng::TemplateComponent>(entity);

	Visitor visitor(data);
	visitor.Read(strName, nameComponent.m_Name, {});
	visitor.Read(strGuid, templateComponent.m_Guid, {});
	for (str::StringView key : visitor)
	{
		const str::Name typeName = NAME(key);
		if (!m_EntryMap.Contains(typeName))
			continue;

		const eng::TemplateEntry& entry = m_EntryMap.Get(typeName);
		entry.m_Load(world, entity, visitor);
	}
}

void eng::TemplateManager::ReadEntity(ecs::EntityWorld& world, const ecs::Entity& entity, str::String& data) const
{
	const auto& nameComponent = world.ReadComponent<ecs::NameComponent>(entity);
	const auto& templateComponent = world.ReadComponent<eng::TemplateComponent>(entity);

	Visitor visitor;
	visitor.Write(strName, nameComponent.m_Name);
	visitor.Write(strGuid, templateComponent.m_Guid);
	for (auto&& [key, entry] : m_EntryMap)
	{
		entry.m_Read(world, entity, visitor);
	}

	data = visitor;
}

void eng::TemplateManager::SyncEntity(ecs::EntityWorld& world, const ecs::Entity& entity, const str::String& data) const
{
	PROFILE_FUNCTION();

	auto& nameComponent = !world.HasComponent<ecs::NameComponent>(entity)
		? world.AddComponent<ecs::NameComponent>(entity)
		: world.WriteComponent<ecs::NameComponent>(entity);
	auto& templateComponent = !world.HasComponent<eng::TemplateComponent>(entity)
		? world.AddComponent<eng::TemplateComponent>(entity)
		: world.WriteComponent<eng::TemplateComponent>(entity);

	Visitor visitor(data);
	visitor.Read(strName, nameComponent.m_Name, {});
	visitor.Read(strGuid, templateComponent.m_Guid, {});
	for (auto&& [key, entry] : m_EntryMap)
	{
		entry.m_Sync(world, entity, visitor);
	}
}

bool eng::TemplateManager::InspectEntity(ecs::EntityWorld& world, const ecs::Entity& entity, imgui::Inspector& insepctor) const
{
	bool modified = false;
	for (auto&& [name, entry] : m_EntryMap)
		modified |= entry.m_Inspect(world, entity, insepctor);
	return modified;
}