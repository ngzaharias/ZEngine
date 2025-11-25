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
	struct ChangeFinished;
	struct ChangeRequest;
	struct StateComponent;
}

namespace eng::network
{
	class NetworkSystem : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			eng::NetworkManager,
			eng::network::ChangeFinished,
			eng::network::StateComponent,
			const eng::network::ChangeRequest>;

		void Update(World& world, const GameTime& gameTime);

	private:
		bool m_HasConnected = false;
		bool m_HasDisconnected = false;
	};
}