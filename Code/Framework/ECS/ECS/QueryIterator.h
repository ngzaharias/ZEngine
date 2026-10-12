#pragma once

#include "Core/TypeList.h"
#include "ECS/EntityTable.h"
#include "ECS/EntityView.h"
#include "ECS/QueryRegistry.h"

namespace ecs
{
	class EntityWorld;

	template<typename TQuery>
	struct QueryIterator
	{
		using Required = ecs::query::IncludeAccess<TQuery>::NonConst;
		using Optional = ecs::query::OptionalAccess<TQuery>::NonConst;

		using EntityView = ecs::EntityView_t<Required, Optional>;
		using TableIterator = ecs::QueryGroup::const_iterator;
		using EntityIterator = ecs::EntityTable::EntityToPage::const_iterator;

		QueryIterator(ecs::EntityWorld& entityWorld, const ecs::QueryMask& queryMask, const TableIterator& tableItr, const TableIterator& tableEnd)
			: m_EntityWorld(entityWorld)
			, m_QueryMask(queryMask)
			, m_TableItr(tableItr)
			, m_TableEnd(tableEnd)
		{
			SkipEmptyTables();
			SkipEmptyEntities();
		}

		auto operator*() -> EntityView
		{
			return EntityView(m_EntityWorld, m_EntityItr->first);
		}

		auto operator++() -> QueryIterator&
		{
			m_EntityItr++;
			if (m_EntityItr == m_EntityEnd)
			{
				m_TableItr++;
				SkipEmptyTables();
			}

			SkipEmptyEntities();
			return *this;
		}

		bool operator==(const QueryIterator& rhs) const
		{
			return m_TableItr == rhs.m_TableItr
				&& m_EntityItr == rhs.m_EntityEnd;
		}

		bool operator!=(const QueryIterator& rhs) const
		{
			return m_TableItr != rhs.m_TableItr
				|| m_EntityItr != rhs.m_EntityEnd;
		}

	private:
		bool IsValidEntity()
		{
			const ecs::Entity& entity = m_EntityItr->first;
			const ecs::ComponentMask& updatedMask = m_QueryMask.m_TableUpdatedMask;
			if (updatedMask.HasAny())
			{
				ecs::EntityTable& table = m_EntityWorld.m_EntityStorage.GetTable(*m_TableItr);
				const auto find = table.m_UpdatedMap.Find(entity);
				if (find == table.m_UpdatedMap.end())
					return false;

				if (!find->second.HasAll(updatedMask))
					return false;
			}

			const ecs::ComponentMask& externalMask = m_QueryMask.m_ExternIncludeMask;
			if (externalMask.HasAny())
			{
				for (const ecs::ComponentId componentId : externalMask)
				{
					if (!m_EntityWorld.m_EntityStorage.HasComponent(entity, componentId))
						return false;
				}
			}

			return true;
		}

		bool IsValidTable()
		{
			return m_TableItr != m_TableEnd;
		}

		void SkipEmptyEntities()
		{
			while (IsValidTable() && !IsValidEntity())
			{
				m_EntityItr++;
				if (m_EntityItr != m_EntityEnd)
					continue;

				m_TableItr++;
				SkipEmptyTables();
			}
		}

		void SkipEmptyTables()
		{
			while (IsValidTable())
			{
				ecs::EntityTable& table = m_EntityWorld.m_EntityStorage.GetTable(*m_TableItr);
				m_EntityItr = table.m_EntityMap.begin();
				m_EntityEnd = table.m_EntityMap.end();
				if (m_EntityItr != m_EntityEnd)
					return;

				m_TableItr++;
			}

			m_EntityItr = {};
			m_EntityEnd = {};
		}

	private:
		ecs::EntityWorld& m_EntityWorld;
		const ecs::QueryMask& m_QueryMask;

		TableIterator m_TableItr;
		TableIterator m_TableEnd;
		EntityIterator m_EntityItr;
		EntityIterator m_EntityEnd;
	};
}