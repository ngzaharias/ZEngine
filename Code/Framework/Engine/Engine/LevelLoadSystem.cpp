#include "EnginePCH.h"
#include "Engine/LevelLoadSystem.h"

#include "Core/Array.h"
#include "Core/GameTime.h"
#include "Core/Name.h"
#include "Core/Path.h"
#include "ECS/EntityWorld.h"
#include "ECS/NameComponent.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/LevelDirectorySingleton.h"
#include "Engine/LevelEntityComponent.h"
#include "Engine/LevelLoadedComponent.h"
#include "Engine/LevelLoadingComponent.h"
#include "Engine/LevelLoadRequest.h"
#include "Engine/PrototypeManager.h"

#include <filesystem>

namespace
{
	constexpr float s_FadeOutTime = 6.f;

	const str::Name strLevels = NAME("Levels");
}

eng::level::LoadSystem::LoadSystem(ecs::EntityWorld& entityWorld)
	: m_EntityWorld(entityWorld)
{
}

void eng::level::LoadSystem::Initialise(World& world)
{
	auto& directoryComponent = world.WriteSingleton<eng::level::DirectorySingleton>();

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

	const auto& directoryComponent = world.ReadSingleton<eng::level::DirectorySingleton>();

	// requests
	for (const auto& request : world.Events<eng::level::LoadRequest>())
	{
		const ecs::Entity levelEntity = world.CreateEntity();
		auto& loadingComponent = world.AddComponent<eng::level::LoadingComponent>(levelEntity);
		loadingComponent.m_Name = request.m_Name;
		loadingComponent.m_IsSplash = request.m_IsSplash;
		if (request.m_IsSplash)
			loadingComponent.m_FadeOutTime = s_FadeOutTime;
	}

	// loading
	for (auto&& loadingView : world.Query<ecs::query::Include<const eng::level::LoadingComponent>>())
	{
		auto& loadingComponent = loadingView.WriteRequired<eng::level::LoadingComponent>();
		loadingComponent.m_StatePrevious = loadingComponent.m_StateCurrent;

		switch (loadingComponent.m_StateCurrent)
		{
		case eng::level::ELoadingState::FadeOut:
		{
			loadingComponent.m_FadeOutTimer += gameTime.m_DeltaTime;
			if (loadingComponent.m_FadeOutTimer > loadingComponent.m_FadeOutTime)
				loadingComponent.m_StateCurrent = eng::level::ELoadingState::Unload;
		} break;

		case eng::level::ELoadingState::Unload:
		{
			for (auto&& loadedView : world.Query<ecs::query::Include<const eng::level::LoadedComponent>>())
			{
				const auto& loadedComponent = loadedView.ReadRequired<eng::level::LoadedComponent>();
				UnloadLevel(world, loadedComponent.m_Name);
			}
			loadingComponent.m_StateCurrent = eng::level::ELoadingState::Load;
		} break;

		case eng::level::ELoadingState::Load:
		{
			if (directoryComponent.m_Levels.Contains(loadingComponent.m_Name))
			{
				const str::Path& directory = directoryComponent.m_Levels.Get(loadingComponent.m_Name);
				LoadLevel(world, loadingComponent.m_Name, directory);
			}
			loadingComponent.m_StateCurrent = eng::level::ELoadingState::FadeIn;
		} break;

		case eng::level::ELoadingState::FadeIn:
		{
			loadingComponent.m_FadeInTimer += gameTime.m_DeltaTime;
			if (loadingComponent.m_FadeInTimer > loadingComponent.m_FadeInTime)
				world.DestroyEntity(loadingView);
		} break;
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
	for (auto&& view : world.Query<ecs::query::Include<const eng::level::EntityComponent>>())
	{
		const auto& component = view.ReadRequired<eng::level::EntityComponent>();
		if (component.m_Name == levelName)
			world.DestroyEntity(view);
	}

	for (auto&& view : world.Query<ecs::query::Include<const eng::level::LoadedComponent>>())
	{
		const auto& component = view.ReadRequired<eng::level::LoadedComponent>();
		if (component.m_Name == levelName)
			world.DestroyEntity(view);
	}
}