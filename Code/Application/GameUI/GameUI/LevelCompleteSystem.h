#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace eng
{
	class UIManager;
}

namespace eng::application
{
	struct CloseEvent;
}

namespace eng::level
{
	struct LoadedComponent;
	struct LoadEvent;
}

namespace eng::settings
{
	struct LaunchSingleton;
}

namespace gui::level_complete
{
	struct ExitGameEvent;
	struct ExitToMenuEvent;
	struct ResetGameEvent;
	struct WindowComponent;
}

namespace gui::level_complete
{
	class MenuSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			eng::application::CloseEvent,
			eng::level::LoadEvent,
			eng::UIManager>
			::Read<
			eng::level::LoadedComponent,
			eng::settings::LaunchSingleton,
			gui::level_complete::ExitGameEvent,
			gui::level_complete::ExitToMenuEvent,
			gui::level_complete::ResetGameEvent,
			gui::level_complete::WindowComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}