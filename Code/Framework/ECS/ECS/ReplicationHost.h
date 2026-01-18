#pragma once

#include "Core/Delegate.h"
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

	private:
		void OnProcessMessages(const Array<const net::Message*>& messages);

		//////////////////////////////////////////////////////////////////////////
		// Entity

		void EntityCreate(const ecs::Entity& entity);

		void EntityDestroy(const ecs::Entity& entity);

		//////////////////////////////////////////////////////////////////////////
		// Component

		void ComponentAdd(const ecs::Entity& entity, const ecs::TypeComponent& entry);

		void ComponentUpdate(const ecs::Entity& entity, const ecs::TypeComponent& entry);

		void ComponentRemove(const ecs::Entity& entity, const ecs::TypeComponent& entry);

		//////////////////////////////////////////////////////////////////////////
		// Event

		void EventAdd(const ecs::TypeEvent& entry);

		void OnEventAdd(const ecs::EventAddMessage* message);

		//////////////////////////////////////////////////////////////////////////
		// Singleton

		void SingletonUpdate(const ecs::TypeSingleton& entry);

	private:
		ecs::EntityWorld& m_EntityWorld;
		DelegateCollection m_Collection;
	};
}
