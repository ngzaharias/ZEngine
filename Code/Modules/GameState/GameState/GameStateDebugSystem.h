#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace gamestate
{
	struct DebugComponent;
}

namespace gamestate
{
	class DebugSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			gamestate::DebugComponent>;

		void Initialise(World& world);
	};
}
