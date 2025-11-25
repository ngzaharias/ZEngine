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
	struct StateComponent;

	class NetworkStopSystem : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			eng::network::ChangeRequest,
			gamestate::ChangeFinished,
			const eng::network::ChangeFinished,
			const gamestate::StateComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}