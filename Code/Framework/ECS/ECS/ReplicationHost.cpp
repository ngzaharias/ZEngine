#include "ECS/ReplicationHost.h"

#include "Core/Algorithms.h"
#include "Core/Profiler.h"
#include "ECS/EntityWorld.h"
#include "ECS/Messages.h"
#include "ECS/ReplicationComponent.h"
#include "ECS/TypeRegistry.h"
#include "Network/Host.h"

namespace
{
	net::Entity ToNetEntity(const ecs::Entity& entity)
	{
		return net::Entity(entity.m_Value);
	}
}

ecs::ReplicationHost::ReplicationHost(ecs::EntityWorld& entityWorld)
	: m_EntityWorld(entityWorld)
{
}

void ecs::ReplicationHost::Initialise()
{
	PROFILE_FUNCTION();

	auto& host = m_EntityWorld.WriteResource<net::Host>();
	m_Collection =
	{
		host.m_OnProcessMessages.Connect(*this, &ecs::ReplicationHost::OnProcessMessages),
	};
}

void ecs::ReplicationHost::Shutdown()
{
	PROFILE_FUNCTION();
}

void ecs::ReplicationHost::Update(const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	ProcessEntities();
	ProcessEvents();
}

void ecs::ReplicationHost::StartReplicate(const ecs::Entity& entity)
{
	Z_PANIC(!IsReplicated(entity), "Trying to start replication of an entity that is already replicated!");

	m_ReplicationData.m_EntitiesToCreate.Add(entity);
}

void ecs::ReplicationHost::StopReplicate(const ecs::Entity& entity)
{
	Z_PANIC(IsReplicated(entity), "Trying to stop replication of an entity that isn't replicated!");

	m_ReplicationData.m_EntitiesToDestroy.Add(entity);
}

bool ecs::ReplicationHost::IsReplicated(const ecs::Entity& entity)
{
	return m_ReplicationData.m_EntitiesReplicated.Contains(entity);
}

void ecs::ReplicationHost::ProcessEntities()
{
	PROFILE_FUNCTION();

	const auto& registry = m_EntityWorld.ReadResource<ecs::TypeRegistry>();
	const auto& queries = m_EntityWorld.m_QueryRegistry;

	// process first so we don't update an entity that is being destroyed
	for (const ecs::Entity& entity : m_ReplicationData.m_EntitiesToDestroy)
	{
		EntityDestroy(entity);
	}

	// process second so that the entities created this frame aren't double updated
	for (ecs::ComponentId typeId = 0; typeId < ecs::COMPONENTS_MAX; ++typeId)
	{
		const ecs::TypeComponent* entry = registry.TryTypeComponent(typeId);
		if (!entry || !entry->m_IsReplicated)
			continue;

		Set<ecs::Entity> toAdd, toUpdate, toRemove;
		const Set<ecs::Entity>& replicated = m_ReplicationData.m_EntitiesReplicated;
		enumerate::Intersection(replicated, queries.GetGroup(entry->m_AddedId), toAdd);
		enumerate::Intersection(replicated, queries.GetGroup(entry->m_UpdatedId), toUpdate);
		enumerate::Intersection(replicated, queries.GetGroup(entry->m_RemovedId), toRemove);

		// #note: this only handles component AFTER an entity was marked for replication
		for (const ecs::Entity& entity : toRemove)
			ComponentRemove(entity, *entry);
		for (const ecs::Entity& entity : toUpdate)
			ComponentUpdate(entity, *entry);
		for (const ecs::Entity& entity : toAdd)
			ComponentAdd(entity, *entry);
	}

	// process last so that the entity is added to the update list for the next frame
	for (const ecs::Entity& entity : m_ReplicationData.m_EntitiesToCreate)
	{
		EntityCreate(entity);

		const ecs::ComponentMask& componentMask = m_EntityWorld.GetComponentMask(entity);
		for (ecs::ComponentId typeId = 0; typeId < ecs::COMPONENTS_MAX; ++typeId)
		{
			if (!componentMask.Has(typeId))
				continue;

			const ecs::TypeComponent& entry = registry.GetTypeComponent(typeId);
			if (!entry.m_IsReplicated)
				continue;

			ComponentAdd(entity, entry);
		}
	}

	m_ReplicationData.m_EntitiesToCreate.RemoveAll();
	m_ReplicationData.m_EntitiesToDestroy.RemoveAll();
}

void ecs::ReplicationHost::ProcessEvents()
{
	PROFILE_FUNCTION();

	MemBuffer data;
	const auto& registry = m_EntityWorld.ReadResource<ecs::TypeRegistry>();
	const auto& storage = m_EntityWorld.m_EventStorage;

	// #hack: we iterate the keys of the remote buffer but fetch from the local buffer
	for (const ecs::EventId& typeId : storage.m_BufferRemoteCurr.GetAll().GetKeys())
	{
		const ecs::IEventContainer& container = storage.m_BufferLocalCurr.GetAt(typeId);
		const int32 count = container.GetCount();
		for (int32 i = 0; i < count; ++i)
		{
			data.Reset();
			container.ReadAt(data, i);
			EventAdd(typeId, data);
		}
	}
}

void ecs::ReplicationHost::OnProcessMessages(const Array<const net::Message*>& messages)
{
	PROFILE_FUNCTION();

	for (const net::Message* message : messages)
	{
		switch (static_cast<ecs::EMessage>(message->m_Type))
		{
		case ecs::EMessage::EntityCreate:
		case ecs::EMessage::EntityDestroy:
		case ecs::EMessage::ComponentAdd:
		case ecs::EMessage::ComponentRemove:
		case ecs::EMessage::ComponentUpdate:
		{
			Z_PANIC(false, "Unsupported!");
		} break;

		case ecs::EMessage::EventAdd:
			OnEventAdd(static_cast<const ecs::EventAddMessage*>(message));
			break;
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// Entity

void ecs::ReplicationHost::EntityCreate(const ecs::Entity& entity)
{
	m_ReplicationData.m_EntitiesReplicated.Add(entity);
	m_EntityWorld.AddComponent<ecs::ReplicationComponent>(entity);

	auto& host = m_EntityWorld.WriteResource<net::Host>();
	auto* message = host.RequestMessage<ecs::EntityCreateMessage>(ecs::EMessage::EntityCreate);
	message->m_Entity = entity.m_Value;

	host.BroadcastMessage(message);
	host.ReleaseMessage(message);
}

void ecs::ReplicationHost::EntityDestroy(const ecs::Entity& entity)
{
	m_ReplicationData.m_EntitiesReplicated.Remove(entity);
	m_EntityWorld.RemoveComponent<ecs::ReplicationComponent>(entity);

	auto& host = m_EntityWorld.WriteResource<net::Host>();
	auto* message = host.RequestMessage<ecs::EntityDestroyMessage>(ecs::EMessage::EntityDestroy);
	message->m_Entity = entity.m_Value;

	host.BroadcastMessage(message);
	host.ReleaseMessage(message);
}

//////////////////////////////////////////////////////////////////////////
// Component

void ecs::ReplicationHost::ComponentAdd(const ecs::Entity& entity, const ecs::TypeComponent& entry)
{
	auto& host = m_EntityWorld.WriteResource<net::Host>();
	auto* message = host.RequestMessage<ecs::ComponentAddMessage>(ecs::EMessage::ComponentAdd);
	message->m_Entity = ToNetEntity(entity);
	message->m_TypeId = entry.m_TypeId;
	entry.m_Read(m_EntityWorld, entity, message->m_Data);

	host.BroadcastMessage(message);
	host.ReleaseMessage(message);
}

void ecs::ReplicationHost::ComponentUpdate(const ecs::Entity& entity, const ecs::TypeComponent& entry)
{
	auto& host = m_EntityWorld.WriteResource<net::Host>();
	auto* message = host.RequestMessage<ecs::ComponentUpdateMessage>(ecs::EMessage::ComponentUpdate);
	message->m_Entity = ToNetEntity(entity);
	message->m_TypeId = entry.m_TypeId;
	entry.m_Read(m_EntityWorld, entity, message->m_Data);

	host.BroadcastMessage(message);
	host.ReleaseMessage(message);
}


void ecs::ReplicationHost::ComponentRemove(const ecs::Entity& entity, const ecs::TypeComponent& entry)
{
	auto& host = m_EntityWorld.WriteResource<net::Host>();
	auto* message = host.RequestMessage<ecs::ComponentRemoveMessage>(ecs::EMessage::ComponentRemove);
	message->m_Entity = ToNetEntity(entity);
	message->m_TypeId = entry.m_TypeId;

	host.BroadcastMessage(message);
	host.ReleaseMessage(message);
}

//////////////////////////////////////////////////////////////////////////
// Event

void ecs::ReplicationHost::EventAdd(const ecs::EventId typeId, const MemBuffer& data)
{
	auto& host = m_EntityWorld.WriteResource<net::Host>();
	auto* message = host.RequestMessage<ecs::EventAddMessage>(ecs::EMessage::EventAdd);
	message->m_TypeId = typeId;
	message->m_Data.Write(data);

	host.BroadcastMessage(message);
	host.ReleaseMessage(message);
}

void ecs::ReplicationHost::OnEventAdd(const ecs::EventAddMessage* message)
{
	const auto& registry = m_EntityWorld.ReadResource<ecs::TypeRegistry>();
	ecs::EventStorage& storage = m_EntityWorld.m_EventStorage;
	ecs::EventBuffer& buffer = storage.m_BufferRemoteCurr;

	MemBuffer data;
	message->m_Data.Read(data);
	registry.AddEvent(buffer, message->m_TypeId, data);
}

//////////////////////////////////////////////////////////////////////////
// Singleton

void ecs::ReplicationHost::SingletonUpdate(const ecs::TypeSingleton& entry)
{
	auto& host = m_EntityWorld.WriteResource<net::Host>();
	auto* message = host.RequestMessage<ecs::SingletonUpdateMessage>(ecs::EMessage::SingletonUpdate);
	message->m_TypeId = entry.m_TypeId;
	entry.m_Read(m_EntityWorld, message->m_Data);

	host.BroadcastMessage(message);
	host.ReleaseMessage(message);
}