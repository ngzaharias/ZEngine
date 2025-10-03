#pragma once

#include "ECS/System.h"

namespace eng
{
	class InputManager;
}

namespace tilemap
{
	struct AgentComponent;
}

namespace tactics
{
	class InputSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			// Resources
			eng::InputManager,
			// Components
			const tilemap::AgentComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}