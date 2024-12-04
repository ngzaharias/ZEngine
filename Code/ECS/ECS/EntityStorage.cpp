#include "ECS/EntityStorage.h"

#include "Core/Profiler.h"
#include "ECS/FrameBuffer.h"
#include "ECS/QueryRegistry.h"

bool ecs::EntityStorage::IsAlive(const ecs::Entity& entity) const
{
	return m_AliveEntities.Contains(entity);
}

void ecs::EntityStorage::FlushChanges(ecs::FrameBuffer& frameBuffer, ecs::QueryRegistry& queryRegistry)
{
	PROFILE_FUNCTION();

	const ecs::QueryMasks& queryMasks = ecs::QueryRegistry::GetMasks();

	// remove dead components
	for (ecs::IComponentStorage* storage : m_DeadComponents.GetValues())
		storage->RemoveAll();

	// remove dead entities from all queries
	for (const ecs::Entity& entity : m_DeadEntities)
	{
		for (const auto& [queryId, queryMask] : queryMasks)
		{
			ecs::QueryGroup& queryGroup = queryRegistry.m_Groups[queryId];
			queryGroup.Remove(entity);
		}

		frameBuffer.m_HandlesRecycled.Append(entity);
		frameBuffer.m_EntityChanges.Remove(entity);
	}
	m_DeadEntities.RemoveAll();

	// clear any queries that refresh each frame
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

	// move added components from buffer -> alive
	for (auto&& [componentId, fStorage] : frameBuffer.m_Components)
	{
		ecs::IComponentStorage* eStorage = m_AliveComponents.Get(componentId);
		fStorage->Move(*eStorage);
	}

	// updates queries
	// inserts new entities
	// moves components from alive -> dead
	for (auto&& [entity, changes] : frameBuffer.m_EntityChanges)
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
	frameBuffer.m_EntityChanges.RemoveAll();
}
