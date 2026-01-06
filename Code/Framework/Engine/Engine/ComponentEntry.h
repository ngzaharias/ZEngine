#pragma once

#include "ECS/ComponentId.h"
#include "ECS/QueryId.h"

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
		using Read = void(ecs::EntityWorld&, const ecs::Entity&, MemBuffer&);
		using Write = void(ecs::EntityWorld&, const ecs::Entity&, const MemBuffer&);

		ecs::ComponentId m_ComponentId = -1;
		ecs::QueryId m_AddedId = -1;
		ecs::QueryId m_RemovedId = -1;
		ecs::QueryId m_UpdatedId = -1;

		Add* m_Add = nullptr;
		Remove* m_Remove = nullptr;
		Read* m_Read = nullptr;
		Write* m_Write = nullptr;
	};
}