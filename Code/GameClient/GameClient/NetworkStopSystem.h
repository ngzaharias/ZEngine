#pragma once

#include "ECS/System.h"

namespace network
{
	struct RequestComponent;
	struct RequestFinishedComponent;
}

namespace gamestate
{
	struct NetworkStopComponent;
	struct StateComponent;
	struct StateFinishedComponent;

	class NetworkStopSystem : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			gamestate::NetworkStopComponent,
			gamestate::StateFinishedComponent,
			network::RequestComponent,
			const gamestate::StateComponent,
			const network::RequestFinishedComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}