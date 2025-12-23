#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace eng::network
{
	struct ChangeFinished;
	struct ChangeRequest;
}

namespace gamestate
{
	struct ChangeFinished;
	struct StateSingleton;
}

namespace gamestate
{
	class NetworkStopSystem : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			eng::network::ChangeRequest,
			gamestate::ChangeFinished>
			::Read<
			eng::network::ChangeFinished,
			gamestate::StateSingleton>;

		void Update(World& world, const GameTime& gameTime);
	};
}