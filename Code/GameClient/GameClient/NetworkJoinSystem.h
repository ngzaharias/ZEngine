#pragma once

#include "ECS/System.h"

namespace eng
{
	class NetworkManager;
}

namespace eng::level
{
	struct LoadRequestComponent;
}

namespace gui::modal
{
	struct MessageComponent;
}

namespace net
{
	struct UserComponent;
}

namespace network
{
	struct RequestComponent;
	struct RequestFinishedComponent;
}

namespace gamestate
{
	struct NetworkJoinComponent;
	struct StateComponent;
	struct StateFinishedComponent;

	class NetworkJoinSystem : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			eng::NetworkManager,
			eng::level::LoadRequestComponent,
			gui::modal::MessageComponent,
			gamestate::NetworkJoinComponent,
			gamestate::StateFinishedComponent,
			network::RequestComponent,
			const gamestate::StateComponent,
			const net::UserComponent,
			const network::RequestFinishedComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}