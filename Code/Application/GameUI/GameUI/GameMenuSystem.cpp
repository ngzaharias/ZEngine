#include "GameUIPCH.h"
#include "GameUI/GameMenuSystem.h"

#include "ECS/EntityWorld.h"
#include "ECS/NameComponent.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/ApplicationComponents.h"
#include "Engine/InputManager.h"
#include "Engine/LevelComponents.h"
#include "Engine/SettingsComponents.h"
#include "Engine/UIManager.h"
#include "GameUI/DCGameMenu.h"
#include "GameUI/GameMenuComponents.h"

namespace
{
	const str::Name strGameMenu = NAME("GameMenu");
	const str::Name strGameMenu_xaml = NAME("GameMenu.xaml");
}

void gui::game_menu::MenuSystem::Update(World& world, const GameTime& gameTime)
{
	for (const ecs::Entity& entity : world.Query<ecs::query::Added<gui::game_menu::WindowComponent>>())
	{
		{
			auto& uiManager = world.WriteResource<eng::UIManager>();
			uiManager.CreateWidget(strGameMenu_xaml);
		}

		{
			input::Layer layer;
			layer.m_Priority = eng::EInputPriority::GameUI;
			layer.m_Consume.RaiseAll();

			auto& input = world.WriteResource<eng::InputManager>();
			input.AppendLayer(strGameMenu, layer);
		}
	}

	for (const ecs::Entity& entity : world.Query<ecs::query::Removed<gui::game_menu::WindowComponent>>())
	{
		auto& uiManager = world.WriteResource<eng::UIManager>();
		uiManager.DestroyWidget(strGameMenu_xaml);

		auto& input = world.WriteResource<eng::InputManager>();
		input.RemoveLayer(strGameMenu);
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
		world.AddEventComponent<eng::level::LoadRequestComponent>(settings.m_Level);
		world.AddEventComponent<gui::game_menu::CloseRequest>();
	}

	if (world.HasAny<ecs::query::Added<gui::game_menu::OpenRequest>>())
	{
		const ecs::Entity entity = world.CreateEntity();
		world.AddComponent<gui::game_menu::WindowComponent>(entity);
	}
}