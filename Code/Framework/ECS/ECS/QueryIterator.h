#pragma once

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
		using GroupIterator = ecs::QueryGroupB::const_iterator;
		using TableIterator = ecs::EntityTable::EntityMap::const_iterator;

		QueryIterator(ecs::EntityWorld& world, const GroupIterator& groupItr, const GroupIterator& groupEnd)
			: m_World(world)
			, m_GroupItr(groupItr)
			, m_GroupEnd(groupEnd)
		{
			if (m_GroupItr != m_GroupEnd)
			{
				ecs::EntityTable& table = m_World.m_EntityStorage2.GetTable(*m_GroupItr);
				m_TableItr = table.m_EntityMap.cbegin();
				m_TableEnd = table.m_EntityMap.cend();
			}
		}

		auto operator*() -> EntityView
		{
			return EntityView(m_World, m_TableItr->first);
		}

		auto operator++() -> QueryIterator&
		{
			m_TableItr++;
			if (m_TableItr != m_TableEnd)
				return *this;

			m_GroupItr++;

			if (m_GroupItr == m_GroupEnd)
				return *this;

			ecs::EntityTable& table = m_World.m_EntityStorage2.GetTable(*m_GroupItr);
			m_TableItr = table.m_EntityMap.cbegin();
			m_TableEnd = table.m_EntityMap.cend();
			return *this;
		}

		bool operator!=(const QueryIterator& rhs) const
		{
			return m_GroupItr != rhs.m_GroupItr;
		}

		ecs::EntityWorld& m_World;
		GroupIterator m_GroupItr;
		GroupIterator m_GroupEnd;
		TableIterator m_TableItr;
		TableIterator m_TableEnd;
	};
}