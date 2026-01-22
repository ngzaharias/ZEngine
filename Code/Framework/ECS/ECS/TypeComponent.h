#pragma once

#include "Core/String.h"
#include "ECS/ComponentId.h"

class MemBuffer;

namespace ecs
{
	class EntityBuffer;
	class EntityStorage;
	struct Entity;
}

namespace ecs
{
	struct TypeComponent
	{
		using Add = void(ecs::EntityBuffer&, const ecs::Entity&, const MemBuffer&);
		using Remove = void(ecs::EntityBuffer&, const ecs::Entity&);
		using Read = void(ecs::EntityStorage&, const ecs::Entity&, MemBuffer&);
		using Write = void(ecs::EntityStorage&, const ecs::Entity&, const MemBuffer&);

		Add* m_Add = nullptr;
		Remove* m_Remove = nullptr;
		Read* m_Read = nullptr;
		Write* m_Write = nullptr;

		str::String m_Name = {};
		ecs::ComponentId m_TypeId = -1;

		ecs::QueryId m_AddedId = -1;
		ecs::QueryId m_UpdatedId = -1;
		ecs::QueryId m_RemovedId = -1;
		ecs::QueryId m_IncludeId = -1;
		bool m_IsReplicated = false;
	};
}
