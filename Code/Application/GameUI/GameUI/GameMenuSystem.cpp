#include "GameUIPCH.h"
#include "GameUI/GameMenuSystem.h"

#include "ECS/EntityWorld.h"
#include "ECS/NameComponent.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/ApplicationCloseRequestEvent.h"
#include "Engine/InputManager.h"
#include "Engine/LevelLoadRequestEvent.h"
#include "Engine/SettingsLaunchSingleton.h"
#include "Engine/UIManager.h"
#include "GameUI/DCGameMenu.h"
#include "GameUI/GameMenuComponents.h"

namespace
{
	const str::Name strInput = NAME("GameMenu");
	const str::Name strGameMenu_xaml = NAME("GameMenu.xaml");
}

void gui::game_menu::MenuSystem::Update(World& world, const GameTime& gameTime)
{
	for (const ecs::Entity& entity : world.Query<ecs::query::Added<gui::game_menu::WindowComponent>>())
	{
		auto& uiManager = world.WriteResource<eng::UIManager>();
		uiManager.CreateWidget(strGameMenu_xaml, true);

		input::Layer layer;
		layer.m_Priority = eng::EInputPriority::Gameplay;
		layer.m_Consume.RaiseAll();

		auto& input = world.WriteResource<eng::InputManager>();
		input.AppendLayer(strInput, layer);
	}

	for (const ecs::Entity& entity : world.Query<ecs::query::Removed<gui::game_menu::WindowComponent>>())
	{
		auto& uiManager = world.WriteResource<eng::UIManager>();
		uiManager.DestroyWidget(strGameMenu_xaml);

		auto& input = world.WriteResource<eng::InputManager>();
		input.RemoveLayer(strInput);
	}

	//////////////////////////////////////////////////////////////////////////
	// Events

	if (world.HasAny<gui::game_menu::CloseRequest>())
	{
		for (const ecs::Entity& entity : world.Query<ecs::query::Include<gui::game_menu::WindowComponent>>())
			world.DestroyEntity(entity);
	}

	if (world.HasAny<gui::game_menu::ExitGameRequest>())
	{
		world.AddEvent<eng::application::CloseRequestEvent>();
	}

	if (world.HasAny<gui::game_menu::ExitToMenuRequest>())
	{
		const auto& settings = world.ReadSingleton<eng::settings::LaunchSingleton>();
		world.AddEvent<eng::level::LoadRequestEvent>(settings.m_Level);
		world.AddEvent<gui::game_menu::CloseRequest>();
	}

	if (world.HasAny<gui::game_menu::OpenRequest>())
	{
		const ecs::Entity entity = world.CreateEntity();
		world.AddComponent<gui::game_menu::WindowComponent>(entity);
	}
}