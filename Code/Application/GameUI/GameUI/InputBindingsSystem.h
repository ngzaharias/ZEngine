#pragma once

#include "ECS/System.h"

namespace eng
{
	class InputManager;
}

namespace gui::game_menu
{
	struct OpenRequestComponent;
}

namespace gui::input
{
	struct BindingsComponent;
}

namespace gui::input
{
	class BindingsSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			// Resources
			const eng::InputManager,
			// Components
			gui::game_menu::OpenRequestComponent,
			const gui::input::BindingsComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}