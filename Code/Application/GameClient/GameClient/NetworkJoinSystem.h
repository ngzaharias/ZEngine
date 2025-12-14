#pragma once

#include "ECS/System.h"

namespace ecs
{
	template <typename... TTypes>
	class WorldView;
}

namespace eng
{
	class NetworkManager;
}

namespace eng::level
{
	struct LoadRequest;
}

namespace eng::network
{
	struct ChangeFinished;
	struct ChangeRequest;
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
			eng::level::LoadRequest,
			eng::network::ChangeRequest,
			gamestate::ChangeFinished,
			const eng::network::ChangeFinished,
			// Singletons
			gamestate::NetworkJoinSingleton,
			const gamestate::StateSingleton>;

		void Update(World& world, const GameTime& gameTime);
	};
}