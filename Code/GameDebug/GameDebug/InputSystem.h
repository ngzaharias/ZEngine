#pragma once

#include "ECS/System.h"

namespace eng
{
	class InputManager;
}

namespace dbg
{
	class InputSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			eng::InputManager>;

		void Initialise(World& world);
		void Shutdown(World& world);

		void Update(World& world, const GameTime& gameTime);
	};
}