#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace gamestate
{
	struct ChangeFinishedEvent;
	struct ChangeRequestEvent;
	struct StateComponent;
}

namespace gamestate
{
	/// \brief System that holds a queue of requested states and the current state.
	class StateSystem : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			gamestate::StateComponent>
			::Read<
			gamestate::ChangeFinishedEvent,
			gamestate::ChangeRequestEvent>;

		void Update(World& world, const GameTime& gameTime);
	};
}