#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

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
		using World = ecs::WorldView
			::Write<
			eng::InputManager>
			::Read<
			tilemap::AgentComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}