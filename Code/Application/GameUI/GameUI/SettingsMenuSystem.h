#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace eng
{
	class ThemeTable;
	class UIManager;
	class WindowManager;
}

namespace eng::settings
{
	struct AudioSingleton;
	struct CameraSingleton;
	struct GameplaySingleton;
	struct WindowSingleton;
}

namespace gui::settings_menu
{
	struct CloseRequest;
	struct OpenRequest;
	struct ValueRequest;
	struct WindowComponent;
}

namespace gui::settings_menu
{
	class MenuSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			eng::settings::AudioSingleton,
			eng::settings::CameraSingleton,
			eng::settings::GameplaySingleton,
			eng::settings::WindowSingleton,
			eng::ThemeTable,
			eng::UIManager,
			eng::WindowManager,
			gui::settings_menu::WindowComponent>
			::Read<
			gui::settings_menu::CloseRequest,
			gui::settings_menu::OpenRequest,
			gui::settings_menu::ValueRequest>;

		void Update(World& world, const GameTime& gameTime);
	};
}