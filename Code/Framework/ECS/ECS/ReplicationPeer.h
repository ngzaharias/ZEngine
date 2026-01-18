#pragma once

#include "Core/Array.h"
#include "Core/Delegate.h"
#include "Core/Map.h"
#include "ECS/Entity.h"
#include "ECS/EventId.h"
#include "Network/Entity.h"

class GameTime;
class MemBuffer;

namespace ecs
{
	class EntityWorld;
	struct ComponentAddMessage;
	struct ComponentRemoveMessage;
	struct ComponentUpdateMessage;
	struct EntityCreateMessage;
	struct EntityDestroyMessage;
	struct EventAddMessage;
	struct SingletonUpdateMessage;
	struct TypeEvent;
}

namespace net
{
	struct Message;
	struct PeerId;
}

namespace ecs
{
	class ReplicationPeer
	{
	public:
		ReplicationPeer(ecs::EntityWorld& entityWorld);

		void Initialise();
		void Shutdown();

		void Update(const GameTime& gameTime);

	private:
		void OnProcessMessages(const Array<const net::Message*>& messages);

		//////////////////////////////////////////////////////////////////////////
		// Entity

		void OnEntityCreate(const ecs::EntityCreateMessage* message);

		void OnEntityDestroy(const ecs::EntityDestroyMessage* message);

		//////////////////////////////////////////////////////////////////////////
		// Component

		void OnComponentAdd(const ecs::ComponentAddMessage* message);

		void OnComponentUpdate(const ecs::ComponentUpdateMessage* message);

		void OnComponentRemove(const ecs::ComponentRemoveMessage* message);

		//////////////////////////////////////////////////////////////////////////
		// Event

		void EventAdd(const ecs::EventId typeId, const MemBuffer& buffer);

		void OnEventAdd(const ecs::EventAddMessage* message);

		//////////////////////////////////////////////////////////////////////////
		// Singleton

		void OnSingletonUpdate(const ecs::SingletonUpdateMessage* message);

	private:
		ecs::EntityWorld& m_EntityWorld;
		DelegateCollection m_Collection;

		Map<ecs::Entity, net::Entity> m_PeerToHost;
		Map<net::Entity, ecs::Entity> m_HostToPeer;
	};
}