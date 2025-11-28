#pragma once

#include "ECS/System.h"

namespace eng::network
{
	struct ChangeFinishedEvent;
	struct ChangeRequestEvent;
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
		using World = ecs::WorldView<
			// Components
			eng::network::ChangeRequestEvent,
			gamestate::ChangeFinished,
			const eng::network::ChangeFinishedEvent,
			// Singletons
			const gamestate::StateSingleton>;

		void Update(World& world, const GameTime& gameTime);
	};
}