#pragma once

#include "ECS/EntityWorld.h"
#include "ECS/System.h"
#include "ECS/WorldView.h"

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

namespace gui::main_menu
{
	struct ContinueGameRequest;
	struct ExitGameRequest;
	struct LoadGameRequest;
	struct NewGameRequest;
	struct WindowComponent;
}

namespace gui::settings
{
	struct OpenRequestComponent;
}

namespace gui::main_menu
{
	class MenuSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			// Resources
			eng::UIManager,
			// Components
			eng::application::CloseRequestComponent,
			eng::level::LoadRequest,
			gui::settings::OpenRequestComponent,
			const gui::main_menu::ContinueGameRequest,
			const gui::main_menu::ExitGameRequest,
			const gui::main_menu::LoadGameRequest,
			const gui::main_menu::NewGameRequest,
			const gui::main_menu::WindowComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}