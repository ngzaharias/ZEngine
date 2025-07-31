#include "GameUIPCH.h"
#include "GameUI/GameMenuSystem.h"

#include "ECS/EntityWorld.h"
#include "ECS/NameComponent.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/ApplicationComponents.h"
#include "Engine/LevelComponents.h"
#include "Engine/SettingsComponents.h"
#include "Engine/UIManager.h"
#include "GameUI/DCGameMenu.h"
#include "GameUI/GameMenuComponents.h"

namespace
{
	const str::Name strGameMenu_xaml = NAME("GameMenu.xaml");
}

void gui::game_menu::MenuSystem::Update(World& world, const GameTime& gameTime)
{
	for (const ecs::Entity& entity : world.Query<ecs::query::Added<gui::game_menu::WindowComponent>>())
	{
		auto& uiManager = world.WriteResource<eng::UIManager>();
		uiManager.CreateWidget(strGameMenu_xaml, true);
	}

	for (const ecs::Entity& entity : world.Query<ecs::query::Removed<gui::game_menu::WindowComponent>>())
	{
		auto& uiManager = world.WriteResource<eng::UIManager>();
		uiManager.DestroyWidget(strGameMenu_xaml);
	}

	//////////////////////////////////////////////////////////////////////////
	// Events

	if (world.HasAny<ecs::query::Added<gui::game_menu::CloseRequest>>())
	{
		for (const ecs::Entity& entity : world.Query<ecs::query::Include<gui::game_menu::WindowComponent>>())
			world.DestroyEntity(entity);
	}

	if (world.HasAny<ecs::query::Added<gui::game_menu::ExitGameRequest>>())
	{
		world.AddEventComponent<eng::application::CloseRequestComponent>();
	}

	if (world.HasAny<ecs::query::Added<gui::game_menu::ExitToMenuRequest>>())
	{
		const auto& settings = world.ReadSingleton<eng::settings::LaunchComponent>();
		world.AddEventComponent<eng::level::LoadRequest>(settings.m_Level);
		world.AddEventComponent<gui::game_menu::CloseRequest>();
	}

	if (world.HasAny<ecs::query::Added<gui::game_menu::OpenRequest>>())
	{
		const ecs::Entity entity = world.CreateEntity();
		world.AddComponent<gui::game_menu::WindowComponent>(entity);
	}
}