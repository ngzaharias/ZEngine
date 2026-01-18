#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace net
{
	class ReplicationSystem : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<>;

		void Update(World& world, const GameTime& gameTime);
	};
}