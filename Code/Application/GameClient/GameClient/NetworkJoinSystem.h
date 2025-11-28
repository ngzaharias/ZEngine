#pragma once

#include "ECS/System.h"

namespace eng
{
	class NetworkManager;
}

namespace eng::level
{
	struct LoadRequestEvent;
}

namespace eng::network
{
	struct ChangeFinishedEvent;
	struct ChangeRequestEvent;
}

namespace gui::modal
{
	struct MessageComponent;
}

namespace net
{
	struct UserComponent;
}

namespace gamestate
{
	struct ChangeFinished;
	struct NetworkJoinSingleton;
	struct StateSingleton;
}

namespace gamestate
{
	class NetworkJoinSystem : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			// Resources
			eng::NetworkManager,
			// Components
			gui::modal::MessageComponent,
			const net::UserComponent,
			// Events
			eng::level::LoadRequestEvent,
			eng::network::ChangeRequestEvent,
			gamestate::ChangeFinished,
			const eng::network::ChangeFinishedEvent,
			// Singletons
			gamestate::NetworkJoinSingleton,
			const gamestate::StateSingleton>;

		void Update(World& world, const GameTime& gameTime);
	};
}