#pragma once

#include "ECS/QueryIterator.h"
#include "ECS/QueryRegistry.h"

namespace ecs
{
	class EntityWorld;
}

namespace ecs
{
	template<typename TQuery>
	struct QueryRange
	{
		operator const ecs::QueryGroupB& () const { return m_Data; }

		static int32 Count(ecs::EntityWorld& world, const ecs::QueryGroupB& data)
		{
			int32 count = 0;
			for (const str::Guid& tableId : data)
			{
				const ecs::EntityTable& table = world.m_EntityStorage2.GetTable(tableId);
				count += table.m_EntityCount;
			}
			return count;
		}

		static bool HasAny(ecs::EntityWorld& world, const ecs::QueryGroupB& data)
		{
			for (const str::Guid& tableId : data)
			{
				const ecs::EntityTable& table = world.m_EntityStorage2.GetTable(tableId);
				if (table.m_EntityCount > 0)
					return true;
			}
			return false;
		}

		auto begin()
		{
			return QueryIterator<TQuery>{ std::begin(m_Data), m_World };
		}

		auto end()
		{
			return QueryIterator<TQuery>{ std::end(m_Data), m_World };
		}

		ecs::EntityWorld& m_World;
		const ecs::QueryGroup& m_Data;
	};
}