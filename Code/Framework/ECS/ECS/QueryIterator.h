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
		using Updated = ecs::query::UpdatedAccess<TQuery>::NonConst;
		using Required = ecs::query::IncludeAccess<TQuery>::NonConst;
		using Optional = ecs::query::OptionalAccess<TQuery>::NonConst;

		using EntityView = ecs::EntityView_t<Required, Optional>;
		using TableIterator = ecs::QueryGroup::const_iterator;
		using EntityIterator = ecs::EntityTable::EntityToPage::const_iterator;

		QueryIterator(ecs::EntityWorld& world, const TableIterator& tableItr, const TableIterator& tableEnd)
			: m_World(world)
			, m_Updated(ecs::ToComponentMask(Updated{}))
			, m_TableItr(tableItr)
			, m_TableEnd(tableEnd)
		{
			SkipEmptyTables();
			SkipEmptyEntities();
		}

		auto operator*() -> EntityView
		{
			return EntityView(m_World, m_EntityItr->first);
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
			if (m_Updated.HasNone())
				return true;

			ecs::EntityTable& table = m_World.m_EntityStorage.GetTable(*m_TableItr);
			const auto find = table.m_UpdatedMap.Find(m_EntityItr->first);
			if (find == table.m_UpdatedMap.end())
				return false;
			
			return find->second.HasAll(m_Updated);
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
				ecs::EntityTable& table = m_World.m_EntityStorage.GetTable(*m_TableItr);
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
		ecs::EntityWorld& m_World;

		ecs::ComponentMask m_Updated;
		TableIterator m_TableItr;
		TableIterator m_TableEnd;
		EntityIterator m_EntityItr;
		EntityIterator m_EntityEnd;
	};
}