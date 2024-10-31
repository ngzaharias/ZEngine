#include "EnginePCH.h"
#include "Engine/PrototypeManager.h"

#include "Core/Assert.h"
#include "ECS/EntityWorld.h"
#include "ECS/NameComponent.h"
#include "Engine/Visitor.h"

namespace
{
	const str::StringView strGuid = "m_Guid";
	const str::StringView strName = "m_Name";
}

bool eng::PrototypeManager::LoadEntity(ecs::EntityWorld& world, const ecs::Entity& entity, const str::Path& filepath) const
{
	PROFILE_FUNCTION();

	eng::Visitor visitor;
	if (!visitor.LoadFromFile(filepath))
		return false;

	str::Guid guid;
	visitor.Read(strGuid, guid, {});
	str::Name name;
	visitor.Read(strName, name, {});

	if (!guid.IsValid())
		return false;

	auto& nameComponent = world.AddComponent<ecs::NameComponent>(entity);
	nameComponent.m_Name = name;

	auto& prototypeComponent = world.AddComponent<eng::PrototypeComponent>(entity);
	prototypeComponent.m_Guid = guid;
	prototypeComponent.m_Path = filepath;

	for (str::StringView key : visitor)
	{
		const str::Name typeName = NAME(key);
		if (!m_EntryMap.Contains(typeName))
			continue;

		const eng::PrototypeEntry& entry = m_EntryMap.Get(typeName);
		entry.m_Load(world, entity, visitor);
	}

	return true;
}