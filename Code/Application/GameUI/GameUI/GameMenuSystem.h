#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace ecs
{
	struct NameComponent;
}

namespace eng
{
	class InputManager;
	class UIManager;
}

namespace eng::application
{
	struct CloseEvent;
}

namespace eng::level
{
	struct LoadEvent;
}

namespace eng::settings
{
	struct LaunchSingleton;
}

namespace gui::game_menu
{
	struct CloseEvent;
	struct ExitGameEvent;
	struct ExitToMenuEvent;
	struct ResumeEvent;
	struct OpenEvent;
	struct WindowComponent;
}

namespace gui::game_menu
{
	class MenuSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			ecs::NameComponent,
			eng::application::CloseEvent,
			eng::InputManager,
			eng::level::LoadEvent,
			eng::UIManager,
			gui::game_menu::CloseEvent,
			gui::game_menu::WindowComponent>
			::Read<
			eng::settings::LaunchSingleton,
			gui::game_menu::ExitGameEvent,
			gui::game_menu::ExitToMenuEvent,
			gui::game_menu::OpenEvent>;

		void Update(World& world, const GameTime& gameTime);
	};
}