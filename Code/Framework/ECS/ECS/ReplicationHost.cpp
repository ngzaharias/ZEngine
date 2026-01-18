#include "ECS/ReplicationHost.h"

#include "Core/Profiler.h"
#include "ECS/EntityWorld.h"
#include "ECS/Messages.h"
#include "ECS/TypeRegistry.h"
#include "Network/NetworkManager.h"
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

	auto& manager = m_EntityWorld.WriteResource<net::NetworkManager>();
	auto& host = manager.GetHost();

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

}

void ecs::ReplicationHost::OnProcessMessages(const Array<const net::Message*>& messages)
{
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
	auto& manager = m_EntityWorld.WriteResource<net::NetworkManager>();
	auto& host = manager.GetHost();

	auto* message = host.RequestMessage<ecs::EntityCreateMessage>(ecs::EMessage::EntityCreate);
	message->m_Entity = entity.m_Value;
	host.BroadcastMessage(message);
	host.ReleaseMessage(message);
}

void ecs::ReplicationHost::EntityDestroy(const ecs::Entity& entity)
{
	auto& manager = m_EntityWorld.WriteResource<net::NetworkManager>();
	auto& host = manager.GetHost();

	auto* message = host.RequestMessage<ecs::EntityDestroyMessage>(ecs::EMessage::EntityDestroy);
	message->m_Entity = entity.m_Value;

	host.BroadcastMessage(message);
	host.ReleaseMessage(message);
}

//////////////////////////////////////////////////////////////////////////
// Component

void ecs::ReplicationHost::ComponentAdd(const ecs::Entity& entity, const ecs::TypeComponent& entry)
{
	auto& manager = m_EntityWorld.WriteResource<net::NetworkManager>();
	auto& host = manager.GetHost();

	auto* message = host.RequestMessage<ecs::ComponentAddMessage>(ecs::EMessage::ComponentAdd);
	message->m_Entity = ToNetEntity(entity);
	message->m_TypeId = entry.m_TypeId;
	entry.m_Read(m_EntityWorld, entity, message->m_Data);

	host.BroadcastMessage(message);
	host.ReleaseMessage(message);
}

void ecs::ReplicationHost::ComponentUpdate(const ecs::Entity& entity, const ecs::TypeComponent& entry)
{
	auto& manager = m_EntityWorld.WriteResource<net::NetworkManager>();
	auto& host = manager.GetHost();

	auto* message = host.RequestMessage<ecs::ComponentUpdateMessage>(ecs::EMessage::ComponentUpdate);
	message->m_Entity = ToNetEntity(entity);
	message->m_TypeId = entry.m_TypeId;
	entry.m_Read(m_EntityWorld, entity, message->m_Data);

	host.BroadcastMessage(message);
	host.ReleaseMessage(message);
}


void ecs::ReplicationHost::ComponentRemove(const ecs::Entity& entity, const ecs::TypeComponent& entry)
{
	auto& manager = m_EntityWorld.WriteResource<net::NetworkManager>();
	auto& host = manager.GetHost();

	auto* message = host.RequestMessage<ecs::ComponentRemoveMessage>(ecs::EMessage::ComponentRemove);
	message->m_Entity = ToNetEntity(entity);
	message->m_TypeId = entry.m_TypeId;

	host.BroadcastMessage(message);
	host.ReleaseMessage(message);
}

//////////////////////////////////////////////////////////////////////////
// Event

void ecs::ReplicationHost::EventAdd(const ecs::TypeEvent& entry)
{
	auto& manager = m_EntityWorld.WriteResource<net::NetworkManager>();
	auto& host = manager.GetHost();

	auto* message = host.RequestMessage<ecs::EventAddMessage>(ecs::EMessage::EventAdd);
	message->m_TypeId = entry.m_TypeId;
	entry.m_Read(m_EntityWorld, message->m_Data);

	host.BroadcastMessage(message);
	host.ReleaseMessage(message);
}

void ecs::ReplicationHost::OnEventAdd(const ecs::EventAddMessage* message)
{
	const auto& registry = m_EntityWorld.ReadResource<ecs::TypeRegistry>();
	registry.AddEvent(m_EntityWorld, message->m_TypeId, message->m_Data);
}

//////////////////////////////////////////////////////////////////////////
// Singleton

void ecs::ReplicationHost::SingletonUpdate(const ecs::TypeSingleton& entry)
{
	auto& manager = m_EntityWorld.WriteResource<net::NetworkManager>();
	auto& host = manager.GetHost();

	auto* message = host.RequestMessage<ecs::SingletonUpdateMessage>(ecs::EMessage::SingletonUpdate);
	message->m_TypeId = entry.m_TypeId;
	entry.m_Read(m_EntityWorld, message->m_Data);

	host.BroadcastMessage(message);
	host.ReleaseMessage(message);
}