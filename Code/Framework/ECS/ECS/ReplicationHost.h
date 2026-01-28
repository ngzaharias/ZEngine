#pragma once

#include "Core/Delegate.h"
#include "Core/Map.h"
#include "Core/Set.h"
#include "Core/SparseArray.h"
#include "Core/TypeInfo.h"
#include "ECS/Entity.h"
#include "ECS/EventId.h"
#include "Network/PeerId.h"

class GameTime;
class MemBuffer;

namespace ecs
{
	class EntityWorld;
	struct EventAddMessage;
	struct TypeComponent;
	struct TypeEvent;
	struct TypeSingleton;
}

namespace net
{
	struct Message;
}

namespace ecs
{
	struct ReplicationData
	{
		net::PeerId m_PeerId = {};
		Set<ecs::Entity> m_ToCreate;
		Set<ecs::Entity> m_ToUpdate;
		Set<ecs::Entity> m_ToDestroy;
		Set<ecs::Entity> m_Replicated;
	};

	class ReplicationHost
	{
	public:
		using ReplicationMap = Map<net::PeerId, ReplicationData>;

	public:
		ReplicationHost(ecs::EntityWorld& entityWorld);

		void Initialise();
		void Shutdown();

		void Update(const GameTime& gameTime);

		void RegisterPeer(const net::PeerId& peerId);
		void UnregisterPeer(const net::PeerId& peerId);

		void StartReplicate(const net::PeerId& peerId, const ecs::Entity& entity);
		void StopReplicate(const net::PeerId& peerId, const ecs::Entity& entity);

		bool IsRegistered(const net::PeerId& peerId);
		bool IsReplicated(const net::PeerId& peerId, const ecs::Entity& entity);

		const ReplicationMap& GetReplicationMap() const { return m_ReplicationMap; }

	private:
		void ProcessEntities();
		void ProcessEvents();

		void OnPeerConnected(const net::PeerId& peerId);
		void OnPeerDisconnected(const net::PeerId& peerId);
		void OnProcessMessages(const Array<const net::Message*>& messages);

		//////////////////////////////////////////////////////////////////////////
		// Entity
		void EntityCreate(const net::PeerId& peerId, const ecs::Entity& entity);
		void EntityDestroy(const net::PeerId& peerId, const ecs::Entity& entity);

		//////////////////////////////////////////////////////////////////////////
		// Component
		void ComponentAdd(const net::PeerId& peerId, const ecs::Entity& entity, const ecs::TypeComponent& entry);
		void ComponentUpdate(const net::PeerId& peerId, const ecs::Entity& entity, const ecs::TypeComponent& entry);
		void ComponentRemove(const net::PeerId& peerId, const ecs::Entity& entity, const ecs::TypeComponent& entry);

		//////////////////////////////////////////////////////////////////////////
		// Event
		void EventAdd(const net::PeerId& peerId, const ecs::EventId typeId, const MemBuffer& data);
		void OnEventAdd(const ecs::EventAddMessage* message);

		//////////////////////////////////////////////////////////////////////////
		// Singleton
		void SingletonUpdate(const net::PeerId& peerId, const ecs::TypeSingleton& entry);

	private:
		ecs::EntityWorld& m_EntityWorld;

		DelegateCollection m_Collection;
		ReplicationMap m_ReplicationMap;
	};
}
