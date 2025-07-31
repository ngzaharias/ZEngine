#pragma once

#include "ECS/EntityWorld.h"
#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace ecs
{
	struct NameComponent;
}

namespace eng
{
	class UIManager;
}

namespace eng::application
{
	struct CloseRequestComponent;
}

namespace eng::level
{
	struct LoadRequest;
}

namespace eng::settings
{
	struct LaunchComponent;
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
		using World = ecs::WorldView<
			// Resources
			eng::UIManager,
			// Components
			ecs::NameComponent,
			eng::application::CloseRequestComponent,
			eng::level::LoadRequest,
			gui::game_menu::CloseRequest,
			const eng::settings::LaunchComponent,
			const gui::game_menu::ExitGameRequest,
			const gui::game_menu::ExitToMenuRequest,
			const gui::game_menu::OpenRequest,
			const gui::game_menu::WindowComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}