#include "ClientNetworkPCH.h"
#include "ClientNetwork/NetworkPeerSystem.h"

#include "ClientNetwork/NetworkPeerComponent.h"
#include "ECS/EntityWorld.h"
#include "ECS/NameComponent.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Network/Peer.h"

void client::network::PeerSystem::Initialise(World& world)
{
	auto& peer = world.WriteResource<net::Peer>();
	m_Collection =
	{
		peer.m_OnHandshake.Connect(*this, &client::network::PeerSystem::OnPeerHandshake),
		peer.m_OnDisconnected.Connect(*this, &client::network::PeerSystem::OnPeerDisconnected),
	};
}

void client::network::PeerSystem::Shutdown(World& world)
{
	m_Collection.Disconnect();
}

void client::network::PeerSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	if (!m_PeersChanges.IsEmpty())
	{
		Map<net::PeerId, ecs::Entity> peerMap;
		for (auto&& view : world.Query<ecs::query::Include<const client::network::PeerComponent>>())
		{
			const auto& peerComponent = view.ReadRequired<client::network::PeerComponent>();
			peerMap[peerComponent.m_PeerId] = view;
		}

		for (auto&& [peerId, hasConnected] : m_PeersChanges)
		{
			if (hasConnected)
			{
				const ecs::Entity entity = world.CreateEntity();
				auto& component = world.AddComponent<client::network::PeerComponent>(entity);
				component.m_PeerId = peerId;
			}
			else
			{
				const ecs::Entity entity = peerMap.Get(peerId);
				world.RemoveComponent<client::network::PeerComponent>(entity);
			}
		}
		m_PeersChanges.RemoveAll();
	}
}

void client::network::PeerSystem::OnPeerHandshake(const net::PeerId& peerId)
{
	m_PeersChanges[peerId] = true;
}

void client::network::PeerSystem::OnPeerDisconnected(const net::PeerId& peerId)
{
	m_PeersChanges[peerId] = false;
}