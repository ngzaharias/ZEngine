#pragma once

#include "ECS/EntityWorld.h"
#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace eng
{
	class InputManager;
	class UIManager;
	class WindowManager;
}

namespace eng::settings
{
	struct LocalComponent;
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
			eng::InputManager,
			eng::UIManager,
			eng::WindowManager,
			// Components
			eng::settings::LocalComponent,
			gui::settings_menu::WindowComponent,
			const gui::settings_menu::CloseRequest,
			const gui::settings_menu::OpenRequest,
			const gui::settings_menu::ValueRequest>;

		void Update(World& world, const GameTime& gameTime);
	};
}