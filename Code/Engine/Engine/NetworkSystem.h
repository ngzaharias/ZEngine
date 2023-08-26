#pragma once

#include <Core/Array.h>

#include <ECS/System.h>

#include <entt/signal/sigh.hpp>

namespace eng
{
	class NetworkManager;
}

namespace net
{
	struct PeerId;
}

namespace network
{
	struct RequestComponent;
	struct RequestFinishedComponent;
	struct StateComponent;

	class NetworkSystem : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			eng::NetworkManager,
			network::RequestFinishedComponent,
			network::StateComponent,
			const network::RequestComponent>;

		void Initialise(World& world);
		void Shutdown(World& world);
		void Update(World& world, const GameTime& gameTime);

	private:
		void OnClientConnected(const net::PeerId& peerId);
		void OnClientDisconnected(const net::PeerId& peerId);

	private:
		Array<entt::connection> m_Connections = { };

		bool m_HasConnected = false;
		bool m_HasDisconnected = false;
	};
}