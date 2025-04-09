#include "EnginePCH.h"
#include "Engine/LevelLoadSystem.h"

#include "Core/Array.h"
#include "Core/Name.h"
#include "Core/Path.h"
#include "ECS/EntityWorld.h"
#include "ECS/NameComponent.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/LevelComponents.h"
#include "Engine/PrototypeManager.h"

#include <filesystem>

namespace
{
	const str::Name strLevels = NAME("Levels");
}

eng::level::LoadSystem::LoadSystem(ecs::EntityWorld& entityWorld)
	: m_EntityWorld(entityWorld)
{
}

void eng::level::LoadSystem::Initialise(World& world)
{
	auto& directoryComponent = world.WriteSingleton<eng::level::DirectoryComponent>();

	str::Path subpath;
	const str::Path path = str::Path(str::EPath::Levels);
	for (const auto& entry : std::filesystem::directory_iterator(path.ToChar()))
	{
		subpath = std::move(entry.path().string());
		if (subpath.IsDirectory())
		{
			const str::Name name = NAME(subpath.GetStem());
			directoryComponent.m_Levels.Emplace(name, subpath);
		}
	}
}

void eng::level::LoadSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	const auto& directoryComponent = world.ReadSingleton<eng::level::DirectoryComponent>();

	// load requests
	{
		for (const ecs::Entity& requestEntity : world.Query<ecs::query::Include<const eng::level::LoadRequestComponent>>())
		{
			const auto& requestComponent = world.ReadComponent<eng::level::LoadRequestComponent>(requestEntity);

			// unload current levels
			for (const ecs::Entity& loadedEntity : world.Query<ecs::query::Include<const eng::level::LoadedComponent>>())
			{
				const auto& loadedComponent = world.ReadComponent<eng::level::LoadedComponent>(loadedEntity);
				UnloadLevel(world, loadedComponent.m_Name);
			}

			// load requested level
			if (directoryComponent.m_Levels.Contains(requestComponent.m_Name))
			{
				const str::Path& directory = directoryComponent.m_Levels.Get(requestComponent.m_Name);
				LoadLevel(world, requestComponent.m_Name, directory);
			}
		}
	}

	// unload requests
	{
		for (const ecs::Entity& requestEntity : world.Query<ecs::query::Include<const eng::level::UnloadRequestComponent>>())
		{
			const auto& requestComponent = world.ReadComponent<eng::level::UnloadRequestComponent>(requestEntity);

			for (const ecs::Entity& loadedEntity : world.Query<ecs::query::Include<const eng::level::LoadedComponent>>())
			{
				const auto& loadedComponent = world.ReadComponent<eng::level::LoadedComponent>(loadedEntity);
				if (requestComponent.m_Name == loadedComponent.m_Name)
					UnloadLevel(world, loadedComponent.m_Name);
			}
		}
	}
}

void eng::level::LoadSystem::LoadLevel(World& world, const str::Name& levelName, const str::Path& directory)
{
	constexpr const char* s_Extension = ".prototype";

	const auto& prototypeManager = world.ReadResource<eng::PrototypeManager>();

	{
		const ecs::Entity levelEntity = world.CreateEntity();
		auto& levelComponent = world.AddComponent<eng::level::LoadedComponent>(levelEntity);
		levelComponent.m_Name = levelName;
		levelComponent.m_Path = directory;

		auto& nameComponent = world.AddComponent<ecs::NameComponent>(levelEntity);
		nameComponent.m_Name = "Level: " + levelName.ToString();
	}

	Array<str::Path> searchPaths;
	searchPaths.Append(directory);

	while (!searchPaths.IsEmpty())
	{
		const str::Path filepath = searchPaths.Pop();
		if (filepath.IsDirectory())
		{
			str::Path folderPath;
			for (const auto& entry : std::filesystem::directory_iterator(filepath.ToChar()))
			{
				folderPath = entry.path().string();
				searchPaths.Append(folderPath);
			}
		}
		else if (filepath.GetFileExtension() == s_Extension)
		{
			const ecs::Entity entity = world.CreateEntity();
			world.AddComponent<eng::level::EntityComponent>(entity, levelName);
			prototypeManager.LoadEntity(m_EntityWorld, entity, filepath);
		}
	}
}

void eng::level::LoadSystem::UnloadLevel(World& world, const str::Name& levelName)
{
	for (const ecs::Entity& entity : world.Query<ecs::query::Include<const eng::level::EntityComponent>>())
	{
		const auto& component = world.ReadComponent<eng::level::EntityComponent>(entity);
		if (component.m_Name == levelName)
			world.DestroyEntity(entity);
	}

	for (const ecs::Entity& entity : world.Query<ecs::query::Include<const eng::level::LoadedComponent>>())
	{
		const auto& component = world.ReadComponent<eng::level::LoadedComponent>(entity);
		if (component.m_Name == levelName)
			world.DestroyEntity(entity);
	}
}