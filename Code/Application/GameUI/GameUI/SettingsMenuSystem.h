#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace camera
{
	struct SettingsStaticComponent;
}

namespace eng
{
	class ThemeTable;
	class UIManager;
	class WindowManager;
}

namespace eng::settings
{
	struct AudioStaticComponent;
	struct GameplayStaticComponent;
	struct WindowStaticComponent;
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
			camera::SettingsStaticComponent,
			eng::settings::AudioStaticComponent,
			eng::settings::GameplayStaticComponent,
			eng::settings::WindowStaticComponent,
			eng::ThemeTable,
			eng::UIManager,
			eng::WindowManager,
			gui::settings_menu::WindowComponent>
			::Read<
			gui::settings_menu::CloseEvent,
			gui::settings_menu::OpenEvent,
			gui::settings_menu::ValueEvent>;

		void Update(World& world, const GameTime& gameTime);
	};
}