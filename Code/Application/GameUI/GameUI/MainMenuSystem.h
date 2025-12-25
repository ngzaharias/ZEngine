#pragma once

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

namespace gui::main_menu
{
	class MenuSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			eng::application::CloseRequest,
			eng::level::LoadRequest,
			eng::UIManager>
			::Read<
			eng::VersionSingleton,
			gui::main_menu::ContinueGameRequest,
			gui::main_menu::ExitGameRequest,
			gui::main_menu::LoadGameRequest,
			gui::main_menu::NewGameRequest,
			gui::main_menu::WindowComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}