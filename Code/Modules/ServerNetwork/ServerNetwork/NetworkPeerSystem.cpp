#include "ServerNetworkPCH.h"
#include "ServerNetwork/NetworkPeerSystem.h"

#include "ECS/EntityWorld.h"
#include "ECS/NameComponent.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Network/Host.h"
#include "ServerNetwork/NetworkPeerComponent.h"

void server::network::PeerSystem::Initialise(World& world)
{
	auto& host = world.WriteResource<net::Host>();
	m_Collection =
	{
		host.m_OnPeerConnected.Connect(*this, &server::network::PeerSystem::OnPeerConnected),
		host.m_OnPeerDisconnected.Connect(*this, &server::network::PeerSystem::OnPeerDisconnected),
	};
}

void server::network::PeerSystem::Shutdown(World& world)
{
	m_Collection.Disconnect();
}

void server::network::PeerSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	if (!m_PeersChanges.IsEmpty())
	{
		Map<net::PeerId, ecs::Entity> peerMap;
		for (auto&& view : world.Query<ecs::query::Include<const server::network::PeerComponent>>())
		{
			const auto& peerComponent = view.ReadRequired<server::network::PeerComponent>();
			peerMap[peerComponent.m_PeerId] = view;
		}

		for (auto&& [peerId, hasConnected] : m_PeersChanges)
		{
			if (hasConnected)
			{
				const ecs::Entity entity = world.CreateEntity();
				auto& component = world.AddComponent<server::network::PeerComponent>(entity);
				component.m_PeerId = peerId;
			}
			else
			{
				const ecs::Entity entity = peerMap.Get(peerId);
				world.RemoveComponent<server::network::PeerComponent>(entity);
			}
		}
		m_PeersChanges.RemoveAll();
	}
}

void server::network::PeerSystem::OnPeerConnected(const net::PeerId& peerId)
{
	m_PeersChanges[peerId] = true;
}

void server::network::PeerSystem::OnPeerDisconnected(const net::PeerId& peerId)
{
	m_PeersChanges[peerId] = false;
}