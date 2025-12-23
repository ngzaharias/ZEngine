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
	struct CloseRequest;
}

namespace eng::level
{
	struct LoadRequest;
}

namespace eng::settings
{
	struct LaunchSingleton;
}

namespace gui::game_menu
{
	struct CloseRequest;
	struct ExitGameRequest;
	struct ExitToMenuRequest;
	struct ResumeRequest;
	struct OpenRequest;
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
			eng::application::CloseRequest,
			eng::InputManager,
			eng::level::LoadRequest,
			eng::UIManager,
			gui::game_menu::CloseRequest>
			::Read<
			eng::settings::LaunchSingleton,
			gui::game_menu::ExitGameRequest,
			gui::game_menu::ExitToMenuRequest,
			gui::game_menu::OpenRequest,
			gui::game_menu::WindowComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}