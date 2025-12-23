#pragma once

#include "Core/Array.h"
#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace eng
{
	class NetworkManager;
}

namespace net
{
	struct PeerId;
}

namespace eng::network
{
	struct ChangeFinished;
	struct ChangeRequest;
	struct StateSingleton;
}

namespace eng::network
{
	class NetworkSystem : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			eng::network::ChangeFinished,
			eng::network::StateSingleton,
			eng::NetworkManager>
			::Read<
			eng::network::ChangeRequest>;

		void Update(World& world, const GameTime& gameTime);

	private:
		bool m_HasConnected = false;
		bool m_HasDisconnected = false;
	};
}