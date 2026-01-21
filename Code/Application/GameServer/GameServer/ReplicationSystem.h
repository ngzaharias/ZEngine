#pragma once

#include "Core/Delegate.h"
#include "Core/Set.h"
#include "ECS/System.h"
#include "ECS/WorldView.h"
#include "Network/PeerId.h"

namespace ecs
{
	class ReplicationHost;
	struct ReplicationComponent;
}

namespace net
{
	class Host;
}

namespace server
{
	class ReplicationSystem : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Read<
			ecs::ReplicationComponent,
			ecs::ReplicationHost,
			net::Host>;

		void Initialise(World& world);
		void Shutdown(World& world);

		void Update(World& world, const GameTime& gameTime);

	private:
		void OnPeerConnected(const net::PeerId& peerId);
		void OnPeerDisconnected(const net::PeerId& peerId);

	private:
		DelegateCollection m_Collection;
		Set<net::PeerId> m_PeersConnected;
		Set<net::PeerId> m_PeersDisconnected;

	};
}