#pragma once

#include "Core/Array.h"
#include "Core/Guid.h"
#include "Core/Map.h"
#include "ECS/Entity.h"
#include "ECS/EntityLayout.h"
#include "ECS/EntityTable.h"

namespace ecs
{
	class EntityBuffer;
	class QueryRegistry;
	class TypeRegistry;
}

namespace ecs
{
	/// \brief Stores all the components for all entities of all archetypes.
	class EntityStorage
	{
		enum class EChange
		{
			Created = 0,
			Destroyed,
		};

	public:
		EntityStorage(const ecs::TypeRegistry& typeRegistry);

		void FlushChanges(ecs::EntityBuffer& entityBuffer, ecs::QueryRegistry& queryRegistry);

		auto GetEntityMap() const -> const Map<ecs::Entity, int32>&;

		//////////////////////////////////////////////////////////////////////////
		// Entity

		bool IsAlive(const ecs::Entity& entity) const;

		//////////////////////////////////////////////////////////////////////////
		// Component

		template<class TComponent>
		bool HasComponent(const ecs::Entity& entity) const;

		template<class TComponent>
		auto GetComponent(const ecs::Entity& entity) -> TComponent&;

		template<class TComponent>
		auto TryComponent(const ecs::Entity& entity) -> TComponent*;

	public:
		void CreateTable(const ecs::EntityLayout& tableLayout);
		void DestroyTable(const int32 index);
		void DestroyTable(const ecs::EntityLayout& tableLayout);
		void MoveTable(const ecs::EntityLayout& sourceLayout, const ecs::EntityLayout& targetLayout);
		void OptimizeTables();

		auto GetTable(const int32 index) -> ecs::EntityTable&;
		auto GetTable(const str::Guid& tableId) -> ecs::EntityTable&;
		auto GetTable(const str::Guid& tableId) const -> const ecs::EntityTable&;
		auto GetTable(const ecs::Entity& entity) -> ecs::EntityTable&;
		auto GetTable(const ecs::Entity& entity) const -> const ecs::EntityTable&;
		auto GetTable(const ecs::EntityLayout& tableLayout) -> ecs::EntityTable&;
		auto GetOrCreateTable(const ecs::EntityLayout& tableLayout) -> ecs::EntityTable&;

		void CreateEntity(const ecs::Entity& entity, const ecs::ComponentMask& componentMask);
		void MoveEntity(const ecs::Entity& entity, const ecs::EntityLayout& sourceLayout, const ecs::EntityLayout& targetLayout);
		void UpdateEntity(const ecs::Entity& entity, const ecs::ComponentMask& componentMask);

		auto GetComponent(const ecs::Entity& entity, const ecs::ComponentId& componentId) -> char*;

	private:
		const ecs::TypeRegistry& m_TypeRegistry;

		Map<str::Guid, EChange> m_TableChanges = {};

		// Array of tables that hold the components for all archetypes.
		Array<ecs::EntityTable> m_Tables = {};
		// Maps an entity to its table index.
		// #todo: use a paged array instead of a map
		Map<ecs::Entity, int32> m_EntityMap = {};
		// Maps a guid to the table index.
		Map<str::Guid, int32> m_GuidMap = {};
		// Maps a layout to the table index.
		Map<ecs::EntityLayout, int32> m_LayoutMap = {};
	};
}

#include "EntityStorage.inl"