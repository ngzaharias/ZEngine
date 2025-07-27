#include "GameUIPCH.h"
#include "GameUI/LevelCompleteSystem.h"

#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/ApplicationComponents.h"
#include "Engine/InputManager.h"
#include "Engine/LevelComponents.h"
#include "Engine/SettingsComponents.h"
#include "Engine/UIManager.h"
#include "GameUI/DCLevelComplete.h"
#include "GameUI/LevelCompleteComponents.h"

namespace
{
	const str::Name strLevelComplete = NAME("LevelComplete");
	const str::Name strLevelComplete_xaml = NAME("LevelComplete.xaml");
}

void gui::level_complete::MenuSystem::Update(World& world, const GameTime& gameTime)
{
	for (const ecs::Entity& entity : world.Query<ecs::query::Added<gui::level_complete::WindowComponent>>())
	{
		auto& uiManager = world.WriteResource<eng::UIManager>();
		uiManager.CreateWidget(strLevelComplete_xaml);

		input::Layer layer;
		layer.m_Priority = eng::EInputPriority::GameUI;
		layer.m_Consume.RaiseAll();

		auto& input = world.WriteResource<eng::InputManager>();
		input.AppendLayer(strLevelComplete, layer);
	}

	for (const ecs::Entity& entity : world.Query<ecs::query::Removed<gui::level_complete::WindowComponent>>())
	{
		auto& uiManager = world.WriteResource<eng::UIManager>();
		uiManager.DestroyWidget(strLevelComplete_xaml);

		auto& input = world.WriteResource<eng::InputManager>();
		input.RemoveLayer(strLevelComplete);
	}

	//////////////////////////////////////////////////////////////////////////
	// Events

	if (world.HasAny<ecs::query::Added<gui::level_complete::ExitGameRequest>>())
	{
		world.AddEventComponent<eng::application::CloseRequestComponent>();
	}

	if (world.HasAny<ecs::query::Added<gui::level_complete::ExitToMenuRequest>>())
	{
		const auto& settings = world.ReadSingleton<eng::settings::LaunchComponent>();
		world.AddEventComponent<eng::level::LoadRequestComponent>(settings.m_Level);

		for (const ecs::Entity& entity : world.Query<ecs::query::Include<gui::level_complete::WindowComponent>>())
			world.DestroyEntity(entity);
	}
}