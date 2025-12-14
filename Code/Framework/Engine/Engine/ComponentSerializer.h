#pragma once

#include "Core/Array.h"
#include "Core/Map.h"
#include "Core/SparseArray.h"
#include "Core/TypeInfo.h"
#include "ECS/Component.h"
#include "ECS/EntityWorld.h"

class MemBuffer;

namespace ecs
{
	class EntityWorld;
	struct Entity;
}

namespace net
{
	struct ComponentEntry
	{
		using Add = void(ecs::EntityWorld&, const ecs::Entity&, const MemBuffer&);
		using Remove = void(ecs::EntityWorld&, const ecs::Entity&);
		using Read = void(ecs::EntityWorld&, const ecs::Entity&, const MemBuffer&);
		using Write = void(ecs::EntityWorld&, const ecs::Entity&, MemBuffer&);

		Add*    m_Add = nullptr;
		Remove* m_Remove = nullptr;
		Read*   m_Read = nullptr;
		Write*  m_Write = nullptr;

		ecs::QueryId m_AddedId = -1;
		ecs::QueryId m_RemovedId = -1;
		ecs::QueryId m_UpdatedId = -1;
		ecs::ComponentId m_ComponentId = -1;
	};

	struct ComponentTag { };

	class ComponentSerializer
	{
	public:
		template<typename TComponent, typename TSerializer>
		void RegisterComponent();

	public:
		Array<net::ComponentEntry> m_Entries;
		Map<TypeId, ecs::ComponentId> m_TypeIdToLocal;
		Map<ecs::ComponentId, ecs::ComponentId> m_WorldToLocal;
	};
}

#include "ComponentSerializer.inl"