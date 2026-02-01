#include "ServerCursorPCH.h"
#include "ServerCursor/CursorTransformSystem.h"

#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/ReplicationComponent.h"
#include "ECS/WorldView.h"
#include "Network/Host.h"
#include "ServerCursor/CursorTransformSingleton.h"
#include "SharedCursor/CursorClientTransformEvent.h"
#include "SharedCursor/CursorTransformComponent.h"

void server::cursor::TransformSystem::Initialise(World& world)
{
	auto& host = world.WriteResource<net::Host>();
	m_Collection =
	{
		host.m_OnPeerDisconnected.Connect(*this, &server::cursor::TransformSystem::OnPeerDisconnected),
	};
}

void server::cursor::TransformSystem::Shutdown(World& world)
{
	m_Collection.Disconnect();
}

void server::cursor::TransformSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	Map<net::PeerId, Vector3f> requests;
	for (const auto& eventData : world.Events<shared::cursor::ClientTransformEvent>())
	{
		requests[eventData.m_PeerId] = eventData.m_Translate;
	}

	if (!requests.IsEmpty())
	{
		auto& singleton = world.WriteSingleton<server::cursor::TransformSingleton>();
		for (auto&& [peerId, translate] : requests)
		{
			ecs::Entity& entity = singleton.m_Peers[peerId];
			if (entity.IsUnassigned())
			{
				entity = world.CreateEntity();
				world.AddComponent<ecs::ReplicationComponent>(entity);
				auto& component = world.AddComponent<shared::cursor::TransformComponent>(entity);
				component.m_PeerId = peerId;
				component.m_Translate = translate;
			}
			else
			{
				auto& component = world.WriteComponent<shared::cursor::TransformComponent>(entity);
				component.m_Translate = translate;
			}
		}
	}

	if (!m_Disconnects.IsEmpty())
	{
		auto& singleton = world.WriteSingleton<server::cursor::TransformSingleton>();
		auto& peerData = singleton.m_Peers;
		for (const net::PeerId& peerId : m_Disconnects)
		{
			auto find = peerData.Find(peerId);
			if (find == peerData.end())
				continue;

			world.DestroyEntity(find->second);
			peerData.Remove(peerId);
		}
		m_Disconnects.RemoveAll();
	}
}

void server::cursor::TransformSystem::OnPeerDisconnected(const net::PeerId& peerId)
{
	m_Disconnects.Add(peerId);
}
