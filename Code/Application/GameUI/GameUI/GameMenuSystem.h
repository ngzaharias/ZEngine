#pragma once

#include "ECS/System.h"


namespace ecs
{
	template <typename... TTypes>
	class WorldView;
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
		using World = ecs::WorldView <
			// Resources
			eng::InputManager,
			eng::UIManager,
			// Components
			ecs::NameComponent,
			const gui::game_menu::WindowComponent,
			// Events
			eng::application::CloseRequest,
			eng::level::LoadRequest,
			gui::game_menu::CloseRequest,
			const gui::game_menu::ExitGameRequest,
			const gui::game_menu::ExitToMenuRequest,
			const gui::game_menu::OpenRequest,
			// Singletons
			const eng::settings::LaunchSingleton>;

		void Update(World& world, const GameTime& gameTime);
	};
}