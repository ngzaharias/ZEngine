#pragma once

#include "ECS/System.h"

namespace gamestate
{
	struct ChangeFinished;
	struct ChangeRequest;
	struct StateSingleton;
}

namespace gamestate
{
	/// \brief System that holds a queue of requested states and the current state.
	class StateSystem : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			// Components
			const gamestate::ChangeFinished,
			const gamestate::ChangeRequest,
			// Singletons
			gamestate::StateSingleton>;

		void Update(World& world, const GameTime& gameTime);
	};
}