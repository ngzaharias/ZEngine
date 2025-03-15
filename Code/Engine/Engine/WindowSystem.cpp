#include "EnginePCH.h"
#include "Engine/WindowSystem.h"

#include "Core/Path.h"
#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/SettingsComponents.h"
#include "Engine/Window.h"
#include "Engine/WindowManager.h"

void eng::WindowSystem::Initialise(World& world)
{
	const auto& localSettings = world.ReadSingleton<eng::settings::LocalComponent>();
	const auto& windowSettings = localSettings.m_Window;

	auto& manager = world.WriteResource<eng::WindowManager>();
	eng::Window* window = manager.GetWindow(0);
	window->SetMode(windowSettings.m_Mode);
	window->SetResolution(windowSettings.m_Resolution);
	window->Refresh();
}

void eng::WindowSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	if (world.HasAny<ecs::query::Updated<eng::settings::LocalComponent>>())
	{
		const auto& localSettings = world.ReadSingleton<eng::settings::LocalComponent>();
		const auto& windowSettings = localSettings.m_Window;

		auto& manager = world.WriteResource<eng::WindowManager>();
		eng::Window* window = manager.GetWindow(0);
		window->SetMode(windowSettings.m_Mode);
		window->SetResolution(windowSettings.m_Resolution);
		window->SetRefreshRate(windowSettings.m_RefreshRate);
		window->Refresh();
	}

	const auto& manager = world.ReadResource<eng::WindowManager>();
	if (const eng::Window* window = manager.GetWindow(0))
	{
		const auto& localSettings = world.ReadSingleton<eng::settings::LocalComponent>();
		const auto& windowSettings = localSettings.m_Window;

		bool hasChanged = false;
		hasChanged |= windowSettings.m_Mode != window->GetMode();
		hasChanged |= windowSettings.m_Resolution != window->GetResolution();
		hasChanged |= windowSettings.m_RefreshRate != window->GetRefreshRate();
		if (hasChanged)
		{
			auto& writeLocal = world.WriteSingleton<eng::settings::LocalComponent>();
			auto& writeWindow = writeLocal.m_Window;
			writeWindow.m_Mode = window->GetMode();
			writeWindow.m_Resolution = window->GetResolution();
			writeWindow.m_RefreshRate = window->GetRefreshRate();
		}
	}
}