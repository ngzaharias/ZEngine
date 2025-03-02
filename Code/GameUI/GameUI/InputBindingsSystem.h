#pragma once

#include "ECS/System.h"

namespace eng
{
	struct InputComponent;
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
			gui::game_menu::OpenRequestComponent,
			const eng::InputComponent,
			const gui::input::BindingsComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}