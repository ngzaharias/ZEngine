#pragma once

#include "Core/Guid.h"
#include "Core/Map.h"
#include "Core/Set.h"
#include "Core/TypeInfo.h"
#include "Core/TypeList.h"
#include "Core/TypeTraits.h"
#include "ECS/ComponentMask.h"
#include "ECS/Entity.h"
#include "ECS/QueryMask.h"
#include "ECS/QueryTypes.h"

using int32 = int32_t;

namespace ecs
{
	class EntityStorage;
	struct EntityLayout;
}

namespace ecs
{
	using QueryId = int32;
	using QueryGroup = Set<ecs::Entity>;
	using QueryGroupB = Set<str::Guid>;
	using QueryGroups = Map<ecs::QueryId, QueryGroup>;
	using QueryGroupsB = Map<ecs::QueryId, QueryGroupB>;
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

		void RegisterTable(const str::Guid& tableId, const ecs::EntityLayout& layout);
		void UnregisterTable(const str::Guid& tableId, const ecs::EntityLayout& layout);

		static ecs::QueryMasks& GetMasks();

		template<typename TQuery>
		const ecs::QueryGroup& GetGroup() const;
		const ecs::QueryGroup& GetGroup(const ecs::QueryId queryId) const;
		const ecs::QueryGroupB& GetGroupB(const ecs::QueryId queryId) const;

	private:
		static inline ecs::QueryMasks m_Masks;
		ecs::QueryGroups m_Groups = { };
		ecs::QueryGroupsB m_GroupsB = { };
	};
}

#include "QueryRegistry.inl"