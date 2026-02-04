#pragma once

#include "Core/MemBuffer.h"
#include "ECS/ComponentId.h"
#include "ECS/EventId.h"
#include "ECS/SingletonId.h"
#include "Network/Entity.h"
#include "Network/Message.h"
#include "Network/MessageEnum.h"

namespace ecs
{
	enum EMessage
	{
		EntityCreate = net::EMessage::ECS_First,
		EntityDestroy,

		ComponentAdd,
		ComponentRemove,
		ComponentUpdate,

		EventAdd,
	};

	//////////////////////////////////////////////////////////////////////////
	// Entity

	struct EntityCreateMessage final : public net::Message
	{
		net::Entity m_Entity = {};
	};

	struct EntityDestroyMessage final : public net::Message
	{
		net::Entity m_Entity = {};
	};

	//////////////////////////////////////////////////////////////////////////
	// Component

	struct ComponentAddMessage final : public net::Message
	{
		ecs::ComponentId m_TypeId = -1;
		net::Entity m_Entity = {};
		MemBuffer m_Data = {};
	};

	struct ComponentUpdateMessage final : public net::Message
	{
		ecs::ComponentId m_TypeId = -1;
		net::Entity m_Entity = {};
		MemBuffer m_Data = {};
	};

	struct ComponentRemoveMessage final : public net::Message
	{
		ecs::ComponentId m_TypeId = -1;
		net::Entity m_Entity = {};
	};

	//////////////////////////////////////////////////////////////////////////
	// Event

	struct EventAddMessage final : public net::Message
	{
		ecs::EventId m_TypeId = -1;
		MemBuffer m_Data = {};
	};
}