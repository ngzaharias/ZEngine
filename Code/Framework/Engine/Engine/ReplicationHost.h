#pragma once

#include "Core/Map.h"
#include "Core/Set.h"
#include "Core/SparseArray.h"
#include "Core/TypeInfo.h"
#include "ECS/Entity.h"
#include "Network/PeerId.h"

class GameTime;

namespace ecs
{
	class EntityWorld;
}

namespace net
{
	struct ComponentEntry;
}

namespace net
{
	struct PeerReplicationData
	{
		net::PeerId m_PeerId;
		Set<ecs::Entity> m_EntitiesReplicated;
		Set<ecs::Entity> m_EntitiesToCreate;
		Set<ecs::Entity> m_EntitiesToUpdate;
		Set<ecs::Entity> m_EntitiesToDestroy;
		Map<ecs::Entity, Set<TypeHash>> m_ComponentsAdded;
		Map<ecs::Entity, Set<TypeHash>> m_ComponentsRemoved;
	};

	class ReplicationHost
	{
	public:
		ReplicationHost(ecs::EntityWorld& entityWorld);

		void Initialise();
		void Shutdown();

		void Update(const GameTime& gameTime);

		void RegisterPeer(const PeerId& peerId);
		void UnregisterPeer(const PeerId& peerId);

		void StartReplicateToPeer(const PeerId& peerId, const ecs::Entity& entity);
		void StopReplicateToPeer(const PeerId& peerId, const ecs::Entity& entity);

		bool IsRegistered(const PeerId& peerId);
		bool IsReplicated(const PeerId& peerId, const ecs::Entity& entity);

	private:
		void CreateEntity(const net::PeerId& peerId, const ecs::Entity& entity);
		void DestroyEntity(const net::PeerId& peerId, const ecs::Entity& entity);

		void AddComponent(const net::PeerId& peerId, const ecs::Entity& entity, const net::ComponentEntry& entry);
		void RemoveComponent(const net::PeerId& peerId, const ecs::Entity& entity, const net::ComponentEntry& entry);
		void UpdateComponent(const net::PeerId& peerId, const ecs::Entity& entity, const net::ComponentEntry& entry);

	private:
		ecs::EntityWorld& m_EntityWorld;
		SparseArray<net::PeerId, net::PeerReplicationData> m_PeerReplicationData;
	};
}

#include "ReplicationHost.inl"