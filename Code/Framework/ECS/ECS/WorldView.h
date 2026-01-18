#pragma once

#include "Core/TypeList.h"
#include "Core/Types.h"
#include "ECS/Entity.h"
#include "ECS/EntityView.h"
#include "ECS/EntityWorld.h"
#include "ECS/QueryRange.h"
#include "ECS/QueryRegistry.h"

template<typename Type>
class Array;
template<typename Type>
class Set;

namespace ecs
{
	class EntityWorld;
	class QueryRegistry;
	struct Entity;
	template<typename TQuery>
	struct QueryRange;
}

namespace ecs
{
	template <typename...>
	class WorldView_t;

	template <typename... TWrite, typename... TRead>
	class WorldView_t<TypeList<TWrite...>, TypeList<TRead...>>
	{
		friend class SystemRegistry;
		template<typename...>
		friend class WorldView_t;

	private:
		using TWriteList = TypeList<TWrite...>;
		using TReadList = TypeList<TRead...>;

	public:
		template <typename... Types>
		using Write = WorldView_t<decltype(TWriteList::template Append<Types...>()), TReadList>;
		template <typename... Types>
		using Read = WorldView_t<TWriteList, decltype(TReadList::template Append<Types...>())>;

		WorldView_t(ecs::EntityWorld& entityWorld);

		template <typename... TOthers>
		WorldView_t(const WorldView_t<TOthers...>& rhs);

		//template <typename... TOthers>
		//operator WorldView_t<TOthers...>() const;

	public:
		template<class TEntityView>
		TEntityView EntityView(const ecs::Entity& entity);

		//////////////////////////////////////////////////////////////////////////
		// Entity

		bool IsAlive(const ecs::Entity& entity) const;

		auto CreateEntity() -> ecs::Entity;

		void DestroyEntity(const ecs::Entity& entity);

		//////////////////////////////////////////////////////////////////////////
		// Component

		template<class TComponent, typename... TArgs>
		auto AddComponent(const ecs::Entity& entity, TArgs&&... args)->decltype(auto);

		template<class TComponent>
		void RemoveComponent(const ecs::Entity& entity);

		template<class TComponent>
		bool HasComponent(const ecs::Entity& entity, const bool alive = true) const;

		template<class TComponent>
		auto ReadComponent(const ecs::Entity& entity, const bool alive = true)->const TComponent&;

		template<class TComponent>
		auto WriteComponent(const ecs::Entity& entity, const bool alive = true)->TComponent&;

		//////////////////////////////////////////////////////////////////////////
		// Event

		template<class TEvent, typename... TArgs>
		auto AddEvent(TArgs&&... args)->decltype(auto);

		//////////////////////////////////////////////////////////////////////////
		// Resource

		template<class TResource>
		auto ReadResource() -> const TResource&;

		template<class TResource>
		auto WriteResource() -> TResource&;

		//////////////////////////////////////////////////////////////////////////
		// Singleton

		template<class TComponent>
		auto ReadSingleton() -> const TComponent&;

		template<class TComponent>
		auto WriteSingleton() -> TComponent&;

		//////////////////////////////////////////////////////////////////////////
		// Query

		template<class TType>
		auto Count() -> int32;

		template<class TType>
		auto HasAny() -> bool;

		template<class TEvent>
		auto Events() -> decltype(auto);

		template<class TQuery>
		auto Query() -> ecs::QueryRange<TQuery>;

	private:
		ecs::EntityWorld& m_EntityWorld;
		ecs::QueryRegistry& m_QueryRegistry;
	};

	using WorldView = ecs::WorldView_t<TypeList<>, TypeList<>>;
}

#include "WorldView.inl"