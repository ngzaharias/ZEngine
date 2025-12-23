#include "GameUIPCH.h"
#include "GameUI/SettingsMenuSystem.h"

#include "Core/Name.h"
#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/SettingsAudioSingleton.h"
#include "Engine/SettingsCameraSingleton.h"
#include "Engine/SettingsGameplaySingleton.h"
#include "Engine/SettingsLaunchSingleton.h"
#include "Engine/SettingsWindowSingleton.h"
#include "Engine/UIManager.h"
#include "GameUI/DCSettingsMenu.h"
#include "GameUI/SettingsMenuCloseRequest.h"
#include "GameUI/SettingsMenuOpenRequest.h"
#include "GameUI/SettingsMenuValueRequest.h"
#include "GameUI/SettingsMenuWindowComponent.h"
#include "Hidden/HiddenDebugSettingsSingleton.h"

namespace
{
	const str::Name strSettingsMenu_xaml = NAME("SettingsMenu.xaml");
}

void gui::settings_menu::MenuSystem::Update(World& world, const GameTime& gameTime)
{
	for (auto&& view : world.Query<ecs::query::Added<gui::settings_menu::WindowComponent>>())
	{
		gui::DCSettingsMenu::World other = world;

		auto& uiManager = world.WriteResource<eng::UIManager>();
		uiManager.CreateWidget(strSettingsMenu_xaml, true);
		auto& dataContext = uiManager.WriteDataContext<gui::DCSettingsMenu>(strSettingsMenu_xaml);
		dataContext.Initialise(other);
	}

	for (auto&& view : world.Query<ecs::query::Removed<gui::settings_menu::WindowComponent>>())
	{
		auto& uiManager = world.WriteResource<eng::UIManager>();
		uiManager.DestroyWidget(strSettingsMenu_xaml);
	}

	//////////////////////////////////////////////////////////////////////////
	// Events

	if (world.HasAny<gui::settings_menu::CloseRequest>())
	{
		for (auto&& view : world.Query<ecs::query::Include<gui::settings_menu::WindowComponent>>())
			world.DestroyEntity(view);
	}

	if (world.HasAny<gui::settings_menu::OpenRequest>())
	{
		world.AddComponent<gui::settings_menu::WindowComponent>(world.CreateEntity());
	}

	for (const auto& eventData : world.Events<gui::settings_menu::ValueRequest>())
	{
		// audio
		if (eventData.m_EffectVolume)
			world.WriteSingleton<eng::settings::AudioSingleton>().m_EffectVolume = *eventData.m_EffectVolume;
		if (eventData.m_MasterVolume)
			world.WriteSingleton<eng::settings::AudioSingleton>().m_MasterVolume = *eventData.m_MasterVolume;
		if (eventData.m_MusicVolume)
			world.WriteSingleton<eng::settings::AudioSingleton>().m_MusicVolume = *eventData.m_MusicVolume;

		// gameplay
		if (eventData.m_MoveSpeed)
			world.WriteSingleton<eng::settings::CameraSingleton>().m_TranslateSpeed = *eventData.m_MoveSpeed;
		if (eventData.m_ZoomRate)
			world.WriteSingleton<eng::settings::CameraSingleton>().m_ZoomAmount = *eventData.m_ZoomRate;
		if (eventData.m_ZoomSpeed)
			world.WriteSingleton<eng::settings::CameraSingleton>().m_ZoomSpeed = *eventData.m_ZoomSpeed;
		if (eventData.m_Theme)
			world.WriteSingleton<eng::settings::GameplaySingleton>().m_Theme = *eventData.m_Theme;

		// graphics
		if (eventData.m_Monitor)
			world.WriteSingleton<eng::settings::WindowSingleton>().m_Monitor = *eventData.m_Monitor;
		if (eventData.m_RefreshRate)
			world.WriteSingleton<eng::settings::WindowSingleton>().m_RefreshRate = *eventData.m_RefreshRate;
		if (eventData.m_Resolution)
			world.WriteSingleton<eng::settings::WindowSingleton>().m_Resolution = *eventData.m_Resolution;
		if (eventData.m_WindowMode)
			world.WriteSingleton<eng::settings::WindowSingleton>().m_WindowMode = *eventData.m_WindowMode;
	}
}
