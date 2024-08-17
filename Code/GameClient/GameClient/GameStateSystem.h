#pragma once

#include "ECS/System.h"

namespace gamestate
{
	struct RequestComponent;
	struct StateComponent;
	struct StateFinishedComponent;

	/// \brief System that holds a queue of requested states and the current state.
	class StateSystem : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			gamestate::StateComponent,
			const gamestate::RequestComponent,
			const gamestate::StateFinishedComponent>;

		void Initialise(World& world);
		void Shutdown(World& world);

		void Update(World& world, const GameTime& gameTime);
	};
}