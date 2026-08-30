#include "ECS/EntityTable.h"

#include "Core/Profiler.h"

namespace
{
	constexpr int32 s_EntitiesPerPage = 64;

	int32 ToOffset(const int32 entityIndex, const int32 entitySize)
	{
		const int32 localIndex = entityIndex & (s_EntitiesPerPage - 1);
		return localIndex * entitySize;
	}

	int32 ToPage(const int32 entityIndex)
	{
		return entityIndex / s_EntitiesPerPage;
	}
}

ecs::EntityTable::~EntityTable()
{
	RemoveAllPages();
}

auto ecs::EntityTable::AppendPage() -> ecs::EntityPage&
{
	EntityPage& entityPage = m_EntityPages.Emplace();
	entityPage.m_Data = new char[m_EntitySize * s_EntitiesPerPage];
	return entityPage;
}

void ecs::EntityTable::RemovePage(const int32 index)
{
	// destruct all components of the same type at the same time
	EntityPage& entityPage = m_EntityPages[index];
	const int32 componentCount = m_ComponentMap.GetCount();
	for (int32 componentIndex = 0; componentIndex < componentCount; ++componentIndex)
	{
		const ComponentLayout& componentLayout = m_ComponentMap.Get(componentIndex);
		if (!componentLayout.m_Destructor)
			continue;

		for (int32 entityIndex = 0; entityIndex < m_EntityCount; ++entityIndex)
		{
			const int32 offset = m_EntitySize * componentIndex;
			componentLayout.m_Destructor(entityPage.m_Data + offset);
		}
	}

	delete[] entityPage.m_Data;
	m_EntityPages.RemoveAt(index);
}

void ecs::EntityTable::RemoveAllPages()
{
	// destruct all components of the same type at the same time
	for (auto&& [componentIndex, componentLayout] : m_ComponentMap)
	{
		if (!componentLayout.m_Destructor)
			continue;

		for (EntityPage& entityPage : m_EntityPages)
		{
			for (int32 entityIndex = 0; entityIndex < m_EntityCount; ++entityIndex)
			{
				const int32 offset = m_EntitySize * componentIndex;
				componentLayout.m_Destructor(entityPage.m_Data + offset);
			}
		}
	}

	for (EntityPage& entityPage : m_EntityPages)
		delete[] entityPage.m_Data;
	m_EntityPages.RemoveAll();
}

void ecs::EntityTable::RemoveLastPage()
{
	RemovePage(m_EntityPages.GetCount() - 1);
}

void ecs::EntityTable::AppendEntity(const ecs::Entity& entity)
{
	const int32 entityIndex = m_EntityCount;
	m_EntityMap.Set(entity, entityIndex);

	if (m_EntitySize > 0)
	{
		const int32 pageIndex = entityIndex / s_EntitiesPerPage;
		if (pageIndex >= m_EntityPages.GetCount())
			AppendPage();
	}

	// increment last
	m_EntityCount++;
}

void ecs::EntityTable::RemoveEntity(const ecs::Entity& entity, const bool destruct)
{
	if (m_EntitySize > 0)
	{
		const int32 entityIndexA = m_EntityMap.Get(entity);
		const int32 pageIndexA = ToPage(entityIndexA);
		const int32 entityOffsetA = ToOffset(entityIndexA, m_EntitySize);
		ecs::EntityPage& entityPageA = m_EntityPages[pageIndexA];

		// destruct its components
		if (destruct)
		{
			const int32 componentCount = m_ComponentMap.GetCount();
			for (int32 componentIndex = 0; componentIndex < componentCount; ++componentIndex)
			{
				const ComponentLayout& componentLayout = m_ComponentMap.Get(componentIndex);
				if (!componentLayout.m_Destructor)
					continue;

				const int32 componentOffset = m_EntitySize * componentIndex;
				componentLayout.m_Destructor(entityPageA.m_Data + entityOffsetA + componentOffset);
			}
		}

		// swap the last entity in the table with this entity so that the data is contiguous
		const int32 entityIndexB = m_EntityCount - 1;
		if (entityIndexA != entityIndexB)
		{
			const int32 pageIndexB = ToPage(entityIndexB);
			const int32 entityOffsetB = ToOffset(entityIndexB, m_EntitySize);

			ecs::EntityPage& entityPageB = m_EntityPages[pageIndexB];
			char* entityDataA = &entityPageA.m_Data[entityOffsetA];
			char* entityDataB = &entityPageB.m_Data[entityOffsetB];
			memcpy(entityDataB, entityDataA, m_EntitySize);
		}
	}

	m_EntityMap.Remove(entity);
	m_EntityCount--;

	if (m_EntitySize > 0)
	{
		// pop the page if there are no entities left in it
		const int32 pagesNeeded = m_EntityCount / s_EntitiesPerPage;
		if (pagesNeeded < m_EntityPages.GetCount())
			RemoveLastPage();
	}
}

bool ecs::EntityTable::HasComponent(const ecs::Entity& /*entity*/, const ecs::ComponentId& componentId) const
{
	return m_ComponentMap.Contains(componentId);
}

auto ecs::EntityTable::GetComponent(const ecs::Entity& entity, const ecs::ComponentId& componentId) -> char*
{
	const ecs::ComponentLayout& componentLayout = m_ComponentMap.Get(componentId);
	char* entityData = GetEntityData(entity);
	return &entityData[componentLayout.m_Offset];
}

auto ecs::EntityTable::TryComponent(const ecs::Entity& entity, const ecs::ComponentId& componentId) -> char*
{
	const auto find = m_ComponentMap.Find(componentId);
	if (find == m_ComponentMap.end())
		return nullptr;

	const ecs::ComponentLayout& componentLayout = find->second;
	char* entityData = GetEntityData(entity);
	return &entityData[componentLayout.m_Offset];
}

auto ecs::EntityTable::GetEntityData(const ecs::Entity& entity) -> char*
{
	const int32 entityIndex = m_EntityMap.Get(entity);
	const int32 pageIndex = ToPage(entityIndex);
	const int32 entityOffset = ToOffset(entityIndex, m_EntitySize);
	ecs::EntityPage& entityPage = m_EntityPages[pageIndex];
	return &entityPage.m_Data[entityOffset];
}