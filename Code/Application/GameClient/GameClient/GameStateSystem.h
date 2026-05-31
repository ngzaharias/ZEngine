#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace shared::gamestate
{
	struct ChangeFinishedEvent;
	struct ChangeRequestEvent;
	struct StateComponent;
}

namespace shared::gamestate
{
	/// \brief System that holds a queue of requested states and the current state.
	class StateSystem : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			shared::gamestate::StateComponent>
			::Read<
			shared::gamestate::ChangeFinishedEvent,
			shared::gamestate::ChangeRequestEvent>;

		void Update(World& world, const GameTime& gameTime);
	};
}