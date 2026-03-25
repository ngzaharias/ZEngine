#include "ServerCursorPCH.h"
#include "ServerCursor/CursorTransformSystem.h"

#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/ReplicationComponent.h"
#include "ECS/WorldView.h"
#include "ServerCursor/CursorTransformComponent.h"
#include "ServerNetwork/NetworkPeerComponent.h"
#include "SharedCursor/CursorClientTransformEvent.h"
#include "SharedCursor/CursorTransformComponent.h"

void server::cursor::TransformSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	for (const auto& eventData : world.Events<shared::cursor::ClientTransformEvent>())
	{
		const auto& serverComponent = world.ReadComponent<server::cursor::TransformComponent>();
		const auto& peers = serverComponent.m_Peers;
		const auto find = peers.Find(eventData.m_PeerId);
		if (find == peers.end())
			continue;

		const ecs::Entity entity = find->second;
		auto& sharedComponent = world.WriteComponent<shared::cursor::TransformComponent>(entity);
		sharedComponent.m_Translate = eventData.m_Translate;
	}

	for (auto&& view : world.Query<ecs::query::Added<server::network::PeerComponent>::Include<const server::network::PeerComponent>>())
	{
		const auto& peerComponent = view.ReadRequired<server::network::PeerComponent>();
		const net::PeerId peerId = peerComponent.m_PeerId;

		const ecs::Entity entity = world.CreateEntity();
		world.AddComponent<ecs::ReplicationComponent>(entity);

		auto& sharedComponent = world.AddComponent<shared::cursor::TransformComponent>(entity);
		sharedComponent.m_PeerId = peerId;

		auto& serverComponent = world.WriteComponent<server::cursor::TransformComponent>();
		serverComponent.m_Peers.Set(peerId, entity);
	}

	for (auto&& view : world.Query<ecs::query::Removed<server::network::PeerComponent>::Include<const server::network::PeerComponent>>())
	{
		const auto& peerComponent = view.ReadRequired<server::network::PeerComponent>();
		auto& transformComponent = world.WriteComponent<server::cursor::TransformComponent>();
		
		const net::PeerId peerId = peerComponent.m_PeerId;
		auto& peers = transformComponent.m_Peers;

		const ecs::Entity entity = peers.Get(peerId);
		world.DestroyEntity(entity);
		peers.Remove(peerId);
	}
}
