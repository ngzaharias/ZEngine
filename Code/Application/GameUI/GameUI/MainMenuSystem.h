#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace eng
{
	class UIManager;
	struct VersionComponent;
}

namespace eng::application
{
	struct CloseEvent;
}

namespace eng::level
{
	struct LoadEvent;
}

namespace gui::main_menu
{
	struct ContinueGameEvent;
	struct ExitGameEvent;
	struct LoadGameEvent;
	struct NewGameEvent;
	struct WindowTemplate;
}

namespace gui::main_menu
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
			eng::VersionComponent,
			gui::main_menu::ContinueGameEvent,
			gui::main_menu::ExitGameEvent,
			gui::main_menu::LoadGameEvent,
			gui::main_menu::NewGameEvent,
			gui::main_menu::WindowTemplate>;

		void Update(World& world, const GameTime& gameTime);
	};
}