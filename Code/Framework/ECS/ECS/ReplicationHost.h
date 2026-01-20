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
		Set<ecs::Entity> m_EntitiesToCreate;
		Set<ecs::Entity> m_EntitiesToUpdate;
		Set<ecs::Entity> m_EntitiesToDestroy;
		Set<ecs::Entity> m_EntitiesReplicated;
	};

	class ReplicationHost
	{
	public:
		ReplicationHost(ecs::EntityWorld& entityWorld);

		void Initialise();
		void Shutdown();

		void Update(const GameTime& gameTime);

		void StartReplicate(const ecs::Entity& entity);
		void StopReplicate(const ecs::Entity& entity);
		bool IsReplicated(const ecs::Entity& entity);

	private:
		void ProcessEntities();
		void ProcessEvents();

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

		void EventAdd(const ecs::EventId typeId, const MemBuffer& data);

		void OnEventAdd(const ecs::EventAddMessage* message);

		//////////////////////////////////////////////////////////////////////////
		// Singleton

		void SingletonUpdate(const ecs::TypeSingleton& entry);

	private:
		ecs::EntityWorld& m_EntityWorld;

		DelegateCollection m_Collection;
		ReplicationData m_ReplicationData;
	};
}
