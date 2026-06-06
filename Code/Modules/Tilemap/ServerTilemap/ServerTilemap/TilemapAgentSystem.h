#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace shared::tilemap
{
	struct AgentComponent;
	struct AgentTemplate;
}

namespace server::tilemap
{
	class AgentSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			shared::tilemap::AgentComponent>
			::Read<
			shared::tilemap::AgentTemplate>;

		void Update(World& world, const GameTime& gameTime);
	};
}