#pragma once

#include "ECS/System.h"

namespace eng::network
{
	struct ChangeFinishedEvent;
	struct ChangeRequestEvent;
}

namespace gamestate
{
	struct StateSingleton;
	struct ChangeFinished;
}

namespace gamestate
{
	class NetworkHostSystem : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			// Components
			eng::network::ChangeRequestEvent,
			gamestate::ChangeFinished,
			const eng::network::ChangeRequestEvent,
			// Singletons
			const gamestate::StateSingleton>;

		void Update(World& world, const GameTime& gameTime);
	};
}