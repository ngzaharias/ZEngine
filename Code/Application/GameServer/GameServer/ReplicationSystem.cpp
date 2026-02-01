#include "GameServerPCH.h"
#include "GameServer/ReplicationSystem.h"

#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/ReplicationComponent.h"
#include "ECS/ReplicationHost.h"
#include "ECS/WorldView.h"
#include "Network/Host.h"

void server::ReplicationSystem::Initialise(World& world)
{
	auto& host = world.WriteResource<net::Host>();
	m_Collection =
	{
		host.m_OnPeerConnected.Connect(*this, &server::ReplicationSystem::OnPeerConnected),
	};
}

void server::ReplicationSystem::Shutdown(World& world)
{
	m_Collection.Disconnect();
}

void server::ReplicationSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	auto& replicationHost = world.WriteResource<ecs::ReplicationHost>();
	const auto& replicationMap = replicationHost.GetReplicationMap();
	for (auto&& view : world.Query<ecs::query::Added<ecs::ReplicationComponent>>())
	{
		for (auto&& [peerId, replicationData] : replicationMap)
			replicationHost.StartReplicate(peerId, view);
	}

	for (auto&& view : world.Query<ecs::query::Removed<ecs::ReplicationComponent>>())
	{
		for (auto&& [peerId, replicationData] : replicationMap)
			replicationHost.StopReplicate(peerId, view);
	}

	for (const net::PeerId& peerId : m_PeersConnected)
	{
		for (auto&& view : world.Query<ecs::query::Include<ecs::ReplicationComponent>>())
			replicationHost.StartReplicate(peerId, view);
	}

	m_PeersConnected.RemoveAll();
}

void server::ReplicationSystem::OnPeerConnected(const net::PeerId& peerId)
{
	m_PeersConnected.Add(peerId);
}