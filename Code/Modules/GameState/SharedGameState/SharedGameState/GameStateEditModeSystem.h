#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace shared::gamestate
{
	struct EditModeComponent;
	struct EditModeToggleEvent;
}

namespace shared::gamestate
{
	class EditModeSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			shared::gamestate::EditModeComponent>
			::Read<
			shared::gamestate::EditModeToggleEvent>;

		void Initialise(World& world);

		void Update(World& world, const GameTime& gameTime);
	};
}
