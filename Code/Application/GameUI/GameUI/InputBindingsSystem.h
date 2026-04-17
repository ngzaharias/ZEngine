#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace eng
{
	class InputManager;
}

namespace gui::game_menu
{
	struct OpenEvent;
}

namespace gui::input
{
	struct BindingsTemplate;
}

namespace gui::input
{
	class BindingsSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			eng::InputManager,
			gui::game_menu::OpenEvent>
			::Read<
			gui::input::BindingsTemplate>;

		void Update(World& world, const GameTime& gameTime);
	};
}