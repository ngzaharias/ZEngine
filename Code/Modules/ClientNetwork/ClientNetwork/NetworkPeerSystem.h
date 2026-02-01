#pragma once

#include "Core/Delegate.h"
#include "Core/Map.h"
#include "ECS/System.h"
#include "ECS/WorldView.h"
#include "Network/PeerId.h"

namespace client::network
{
	struct PeerComponent;
}

namespace ecs
{
	struct NameComponent;
}

namespace net
{
	class Peer;
}

namespace client::network
{
	class PeerSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			client::network::PeerComponent,
			ecs::NameComponent>
			::Read<
			net::Peer>;

		void Initialise(World& world);
		void Shutdown(World& world);

		void Update(World& world, const GameTime& gameTime);

	private:
		void OnPeerHandshake(const net::PeerId& peerId);
		void OnPeerDisconnected(const net::PeerId& peerId);

	private:
		DelegateCollection m_Collection = {};
		Map<net::PeerId, bool> m_PeersChanges = {};
	};
}
