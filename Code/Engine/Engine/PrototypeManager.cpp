#include "EnginePCH.h"
#include "Engine/PrototypeManager.h"

#include "Core/Assert.h"
#include "ECS/EntityWorld.h"
#include "ECS/NameComponent.h"
#include "Engine/PrototypeLoader.h"
#include "Engine/Visitor.h"

namespace
{
	const str::StringView strGuid = "m_Guid";
	const str::StringView strName = "m_Name";
}

eng::PrototypeManager::PrototypeManager()
{
}

eng::PrototypeManager::~PrototypeManager()
{
	for (auto&& [key, value] : m_EntryMap)
		delete value.m_Loader;
}

ecs::Entity eng::PrototypeManager::CreateEntity(ecs::EntityWorld& world, const str::Path& filepath)
{
	PROFILE_FUNCTION();

	eng::Visitor visitor;
	visitor.LoadFromFile(filepath);

	str::Guid guid;
	visitor.Visit(strGuid, guid, {});
	
	str::Name name;
	visitor.Visit(strName, name, {});

	if (!guid.IsValid())
		return ecs::Entity::Unassigned;

	const ecs::Entity entity = world.CreateEntity();
	for (str::StringView key : visitor)
	{
		const str::Name typeName = NAME(key);
		if (!m_EntryMap.Contains(typeName))
			continue;

		const eng::PrototypeEntry& entry = m_EntryMap.Get(typeName);
		entry.m_Create(visitor, world, entity, *entry.m_Loader);
	}

	auto& nameComponent = world.AddComponent<ecs::NameComponent>(entity);
	nameComponent.m_Name = name;

	auto& prototypeComponent = world.AddComponent<eng::PrototypeComponent>(entity);
	prototypeComponent.m_Guid = guid;
	prototypeComponent.m_Path = filepath;

	return entity;
}