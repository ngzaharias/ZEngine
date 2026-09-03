#pragma once

#include "Core/Array.h"
#include "Core/Guid.h"
#include "Core/Map.h"
#include "ECS/ComponentId.h"
#include "ECS/ComponentLayout.h"
#include "ECS/Entity.h"
#include "ECS/EntityLayout.h"
#include "ECS/EntityPage.h"

namespace ecs
{
	/// \brief Stores the components for all entities of a single archetype spread across multiple pages.
	class EntityTable
	{
	public:
		// Allocates memory for a new page and appends it.
		auto AppendPage() -> ecs::EntityPage&;

		// De-constructs the components of all pages that match the component mask.
		void DestructAllPages(const ecs::ComponentMask& componentMask);

		// Deallocates the memory of a page and removes it.
		void RemovePage(const int32 index);
		// Deallocates the memory of the last page and removes it, and optionally de-constructs all of its components.
		void RemoveLastPage();
		// Deallocates the memory of all pages and removes them, and optionally de-constructs all components.
		void RemoveAllPages();

		// Appends the entity to the table.
		// Will create a new page for it if the last page is full.
		void AppendEntity(const ecs::Entity& entity);

		// De-constructs the components of the entity that match the component mask.
		void DestructEntity(const ecs::Entity& entity, const ecs::ComponentMask& componentMask);

		// Removes the entity from the table and optionally destructs its components.
		// Will remove the last page if its size drops to 0.
		void RemoveEntity(const ecs::Entity& entity);

		bool HasComponent(const ecs::Entity& entity, const ecs::ComponentId& componentId) const;

		auto GetComponent(const ecs::Entity& entity, const ecs::ComponentId& componentId) -> char*;

		auto TryComponent(const ecs::Entity& entity, const ecs::ComponentId& componentId) -> char*;

		auto GetEntityData(const ecs::Entity& entity) -> char*;

	public:
		// Reference id of the table for quick lookups.
		str::Guid m_TableId = {};
		// Layout of all the components in the table and if it is for alive/dead entities.
		ecs::EntityLayout m_EntityLayout = {};
		// Maps a component id to its layout in entity data.
		Map<ecs::ComponentId, ecs::ComponentLayout> m_ComponentMap = {};

		// Number of entities in the table.
		int32 m_EntityCount = 0;
		// Size in bytes of all components on a single entity.
		int32 m_EntitySize = 0;
		// Array of pages that hold the components for each entity.
		Array<ecs::EntityPage> m_EntityPages = {};
		// Maps an entity to its index within the pages.
		Map<ecs::Entity, uint16> m_EntityMap = {};
	};
}
