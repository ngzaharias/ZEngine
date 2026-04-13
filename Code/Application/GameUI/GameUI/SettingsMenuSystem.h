#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace camera
{
	struct SettingsComponent;
}

namespace eng
{
	class ThemeTable;
	class UIManager;
	class WindowManager;
}

namespace eng::settings
{
	struct AudioComponent;
	struct GameplayComponent;
	struct WindowComponent;
}

namespace gui::settings_menu
{
	struct CloseEvent;
	struct OpenEvent;
	struct ValueEvent;
	struct WindowComponent;
}

namespace gui::settings_menu
{
	class MenuSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			camera::SettingsComponent,
			eng::settings::AudioComponent,
			eng::settings::GameplayComponent,
			eng::settings::WindowComponent,
			eng::ThemeTable,
			eng::UIManager,
			eng::WindowManager,
			gui::settings_menu::WindowComponent>
			::Read<
			camera::SettingsComponent,
			eng::settings::AudioComponent,
			eng::settings::GameplayComponent,
			eng::settings::WindowComponent,
			eng::ThemeTable,
			eng::WindowManager,
			gui::settings_menu::CloseEvent,
			gui::settings_menu::OpenEvent,
			gui::settings_menu::ValueEvent>;

		void Update(World& world, const GameTime& gameTime);
	};
}