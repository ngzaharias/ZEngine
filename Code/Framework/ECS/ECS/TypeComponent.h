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

		using HasSolo = bool(ecs::EntityStorage&, const ecs::Entity&);
		HasSolo* m_HasSolo = nullptr;

		using AddSolo = void(ecs::EntityStorage&, const ecs::Entity&);
		AddSolo* m_AddSolo = nullptr;
		using AddData = void(ecs::EntityStorage&, const ecs::Entity&, const MemBuffer&);
		AddData* m_AddData = nullptr;

		using UpdateSolo = void(ecs::EntityStorage&, const ecs::Entity&);
		UpdateSolo* m_UpdateSolo = nullptr;
		using UpdateData = void(ecs::EntityStorage&, const ecs::Entity&, const MemBuffer&);
		UpdateData* m_UpdateData = nullptr;

		using RemoveSolo = void(ecs::EntityStorage&, const ecs::Entity&);
		RemoveSolo* m_RemoveSolo = nullptr;

		using ReadData = void(ecs::EntityStorage&, const ecs::Entity&, MemBuffer&);
		ReadData* m_ReadData = nullptr;

		using WriteData = void(ecs::EntityStorage&, const ecs::Entity&, const MemBuffer&);
		WriteData* m_WriteData = nullptr;
	};
}
