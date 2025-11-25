#pragma once

#include "ECS/System.h"

namespace gamestate
{
	struct ChangeFinished;
	struct ChangeRequest;
	struct StateComponent;

	/// \brief System that holds a queue of requested states and the current state.
	class StateSystem : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			gamestate::StateComponent,
			const gamestate::ChangeFinished,
			const gamestate::ChangeRequest>;

		void Update(World& world, const GameTime& gameTime);
	};
}