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

namespace eng::network
{
	struct RequestComponent;
	struct RequestFinishedComponent;
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
	struct NetworkJoinComponent;
	struct StateComponent;
	struct StateFinishedComponent;

	class NetworkJoinSystem : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			eng::level::LoadRequestComponent,
			eng::NetworkManager,
			eng::network::RequestComponent,
			gui::modal::MessageComponent,
			gamestate::NetworkJoinComponent,
			gamestate::StateFinishedComponent,
			const eng::network::RequestFinishedComponent,
			const gamestate::StateComponent,
			const net::UserComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}