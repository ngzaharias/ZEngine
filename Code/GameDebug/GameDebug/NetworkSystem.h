#pragma once

#include "ECS/System.h"

namespace gamestate
{
	struct RequestComponent;
}

namespace ecs
{
	struct NameComponent;
}

namespace dbg
{
	struct NetworkWindowComponent;
	struct NetworkWindowRequestComponent;

	class NetworkSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			dbg::NetworkWindowComponent,
			ecs::NameComponent,
			gamestate::RequestComponent,
			const dbg::NetworkWindowRequestComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}