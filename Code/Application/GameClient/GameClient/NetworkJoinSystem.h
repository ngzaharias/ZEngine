#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

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
	class NetworkManager;
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
		using World = ecs::WorldView
			::Write<
			eng::level::LoadRequest,
			eng::network::ChangeRequest,
			gamestate::ChangeFinished,
			gamestate::NetworkJoinSingleton,
			gui::modal::MessageComponent,
			net::NetworkManager>
			::Read<
			eng::network::ChangeFinished,
			gamestate::StateSingleton,
			net::UserComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}