#pragma once

#include "Core/String.h"
#include "Core/TypeInfo.h"
#include "ECS/QueryId.h"

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
		TypeId m_GlobalId = -1;
		TypeId m_LocalId = -1;
		ecs::QueryId m_AddedId = -1;
		ecs::QueryId m_UpdatedId = -1;
		ecs::QueryId m_RemovedId = -1;
		ecs::QueryId m_IncludeId = -1;
		bool m_IsPrototype = false;
		bool m_IsReplicated = false;

		using Add = void(ecs::EntityStorage&, const ecs::Entity&, const MemBuffer&);
		Add* m_Add = nullptr;

		using Update = void(ecs::EntityStorage&, const ecs::Entity&, const MemBuffer&);
		Update* m_Update = nullptr;

		using Remove = void(ecs::EntityStorage&, const ecs::Entity&);
		Remove* m_Remove = nullptr;

		using Read = void(ecs::EntityStorage&, const ecs::Entity&, MemBuffer&);
		Read* m_Read = nullptr;

		using Write = void(ecs::EntityStorage&, const ecs::Entity&, const MemBuffer&);
		Write* m_Write = nullptr;
	};
}
