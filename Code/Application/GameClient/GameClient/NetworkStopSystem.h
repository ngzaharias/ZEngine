#pragma once

#include "ECS/System.h"

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
		using World = ecs::WorldView<
			// Components
			eng::network::ChangeRequest,
			gamestate::ChangeFinished,
			const eng::network::ChangeFinished,
			// Singletons
			const gamestate::StateSingleton>;

		void Update(World& world, const GameTime& gameTime);
	};
}