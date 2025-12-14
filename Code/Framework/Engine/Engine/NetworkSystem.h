#pragma once

#include "Core/Array.h"
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
		using World = ecs::WorldView<
			// Resources
			eng::NetworkManager,
			// Events
			eng::network::ChangeFinished,
			const eng::network::ChangeRequest,
			// Singletons
			eng::network::StateSingleton>;

		void Update(World& world, const GameTime& gameTime);

	private:
		bool m_HasConnected = false;
		bool m_HasDisconnected = false;
	};
}