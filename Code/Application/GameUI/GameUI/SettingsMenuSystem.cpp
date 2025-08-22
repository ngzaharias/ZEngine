#include "GameUIPCH.h"
#include "GameUI/SettingsMenuSystem.h"

#include "Core/Name.h"
#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/AudioSettingsComponent.h"
#include "Engine/CameraSettingsComponent.h"
#include "Engine/WindowSettingsComponent.h"
#include "Engine/UIManager.h"
#include "GameUI/DCSettingsMenu.h"
#include "GameUI/SettingsMenuComponents.h"

namespace
{
	const str::Name strSettingsMenu_xaml = NAME("SettingsMenu.xaml");
}

void gui::settings_menu::MenuSystem::Update(World& world, const GameTime& gameTime)
{
	for (const ecs::Entity& entity : world.Query<ecs::query::Added<gui::settings_menu::WindowComponent>>())
	{
		auto& uiManager = world.WriteResource<eng::UIManager>();
		uiManager.CreateWidget(strSettingsMenu_xaml, true);
		auto& dataContext = uiManager.WriteDataContext<gui::DCSettingsMenu>(strSettingsMenu_xaml);
		dataContext.Initialise(world);
	}

	for (const ecs::Entity& entity : world.Query<ecs::query::Removed<gui::settings_menu::WindowComponent>>())
	{
		auto& uiManager = world.WriteResource<eng::UIManager>();
		uiManager.DestroyWidget(strSettingsMenu_xaml);
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

		// audio
		if (eventData.m_EffectVolume)
			world.WriteSingleton<eng::settings::AudioComponent>().m_EffectVolume = *eventData.m_EffectVolume;
		if (eventData.m_MasterVolume)
			world.WriteSingleton<eng::settings::AudioComponent>().m_MasterVolume = *eventData.m_MasterVolume;
		if (eventData.m_MusicVolume)
			world.WriteSingleton<eng::settings::AudioComponent>().m_MusicVolume = *eventData.m_MusicVolume;

		// gameplay
		if (eventData.m_MoveSpeed)
			world.WriteSingleton<eng::settings::CameraComponent>().m_TranslateSpeed = *eventData.m_MoveSpeed;
		if (eventData.m_ZoomRate)
			world.WriteSingleton<eng::settings::CameraComponent>().m_ZoomAmount = *eventData.m_ZoomRate;
		if (eventData.m_ZoomSpeed)
			world.WriteSingleton<eng::settings::CameraComponent>().m_ZoomSpeed = *eventData.m_ZoomSpeed;

		// graphics
		if (eventData.m_Monitor)
			world.WriteSingleton<eng::settings::WindowComponent>().m_Monitor = *eventData.m_Monitor;
		if (eventData.m_RefreshRate)
			world.WriteSingleton<eng::settings::WindowComponent>().m_RefreshRate = *eventData.m_RefreshRate;
		if (eventData.m_Resolution)
			world.WriteSingleton<eng::settings::WindowComponent>().m_Resolution = *eventData.m_Resolution;
		if (eventData.m_WindowMode)
			world.WriteSingleton<eng::settings::WindowComponent>().m_WindowMode = *eventData.m_WindowMode;
	}
}
