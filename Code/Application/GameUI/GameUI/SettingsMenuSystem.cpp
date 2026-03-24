#include "GameUIPCH.h"
#include "GameUI/SettingsMenuSystem.h"

#include "Camera/CameraSettingsStaticComponent.h"
#include "ClientHidden/HiddenDebugStaticComponent.h"
#include "Core/Name.h"
#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/SettingsAudioStaticComponent.h"
#include "Engine/SettingsGameplayStaticComponent.h"
#include "Engine/SettingsLaunchStaticComponent.h"
#include "Engine/SettingsWindowStaticComponent.h"
#include "Engine/UIManager.h"
#include "GameUI/DCSettingsMenu.h"
#include "GameUI/SettingsMenuCloseEvent.h"
#include "GameUI/SettingsMenuOpenEvent.h"
#include "GameUI/SettingsMenuValueEvent.h"
#include "GameUI/SettingsMenuWindowComponent.h"

namespace
{
	const str::Name strSettingsMenu_xaml = NAME("SettingsMenu.xaml");
}

void gui::settings_menu::MenuSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

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

	if (world.HasAny<gui::settings_menu::CloseEvent>())
	{
		for (auto&& view : world.Query<ecs::query::Include<gui::settings_menu::WindowComponent>>())
			world.DestroyEntity(view);
	}

	if (world.HasAny<gui::settings_menu::OpenEvent>())
	{
		world.AddComponent<gui::settings_menu::WindowComponent>(world.CreateEntity());
	}

	for (const auto& eventData : world.Events<gui::settings_menu::ValueEvent>())
	{
		// audio
		if (eventData.m_EffectVolume)
			world.WriteComponent<eng::settings::AudioStaticComponent>().m_EffectVolume = *eventData.m_EffectVolume;
		if (eventData.m_MasterVolume)
			world.WriteComponent<eng::settings::AudioStaticComponent>().m_MasterVolume = *eventData.m_MasterVolume;
		if (eventData.m_MusicVolume)
			world.WriteComponent<eng::settings::AudioStaticComponent>().m_MusicVolume = *eventData.m_MusicVolume;

		// gameplay
		if (eventData.m_MoveSpeed)
			world.WriteComponent<camera::SettingsStaticComponent>().m_TranslateSpeed = *eventData.m_MoveSpeed;
		if (eventData.m_ZoomRate)
			world.WriteComponent<camera::SettingsStaticComponent>().m_ZoomAmount = *eventData.m_ZoomRate;
		if (eventData.m_ZoomSpeed)
			world.WriteComponent<camera::SettingsStaticComponent>().m_ZoomSpeed = *eventData.m_ZoomSpeed;
		if (eventData.m_Theme)
			world.WriteComponent<eng::settings::GameplayStaticComponent>().m_Theme = *eventData.m_Theme;

		// graphics
		if (eventData.m_Monitor)
			world.WriteComponent<eng::settings::WindowStaticComponent>().m_Monitor = *eventData.m_Monitor;
		if (eventData.m_RefreshRate)
			world.WriteComponent<eng::settings::WindowStaticComponent>().m_RefreshRate = *eventData.m_RefreshRate;
		if (eventData.m_Resolution)
			world.WriteComponent<eng::settings::WindowStaticComponent>().m_Resolution = *eventData.m_Resolution;
		if (eventData.m_WindowMode)
			world.WriteComponent<eng::settings::WindowStaticComponent>().m_WindowMode = *eventData.m_WindowMode;
	}
}
