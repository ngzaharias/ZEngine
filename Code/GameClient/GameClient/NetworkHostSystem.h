#pragma once

#include "ECS/System.h"

namespace network
{
	struct RequestComponent;
	struct RequestFinishedComponent;
}

namespace gamestate
{
	struct NetworkHostComponent;
	struct StateComponent;
	struct StateFinishedComponent;

	class NetworkHostSystem : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			gamestate::NetworkHostComponent,
			gamestate::StateFinishedComponent,
			network::RequestComponent,
			const gamestate::StateComponent,
			const network::RequestFinishedComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}