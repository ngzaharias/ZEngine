#pragma once

#include "ECS/System.h"

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
	struct NetworkJoinComponent;
	struct StateComponent;

	class NetworkJoinSystem : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			eng::level::LoadRequest,
			eng::NetworkManager,
			eng::network::ChangeRequest,
			gamestate::ChangeFinished,
			gamestate::NetworkJoinComponent,
			gui::modal::MessageComponent,
			const eng::network::ChangeFinished,
			const gamestate::StateComponent,
			const net::UserComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}