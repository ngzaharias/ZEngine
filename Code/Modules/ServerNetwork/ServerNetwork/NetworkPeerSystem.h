#pragma once

#include "Core/Delegate.h"
#include "Core/Map.h"
#include "ECS/System.h"
#include "ECS/WorldView.h"
#include "Network/PeerId.h"

namespace ecs
{
	struct NameComponent;
}

namespace net
{
	class Host;
}

namespace server::network
{
	struct PeerComponent;
}

namespace server::network
{
	class PeerSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			ecs::NameComponent,
			server::network::PeerComponent>
			::Read<
			net::Host>;

		void Initialise(World& world);
		void Shutdown(World& world);

		void Update(World& world, const GameTime& gameTime);

	private:
		void OnPeerConnected(const net::PeerId& peerId);
		void OnPeerDisconnected(const net::PeerId& peerId);

	private:
		DelegateCollection m_Collection = {};
		Map<net::PeerId, bool> m_PeersChanges = {};
	};
}
