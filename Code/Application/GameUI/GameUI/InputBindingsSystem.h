#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace eng
{
	class InputManager;
}

namespace gui::game_menu
{
	struct OpenRequest;
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
		using World = ecs::WorldView
			::Write<
			gui::game_menu::OpenRequest>
			::Read<
			eng::InputManager,
			gui::input::BindingsComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}