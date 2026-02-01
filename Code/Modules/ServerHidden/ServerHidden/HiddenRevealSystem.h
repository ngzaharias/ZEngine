#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace shared::hidden
{
	struct RevealedEvent;
	struct SelectedEvent;
}

namespace server::hidden
{
	class RevealSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			shared::hidden::RevealedEvent>
			::Read<
			shared::hidden::SelectedEvent>;

		void Update(World& world, const GameTime& gameTime);
	};
}