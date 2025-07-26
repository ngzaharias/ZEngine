#include "GameUIPCH.h"
#include "GameUI/SettingsMenuSystem.h"

#include "Core/Name.h"
#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/InputManager.h"
#include "Engine/SettingsComponents.h"
#include "Engine/UIManager.h"
#include "GameUI/DCSettingsMenu.h"
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
			auto& dataContext = uiManager.WriteDataContext<gui::DCSettingsMenu>(strSettingsMenu_xaml);
			dataContext.Initialise(world);
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

	for (const ecs::Entity& entity : world.Query<ecs::query::Added<gui::settings_menu::ValueRequest>>())
	{
		const auto& eventData = world.ReadComponent<gui::settings_menu::ValueRequest>(entity);
		auto& settings = world.WriteSingleton<eng::settings::LocalComponent>();

		// audio
		if (eventData.m_EffectVolume)
			settings.m_Audio.m_EffectVolume = *eventData.m_EffectVolume;
		if (eventData.m_MasterVolume)
			settings.m_Audio.m_MasterVolume = *eventData.m_MasterVolume;
		if (eventData.m_MusicVolume)
			settings.m_Audio.m_MusicVolume = *eventData.m_MusicVolume;

		// gameplay
		if (eventData.m_MoveSpeed)
			settings.m_Camera.m_TranslateSpeed = *eventData.m_MoveSpeed;
		if (eventData.m_ZoomRate)
			settings.m_Camera.m_ZoomAmount = *eventData.m_ZoomRate;
		if (eventData.m_ZoomSpeed)
			settings.m_Camera.m_ZoomSpeed = *eventData.m_ZoomSpeed;

		// graphics
		if (eventData.m_Monitor)
			settings.m_Window.m_Monitor = *eventData.m_Monitor;
		if (eventData.m_RefreshRate)
			settings.m_Window.m_RefreshRate = *eventData.m_RefreshRate;
		if (eventData.m_Resolution)
			settings.m_Window.m_Resolution = *eventData.m_Resolution;
		if (eventData.m_WindowMode)
			settings.m_Window.m_WindowMode = *eventData.m_WindowMode;
	}
}
