#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace shared::gamestate
{
	struct DebugComponent;
}

namespace shared::gamestate
{
	class DebugSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			shared::gamestate::DebugComponent>;

		void Initialise(World& world);
	};
}
