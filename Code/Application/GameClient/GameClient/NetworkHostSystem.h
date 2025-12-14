#pragma once

#include "ECS/System.h"

namespace ecs
{
	template <typename... TTypes>
	class WorldView;
}

namespace eng::network
{
	struct ChangeFinished;
	struct ChangeRequest;
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
			eng::network::ChangeRequest,
			gamestate::ChangeFinished,
			const eng::network::ChangeRequest,
			// Singletons
			const gamestate::StateSingleton>;

		void Update(World& world, const GameTime& gameTime);
	};
}