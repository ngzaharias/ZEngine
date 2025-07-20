#pragma once

#include "ECS/EntityWorld.h"
#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace eng
{
	class InputManager;
	class UIManager;
}

namespace gui::settings_menu
{
	struct CloseRequest;
	struct OpenRequest;
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
			// Components
			gui::settings_menu::WindowComponent,
			const gui::settings_menu::CloseRequest,
			const gui::settings_menu::OpenRequest>;

		void Update(World& world, const GameTime& gameTime);
	};
}