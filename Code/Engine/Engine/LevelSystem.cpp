#include "EnginePCH.h"
#include "Engine/LevelSystem.h"

#include <Core/Array.h>
#include <Core/Name.h>
#include <Core/Path.h>

#include <ECS/EntityWorld.h>
#include <ECS/NameComponent.h>
#include <ECS/QueryTypes.h>
#include <ECS/WorldView.h>

#include "Engine/LevelComponents.h"
#include "Engine/PrototypeManager.h"

#include <filesystem>

namespace
{
	const str::Name strLevels = NAME("Levels");
}

eng::LevelSystem::LevelSystem(ecs::EntityWorld& entityWorld)
	: m_EntityWorld(entityWorld)
{
}

void eng::LevelSystem::Initialise(World& world)
{
	auto& directoryComponent = world.AddSingleton<eng::LevelDirectoryComponent>();

	str::Path subpath;
	const str::Path path = str::Path(str::EPath::Assets, strLevels.ToChar());
	for (const auto& entry : std::filesystem::directory_iterator(path.ToChar()))
	{
		subpath = std::move(entry.path().string());
		if (path.IsDirectory())
		{
			const str::Name name = NAME(subpath.GetStem());
			directoryComponent.m_Levels.Emplace(name, subpath);
		}
	}
}

void eng::LevelSystem::Shutdown(World& world)
{
	world.RemoveSingleton<eng::LevelDirectoryComponent>();
}

void eng::LevelSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	const auto& directoryComponent = world.GetSingleton<eng::LevelDirectoryComponent>();

	// load requests
	{
		for (const ecs::Entity& requestEntity : world.Query<ecs::query::Include<const eng::LevelLoadRequestComponent>>())
		{
			const auto& requestComponent = world.GetComponent<const eng::LevelLoadRequestComponent>(requestEntity);

			// unload current levels
			for (const ecs::Entity& loadedEntity : world.Query<ecs::query::Include<const eng::LevelLoadedComponent>>())
			{
				const auto& loadedComponent = world.GetComponent<const eng::LevelLoadedComponent>(loadedEntity);
				UnloadLevel(world, loadedComponent.m_Name);
			}

			// load requested level
			if (core::Contains(directoryComponent.m_Levels, requestComponent.m_Name))
			{
				const str::Path& directory = directoryComponent.m_Levels.Get(requestComponent.m_Name);
				LoadLevel(world, requestComponent.m_Name, directory);
			}
		}
	}

	// unload requests
	{
		for (const ecs::Entity& requestEntity : world.Query<ecs::query::Include<const eng::LevelUnloadRequestComponent>>())
		{
			const auto& requestComponent = world.GetComponent<const eng::LevelUnloadRequestComponent>(requestEntity);

			for (const ecs::Entity& loadedEntity : world.Query<ecs::query::Include<const eng::LevelLoadedComponent>>())
			{
				const auto& loadedComponent = world.GetComponent<const eng::LevelLoadedComponent>(requestEntity);
				if (requestComponent.m_Name == loadedComponent.m_Name)
					UnloadLevel(world, loadedComponent.m_Name);
			}
		}
	}
}

void eng::LevelSystem::LoadLevel(World& world, const str::Name& name, const str::Path& directory)
{
	constexpr const char* s_Extension = ".prototype";

	auto& prototypeManager = world.GetResource<eng::PrototypeManager>();

	{
		const ecs::Entity entity = world.CreateEntity();
		auto& levelComponent = world.AddComponent<eng::LevelLoadedComponent>(entity);
		levelComponent.m_Name = name;

		auto& nameComponent = world.AddComponent<ecs::NameComponent>(entity);
		nameComponent.m_Name = "Level: " + name.ToString();
	}

	Array<str::Path> directories;
	directories.Append(directory);

	while (!directories.IsEmpty())
	{
		const str::Path path = directories.Pop();
		if (path.IsDirectory())
		{
			str::Path subpath;
			std::filesystem::path directory = path.ToChar();
			for (const auto& entry : std::filesystem::directory_iterator(directory))
			{
				subpath = entry.path().string();
				directories.Append(subpath);
			}
		}
		else if (path.GetFileExtension() == s_Extension)
		{
			const ecs::Entity entity = world.CreateEntity();
			auto& levelComponent = world.AddComponent<eng::LevelEntityComponent>(entity);
			levelComponent.m_Name = name;

			prototypeManager.CreateEntity(m_EntityWorld, entity, path);
		}
	}
}

void eng::LevelSystem::UnloadLevel(World& world, const str::Name& name)
{
	for (const ecs::Entity& entity : world.Query<ecs::query::Include<const eng::LevelEntityComponent>>())
	{
		const auto& component = world.GetComponent<const eng::LevelEntityComponent>(entity);
		if (component.m_Name == name)
			world.DestroyEntity(entity);
	}

	for (const ecs::Entity& entity : world.Query<ecs::query::Include<const eng::LevelLoadedComponent>>())
	{
		const auto& component = world.GetComponent<const eng::LevelLoadedComponent>(entity);
		if (component.m_Name == name)
			world.DestroyEntity(entity);
	}
}