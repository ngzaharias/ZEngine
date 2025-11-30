#pragma once

#include "ECS/EntityWorld.h"
#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace eng
{
	class UIManager;
	struct VersionSingleton;
}

namespace eng::application
{
	struct CloseRequest;
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
	struct OpenRequest;
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
			const gui::main_menu::ContinueGameRequest,
			const gui::main_menu::WindowComponent,
			// Events
			eng::application::CloseRequest,
			eng::level::LoadRequest,
			gui::settings::OpenRequest,
			const gui::main_menu::ExitGameRequest,
			const gui::main_menu::LoadGameRequest,
			const gui::main_menu::NewGameRequest,
			// Singletons
			const eng::VersionSingleton>;

		void Update(World& world, const GameTime& gameTime);
	};
}