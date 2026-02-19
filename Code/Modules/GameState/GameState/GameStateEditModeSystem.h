#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace gamestate
{
	struct EditModeComponent;
	struct EditModeToggleEvent;
}

namespace gamestate
{
	class EditModeSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			gamestate::EditModeComponent>
			::Read<
			gamestate::EditModeToggleEvent>;

		void Initialise(World& world);

		void Update(World& world, const GameTime& gameTime);
	};
}
