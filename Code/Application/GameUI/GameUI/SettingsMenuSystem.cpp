#include "GameUIPCH.h"
#include "GameUI/SettingsMenuSystem.h"

#include "Core/Name.h"
#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/InputManager.h"
#include "Engine/UIManager.h"
#include "GameUI/SettingsMenuComponents.h"

namespace
{
	const str::Name strSettingsMenu = NAME("SettingsMenu");
	const str::Name strSettingsMenu_xaml = NAME("SettingsMenu.xaml");
}

void gui::settings_menu::MenuSystem::Update(World& world, const GameTime& gameTime)
{
	for (const ecs::Entity& entity : world.Query<ecs::query::Added<gui::settings_menu::WindowComponent>>())
	{
		{
			auto& uiManager = world.WriteResource<eng::UIManager>();
			uiManager.CreateWidget(strSettingsMenu_xaml);
		}

		{
			input::Layer layer;
			layer.m_Priority = eng::EInputPriority::GameUI;
			layer.m_Consume.RaiseAll();

			auto& input = world.WriteResource<eng::InputManager>();
			input.AppendLayer(strSettingsMenu, layer);
		}
	}

	for (const ecs::Entity& entity : world.Query<ecs::query::Removed<gui::settings_menu::WindowComponent>>())
	{
		auto& uiManager = world.WriteResource<eng::UIManager>();
		uiManager.DestroyWidget(strSettingsMenu_xaml);

		auto& input = world.WriteResource<eng::InputManager>();
		input.RemoveLayer(strSettingsMenu);
	}

	//////////////////////////////////////////////////////////////////////////
	// Events

	if (world.HasAny<ecs::query::Added<gui::settings_menu::CloseRequest>>())
	{
		for (const ecs::Entity& entity : world.Query<ecs::query::Include<gui::settings_menu::WindowComponent>>())
			world.DestroyEntity(entity);
	}

	if (world.HasAny<ecs::query::Added<gui::settings_menu::OpenRequest>>())
	{
		world.AddComponent<gui::settings_menu::WindowComponent>(world.CreateEntity());
	}
}
