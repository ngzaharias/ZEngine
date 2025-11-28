#pragma once

#include "Core/Array.h"
#include "ECS/System.h"

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
	struct ChangeFinishedEvent;
	struct ChangeRequestEvent;
	struct StateSingleton;
}

namespace eng::network
{
	class NetworkSystem : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			// Resources
			eng::NetworkManager,
			// Events
			eng::network::ChangeFinishedEvent,
			const eng::network::ChangeRequestEvent,
			// Singletons
			eng::network::StateSingleton>;

		void Update(World& world, const GameTime& gameTime);

	private:
		bool m_HasConnected = false;
		bool m_HasDisconnected = false;
	};
}