#pragma once

#include "ECS/System.h"

namespace ecs
{
	template <typename... TTypes>
	class WorldView;
}

namespace eng
{
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
		using World = ecs::WorldView<
			// Resources
			eng::UIManager,
			eng::WindowManager,
			// Components
			gui::settings_menu::WindowComponent,
			const gui::settings_menu::CloseRequest,
			const gui::settings_menu::OpenRequest,
			const gui::settings_menu::ValueRequest,
			// Singletons
			eng::settings::AudioSingleton,
			eng::settings::CameraSingleton,
			eng::settings::GameplaySingleton,
			eng::settings::WindowSingleton>;

		void Update(World& world, const GameTime& gameTime);
	};
}