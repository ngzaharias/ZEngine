#include "ECS/EntityStorage2.h"

#include "Core/Profiler.h"
#include "ECS/EntityBuffer.h"
#include "ECS/TypeRegistry.h"

ecs::EntityStorage2::EntityStorage2(const ecs::TypeRegistry& registry)
	: m_TypeRegistry(registry)
{
}

void ecs::EntityStorage2::FlushChanges(ecs::EntityBuffer& entityBuffer)
{
	{
		PROFILE_CUSTOM("Remove dead tables.");
		const int32 count = m_Tables.GetCount();
		for (int32 i = count - 1; i >= 0; --i)
		{
			ecs::EntityTable& table = m_Tables[i];
			if (table.m_EntityLayout.m_IsDead)
				DestroyTable(i);
		}
	}

	{
		PROFILE_CUSTOM("Remove frame components.");
	}

	{
		PROFILE_CUSTOM("Move entities to tables.");
		for (auto&& [entity, changes] : entityBuffer.m_EntityChanges)
		{
			const auto find = m_EntityMap.Find(entity);
			if (find == m_EntityMap.end())
			{
				CreateEntity(entity, changes.m_Added);
			}
			else
			{
				// #edge-case: copy table layout since a new table might be created as a part of the move
				const ecs::EntityTable& table = GetTable(find->second);
				const ecs::EntityLayout source = table.m_EntityLayout;

				ecs::EntityLayout target = source;
				target.m_IncludeMask.Raise(changes.m_Added);
				target.m_IncludeMask.Clear(changes.m_Removed);
				target.m_RemovedMask.Raise(changes.m_Removed);
				target.m_IsDead |= changes.m_IsDestroy;

				MoveEntity(entity, source, target);
			}
		}
	}

	{
		PROFILE_CUSTOM("Move components to tables.");
	}


	entityBuffer.m_EntityChanges.RemoveAll();
}

bool ecs::EntityStorage2::IsAlive(const ecs::Entity& entity) const
{
	return m_EntityMap.Contains(entity);
}

void ecs::EntityStorage2::CreateTable(const ecs::EntityLayout& tableLayout)
{
	Z_PANIC(!m_LayoutMap.Contains(tableLayout), "");

	const str::Guid tableId = str::Guid::Generate();
	const int32 index = m_Tables.GetCount();
	m_GuidMap.Set(tableId, index);
	m_LayoutMap.Set(tableLayout, index);

	ecs::EntityTable& table = m_Tables.Emplace();
	table.m_TableId = tableId;
	table.m_EntityLayout = tableLayout;

	const ecs::ComponentMask componentMask = tableLayout.m_IncludeMask | tableLayout.m_RemovedMask;
	for (const ecs::ComponentId componentId : componentMask)
	{
		const ecs::TypeComponent& componentType = m_TypeRegistry.GetComponentInfo(componentId);

		ecs::ComponentLayout& layout = table.m_ComponentMap[componentId];
		layout.m_Destructor = componentType.m_Destructor;
		layout.m_Offset = table.m_EntitySize;
		layout.m_Bytes = componentType.m_Bytes;

		// do last
		table.m_EntitySize += componentType.m_Bytes;
	}

	// #todo: report new table to all queries
}

void ecs::EntityStorage2::DestroyTable(const int32 index)
{
	// #note: don't remove the table from storage, that is done separately as it is an expensive operation
	ecs::EntityTable& table = m_Tables[index];
	table.RemoveAllPages();
}

void ecs::EntityStorage2::DestroyTable(const ecs::EntityLayout& tableLayout)
{
	const int32 index = m_LayoutMap.Get(tableLayout);
	DestroyTable(index);
}

auto ecs::EntityStorage2::GetTable(const int32 index) -> ecs::EntityTable&
{
	return m_Tables[index];
}

auto ecs::EntityStorage2::GetTable(const str::Guid& tableId) -> ecs::EntityTable&
{
	const int32 index = m_GuidMap.Get(tableId);
	return m_Tables[index];
}

auto ecs::EntityStorage2::GetTable(const ecs::Entity& entity) -> ecs::EntityTable&
{
	const int32 index = m_EntityMap.Get(entity);
	return m_Tables[index];
}

auto ecs::EntityStorage2::GetTable(const ecs::EntityLayout& tableLayout) -> ecs::EntityTable&
{
	const int32 index = m_LayoutMap.Get(tableLayout);
	return m_Tables[index];
}

auto ecs::EntityStorage2::GetOrCreateTable(const ecs::EntityLayout& tableLayout) -> ecs::EntityTable&
{
	const auto find = m_LayoutMap.Find(tableLayout);
	if (find != m_LayoutMap.end())
	{
		return m_Tables[find->second];
	}
	else
	{
		CreateTable(tableLayout);
		return m_Tables.GetLast();
	}
}

void ecs::EntityStorage2::CleanupTables()
{
	for (int32 i = m_Tables.GetCount(); i > 0; --i)
	{
		ecs::EntityTable& table = m_Tables[i];
		if (table.m_EntityMap.IsEmpty())
			m_Tables.RemoveAt(i);
	}

	// #todo: fix mask map
}

void ecs::EntityStorage2::CreateEntity(const ecs::Entity& entity, const ecs::ComponentMask& componentMask)
{
	const ecs::EntityLayout tableLayout = { componentMask, {}, false };
	ecs::EntityTable& table = GetOrCreateTable(tableLayout);
	table.AppendEntity(entity);

	// #todo: different way to fetch table index
	m_EntityMap[entity] = m_GuidMap.Get(table.m_TableId);
}

void ecs::EntityStorage2::DestroyEntity(const ecs::Entity& entity)
{
	const int32 tableIndex = m_EntityMap.Get(entity);
	ecs::EntityTable& table = GetTable(tableIndex);
	table.RemoveEntity(entity, true);

	m_EntityMap.Remove(entity);
}

void ecs::EntityStorage2::MoveEntity(const ecs::Entity& entity, const ecs::EntityLayout& sourceLayout, const ecs::EntityLayout& targetLayout)
{
	// #edge-case: target table first else you'll invalidate the source table if the target needs to be created
	ecs::EntityTable& targetTable = GetOrCreateTable(targetLayout);
	ecs::EntityTable& sourceTable = GetTable(sourceLayout);

	targetTable.AppendEntity(entity);

	// move entity
	if (sourceTable.m_EntitySize > 0)
	{
		char* sourceData = sourceTable.GetEntityData(entity);
		char* targetData = targetTable.GetEntityData(entity);
		auto& sourceMap = sourceTable.m_ComponentMap;
		auto& targetMap = targetTable.m_ComponentMap;
		for (auto&& [componentId, sourceLayout] : sourceMap)
		{
			const ecs::ComponentLayout& targetLayout = targetMap.Get(componentId);
			Z_PANIC(sourceLayout.m_Bytes == targetLayout.m_Bytes, "Component size mismatch!");

			char* sourceComponent = &sourceData[sourceLayout.m_Offset];
			char* targetComponent = &targetData[targetLayout.m_Offset];
			memcpy(sourceComponent, targetComponent, sourceLayout.m_Bytes);
		}
	}

	sourceTable.RemoveEntity(entity, false);

	// #todo: different way to fetch table index
	m_EntityMap[entity] = m_GuidMap.Get(targetTable.m_TableId);
}

auto ecs::EntityStorage2::GetComponent(const ecs::Entity& entity, const ecs::ComponentId& componentId) -> char*
{
	const int32 tableIndex = m_EntityMap.Get(entity);
	ecs::EntityTable& table = GetTable(tableIndex);
	return table.GetComponent(entity, componentId);
}
