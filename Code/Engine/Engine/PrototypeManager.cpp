#include "EnginePCH.h"
#include "Engine/PrototypeManager.h"

#include <Core/Assert.h>

#include <ECS/EntityWorld.h>
#include <ECS/NameComponent.h>

#include "Engine/AssetManager.h"
#include "Engine/Prototype.h"
#include "Engine/PrototypeLoader.h"
#include "Engine/TomlHelpers.h"
#include "Engine/TypeInfo.h"

namespace
{
	const str::Name strGuid = NAME("m_Guid");
	const str::Name strName = NAME("m_Name");
}

eng::PrototypeManager::PrototypeManager(eng::AssetManager& assetManager)
	: m_AssetManager(assetManager)
{
}

bool eng::PrototypeManager::CreateEntity(ecs::EntityWorld& world, const ecs::Entity& entity, const str::Path& filepath)
{
	PROFILE_FUNCTION();

	eng::Visitor visitor;
	visitor.LoadFromFile(filepath);

	str::Guid guid;
	visitor.Visit(strGuid, guid, {});
	if (!guid.IsValid())
		return false;

	str::Name name;
	visitor.Visit(strName, name, {});

	if (toml::Table* table = visitor.m_Root.as_table())
	{
		for (auto&& [key, value] : *table)
		{
			const str::Name type = NAME(key.str());
			if (!core::Contains(m_TypeMap, type))
				continue;

			const TypeId& typeId = m_TypeMap[type];
			auto& entry = m_EntryMap[typeId];
			auto& addFunc = entry.m_Add;
			auto& loadFunc = entry.m_Load;
			auto& newFunc = entry.m_New;

			auto* loader = entry.m_Loader;
			auto& cache = loader->m_Cache;

			Prototype* prototype = cache[guid];
			if (!prototype)
			{
				prototype = newFunc();
				prototype->m_Guid = guid;
				prototype->m_Name = name;
				prototype->m_Path = filepath;
				prototype->m_Type = entry.m_Type;

				Visitor child;
				child.JumpToNode(value);
				loadFunc(prototype, loader, child);
				cache[guid] = prototype;
			}

			addFunc(world, entity, prototype, loader);
		}
	}

	{
		auto& prototypeComponent = world.AddComponent<eng::PrototypeComponent>(entity);
		prototypeComponent.m_Guid = guid;
	}

	if (!name.IsEmpty())
	{
		auto& nameComponent = world.AddComponent<ecs::NameComponent>(entity);
		nameComponent.m_Name = name;
	}

	// #todo: add level component

	return true;
}