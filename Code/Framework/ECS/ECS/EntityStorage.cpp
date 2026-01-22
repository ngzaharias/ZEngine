#include "ECS/EntityStorage.h"

#include "Core/Profiler.h"
#include "ECS/QueryRegistry.h"

ecs::EntityStorage::EntityStorage()
	: m_MainBuffer(ecs::Entity::MainId)
	, m_SyncBuffer(ecs::Entity::SyncId)
{
}

bool ecs::EntityStorage::IsAlive(const ecs::Entity& entity) const
{
	return m_AliveEntities.Contains(entity);
}

void ecs::EntityStorage::FlushChanges(ecs::QueryRegistry& queryRegistry)
{
	PROFILE_FUNCTION();

	const ecs::QueryMasks& queryMasks = ecs::QueryRegistry::GetMasks();

	{
		PROFILE_CUSTOM("Remove dead components.");
		for (ecs::IComponentStorage* storage : m_DeadComponents.GetValues())
			storage->RemoveAll();
	}

	{
		PROFILE_CUSTOM("Remove dead entities from queries.");
		for (const ecs::Entity& entity : m_DeadEntities)
		{
			for (const auto& [queryId, queryMask] : queryMasks)
			{
				ecs::QueryGroup& queryGroup = queryRegistry.m_Groups[queryId];
				queryGroup.Remove(entity);
			}

			m_MainBuffer.m_HandlesRecycled.Append(entity);
			m_MainBuffer.m_EntityChanges.Remove(entity);
		}
		m_DeadEntities.RemoveAll();
	}

	{
		PROFILE_CUSTOM("Clear any queries that refresh each frame.");
		for (const auto& [queryId, queryMask] : queryMasks)
		{
			ecs::QueryGroup& queryGroup = queryRegistry.m_Groups[queryId];

			const bool isSingleFrameQuery =
				queryMask.m_AddedMask.HasAny() ||
				queryMask.m_RemovedMask.HasAny() ||
				queryMask.m_UpdatedMask.HasAny();

			if (isSingleFrameQuery)
				queryGroup.RemoveAll();
		}
	}

	{
		PROFILE_CUSTOM("Move added components from buffer -> alive.");
		for (auto&& [componentId, fStorage] : m_MainBuffer.m_AddedComponents)
		{
			ecs::IComponentStorage* eStorage = m_AliveComponents.Get(componentId);
			fStorage->Move(*eStorage);
			fStorage->RemoveAll();
		}
	}

	{
		PROFILE_CUSTOM("Update queries.");

		// updates queries
		// inserts new entities
		// moves components from alive -> dead
		for (auto&& [entity, changes] : m_MainBuffer.m_EntityChanges)
		{
			ecs::ComponentMask& componentMask = m_AliveEntities[entity];
			componentMask |= changes.m_Added;

			// mark all components as removed
			if (changes.m_IsDestroy)
				changes.m_Removed |= componentMask;

			// move components from alive -> dead
			// update the component mask
			if (changes.m_Removed.HasAny())
			{
				for (auto&& [componentId, aStorage] : m_AliveComponents)
				{
					if (!changes.m_Removed.Has(componentId))
						continue;

					ecs::IComponentStorage* dStorage = m_DeadComponents.Get(componentId);
					aStorage->Move(entity, *dStorage);
				}

				componentMask &= ~changes.m_Removed;
				changes.m_Added &= ~changes.m_Removed;
				changes.m_Updated &= ~changes.m_Removed;
			}

			// update queries
			for (const auto& [queryId, queryMask] : queryMasks)
			{
				ecs::QueryGroup& queryGroup = queryRegistry.m_Groups[queryId];

				const bool hasAllAdded = changes.m_Added.HasAll(queryMask.m_AddedMask);
				const bool hasAllRemoved = changes.m_Removed.HasAll(queryMask.m_RemovedMask);
				const bool hasAllUpdated = changes.m_Updated.HasAll(queryMask.m_UpdatedMask);

				const bool hasAllIncludes = componentMask.HasAll(queryMask.m_IncludeMask);
				const bool hasNoneExcludes = componentMask.HasNone(queryMask.m_ExcludeMask);

				if (hasAllAdded && hasAllRemoved && hasAllUpdated && hasAllIncludes && hasNoneExcludes)
				{
					queryGroup.Add(entity);
				}
				else
				{
					queryGroup.Remove(entity);
				}
			}

			// move entity from alive -> dead
			if (changes.m_IsDestroy)
			{
				m_AliveEntities.Remove(entity);
				m_DeadEntities.Append(entity);
			}
		}
		m_MainBuffer.m_EntityChanges.RemoveAll();
	}
}
