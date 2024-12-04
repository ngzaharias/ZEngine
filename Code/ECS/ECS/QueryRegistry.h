#pragma once

#include "Core/Map.h"
#include "Core/Set.h"
#include "ECS/Entity.h"
#include "ECS/QueryMask.h"

using int32 = int32_t;

namespace ecs
{
	class EntityStorage;

	using QueryId = int32;
	using QueryGroup = Set<ecs::Entity>;
	using QueryGroups = Map<ecs::QueryId, QueryGroup>;
	using QueryMasks = Map<ecs::QueryId, ecs::QueryMask>;

	template<class TQuery>
	class QueryProxy final
	{
	public:
		virtual ~QueryProxy() { m_QueryId; }
		static inline ecs::QueryId Id();
		static const ecs::QueryId m_QueryId;
	};

	class QueryRegistry final
	{
		friend class EntityStorage;

	public:
		void Initialise();

		static ecs::QueryMasks& GetMasks();

		template<typename TQuery>
		const ecs::QueryGroup& GetGroup() const;
		const ecs::QueryGroup& GetGroup(const ecs::QueryId queryId) const;

	private:
		static inline ecs::QueryMasks m_Masks;
		ecs::QueryGroups m_Groups = { };
	};
}

#include "QueryRegistry.inl"