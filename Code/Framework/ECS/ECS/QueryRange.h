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
			return QueryIterator<TQuery>{ m_World, std::begin(m_Data), std::end(m_Data) };
		}

		auto end()
		{
			return QueryIterator<TQuery>{ m_World, std::end(m_Data), std::end(m_Data)};
		}

		ecs::EntityWorld& m_World;
		const ecs::QueryGroupB& m_Data;
	};
}