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
	struct RequestComponent;
	struct RequestFinishedComponent;
	struct StateComponent;

	class NetworkSystem : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			eng::NetworkManager,
			eng::network::RequestFinishedComponent,
			eng::network::StateComponent,
			const eng::network::RequestComponent>;

		void Update(World& world, const GameTime& gameTime);

	private:
		bool m_HasConnected = false;
		bool m_HasDisconnected = false;
	};
}