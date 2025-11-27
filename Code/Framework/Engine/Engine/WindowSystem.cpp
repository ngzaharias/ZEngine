#include "EnginePCH.h"
#include "Engine/WindowSystem.h"

#include "Core/Path.h"
#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/WindowSettingsComponent.h"
#include "Engine/Window.h"
#include "Engine/WindowManager.h"

void eng::WindowSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	if (world.HasAny<eng::settings::WindowSingleton>())
	{
		const auto& windowSettings = world.ReadSingleton<eng::settings::WindowSingleton>();

		auto& manager = world.WriteResource<eng::WindowManager>();
		if (eng::Window* window = manager.GetWindow(0))
		{
			window->Refresh(
				windowSettings.m_WindowMode, 
				windowSettings.m_Resolution, 
				windowSettings.m_RefreshRate, 
				windowSettings.m_Monitor);
		}
	}
}