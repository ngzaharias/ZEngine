#include "GameUIPCH.h"
#include "GameUI/LevelCompleteSystem.h"

#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/ApplicationComponents.h"
#include "Engine/LevelComponents.h"
#include "Engine/SettingsLaunchSingleton.h"
#include "Engine/UIManager.h"
#include "GameUI/DCLevelComplete.h"
#include "GameUI/LevelCompleteComponents.h"

namespace
{
	const str::Name strLevelComplete_xaml = NAME("LevelComplete.xaml");

	const str::StringView strFilename = "Savegame.toml";
}

void gui::level_complete::MenuSystem::Update(World& world, const GameTime& gameTime)
{
	for (const ecs::Entity& entity : world.Query<ecs::query::Added<gui::level_complete::WindowComponent>>())
	{
		auto& uiManager = world.WriteResource<eng::UIManager>();
		uiManager.CreateWidget(strLevelComplete_xaml, true);
	}

	for (const ecs::Entity& entity : world.Query<ecs::query::Removed<gui::level_complete::WindowComponent>>())
	{
		auto& uiManager = world.WriteResource<eng::UIManager>();
		uiManager.DestroyWidget(strLevelComplete_xaml);
	}

	//////////////////////////////////////////////////////////////////////////
	// Events

	if (world.HasAny<gui::level_complete::ExitGameRequest>())
	{
		world.AddEvent<eng::application::CloseRequest>();
	}

	if (world.HasAny<gui::level_complete::ExitToMenuRequest>())
	{
		const auto& settings = world.ReadSingleton<eng::settings::LaunchSingleton>();
		world.AddEvent<eng::level::LoadRequest>(settings.m_Level);

		for (const ecs::Entity& entity : world.Query<ecs::query::Include<gui::level_complete::WindowComponent>>())
			world.DestroyEntity(entity);
	}

	if (world.HasAny<gui::level_complete::ResetGameRequest>())
	{
		const str::Path filepath = str::Path(str::EPath::AppData, strFilename);
		std::remove(filepath.ToChar());

		for (const ecs::Entity& entity : world.Query<ecs::query::Include<const eng::level::LoadedComponent>>())
		{
			const auto& levelComponent = world.ReadComponent<eng::level::LoadedComponent>(entity);
			world.AddEvent<eng::level::LoadRequest>(levelComponent.m_Name);
		}

		for (const ecs::Entity& entity : world.Query<ecs::query::Include<gui::level_complete::WindowComponent>>())
			world.DestroyEntity(entity);
	}
}