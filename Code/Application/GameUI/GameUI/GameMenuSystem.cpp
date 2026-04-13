#include "GameUIPCH.h"
#include "GameUI/GameMenuSystem.h"

#include "ECS/EntityWorld.h"
#include "ECS/NameComponent.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/ApplicationCloseEvent.h"
#include "Engine/InputManager.h"
#include "Engine/LevelLoadEvent.h"
#include "Engine/SettingsLaunchComponent.h"
#include "Engine/UIManager.h"
#include "GameUI/DCGameMenu.h"
#include "GameUI/GameMenuCloseEvent.h"
#include "GameUI/GameMenuExitGameEvent.h"
#include "GameUI/GameMenuExitToMenuEvent.h"
#include "GameUI/GameMenuOpenEvent.h"
#include "GameUI/GameMenuWindowComponent.h"

namespace
{
	const str::Name strInput = NAME("GameMenu");
	const str::Name strGameMenu_xaml = NAME("GameMenu.xaml");
}

void gui::game_menu::MenuSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	for (auto&& view : world.Query<ecs::query::Added<gui::game_menu::WindowComponent>>())
	{
		auto& uiManager = world.WriteResource<eng::UIManager>();
		uiManager.CreateWidget(strGameMenu_xaml, true);

		input::Layer layer;
		layer.m_Priority = eng::EInputPriority::Gameplay;
		layer.m_Consume.RaiseAll();

		auto& input = world.WriteResource<eng::InputManager>();
		input.AppendLayer(strInput, layer);
	}

	using RemovedQuery = ecs::query
		::Condition<ecs::Alive, ecs::Dead>
		::Removed<gui::game_menu::WindowComponent>;
	for (auto&& view : world.Query<RemovedQuery>())
	{
		auto& uiManager = world.WriteResource<eng::UIManager>();
		uiManager.DestroyWidget(strGameMenu_xaml);

		auto& input = world.WriteResource<eng::InputManager>();
		input.RemoveLayer(strInput);
	}

	//////////////////////////////////////////////////////////////////////////
	// Events

	if (world.HasAny<gui::game_menu::CloseEvent>())
	{
		for (auto&& view : world.Query<ecs::query::Include<gui::game_menu::WindowComponent>>())
			world.DestroyEntity(view);
	}

	if (world.HasAny<gui::game_menu::ExitGameEvent>())
	{
		world.AddEvent<eng::application::CloseEvent>();
	}

	if (world.HasAny<gui::game_menu::ExitToMenuEvent>())
	{
		const auto& settings = world.ReadComponent<eng::settings::LaunchComponent>();
		world.AddEvent<eng::level::LoadEvent>(settings.m_Level);
		world.AddEvent<gui::game_menu::CloseEvent>();
	}

	if (world.HasAny<gui::game_menu::OpenEvent>())
	{
		const ecs::Entity entity = world.CreateEntity();
		world.AddComponent<gui::game_menu::WindowComponent>(entity);
	}
}