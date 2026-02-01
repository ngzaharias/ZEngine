#include "ServerCursorPCH.h"
#include "ServerCursor/CursorTransformSystem.h"

#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/ReplicationComponent.h"
#include "ECS/WorldView.h"
#include "ServerCursor/CursorTransformSingleton.h"
#include "ServerNetwork/NetworkPeerComponent.h"
#include "SharedCursor/CursorClientTransformEvent.h"
#include "SharedCursor/CursorTransformComponent.h"

void server::cursor::TransformSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	for (const auto& eventData : world.Events<shared::cursor::ClientTransformEvent>())
	{
		const auto& transformSingleton = world.ReadSingleton<server::cursor::TransformSingleton>();
		const auto& peers = transformSingleton.m_Peers;
		const auto find = peers.Find(eventData.m_PeerId);
		if (find == peers.end())
			continue;

		const ecs::Entity entity = find->second;
		auto& transformComponent = world.WriteComponent<shared::cursor::TransformComponent>(entity);
		transformComponent.m_Translate = eventData.m_Translate;
	}

	for (auto&& view : world.Query<ecs::query::Added<server::network::PeerComponent>::Include<server::network::PeerComponent>>())
	{
		const auto& peerComponent = view.ReadRequired<server::network::PeerComponent>();
		const net::PeerId peerId = peerComponent.m_PeerId;

		const ecs::Entity entity = world.CreateEntity();
		world.AddComponent<ecs::ReplicationComponent>(entity);

		auto& transformComponent = world.AddComponent<shared::cursor::TransformComponent>(entity);
		transformComponent.m_PeerId = peerId;

		auto& transformSingleton = world.WriteSingleton<server::cursor::TransformSingleton>();
		transformSingleton.m_Peers.Set(peerId, entity);
	}

	for (auto&& view : world.Query<ecs::query::Removed<server::network::PeerComponent>::Include<server::network::PeerComponent>>())
	{
		const auto& peerComponent = view.ReadRequired<server::network::PeerComponent>();
		auto& transformSingleton = world.WriteSingleton<server::cursor::TransformSingleton>();
		
		const net::PeerId peerId = peerComponent.m_PeerId;
		auto& peers = transformSingleton.m_Peers;

		const ecs::Entity entity = peers.Get(peerId);
		world.DestroyEntity(entity);
		peers.Remove(peerId);
	}
}
