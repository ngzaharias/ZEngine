#pragma once

#include "ECS/System.h"

namespace eng::network
{
	struct RequestComponent;
	struct RequestFinishedComponent;
}

namespace gamestate
{
	struct StateComponent;
	struct StateFinishedComponent;

	class NetworkHostSystem : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			eng::network::RequestComponent,
			gamestate::StateFinishedComponent,
			const eng::network::RequestFinishedComponent,
			const gamestate::StateComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}