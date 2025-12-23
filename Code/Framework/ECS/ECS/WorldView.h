#pragma once

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
	template <typename... TTypes>
	class WorldView final
	{
	public:
		explicit WorldView(ecs::EntityWorld& entityWorld);
		WorldView(const WorldView&) = delete;

		template<typename... TOthers>
		operator WorldView<TOthers...>&();

	public:
		//////////////////////////////////////////////////////////////////////////
		// Entity

		bool IsAlive(const Entity& entity) const;

		auto CreateEntity() -> ecs::Entity;

		void DestroyEntity(const Entity& entity);

		//////////////////////////////////////////////////////////////////////////
		// Component

		template<class TComponent, typename... TArgs>
		auto AddComponent(const Entity& entity, TArgs&&... args)->decltype(auto);

		template<class TComponent>
		void RemoveComponent(const Entity& entity);

		template<class TComponent>
		bool HasComponent(const Entity& entity, const bool alive = true) const;

		template<class TComponent>
		auto ReadComponent(const Entity& entity, const bool alive = true)->const TComponent&;

		template<class TComponent>
		auto WriteComponent(const Entity& entity, const bool alive = true)->TComponent&;

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
		auto Events() -> const Array<TEvent>&;

		template<class TQuery>
		auto Query() -> ecs::QueryRange<TQuery>;

	private:
		ecs::EntityWorld& m_EntityWorld;
		ecs::QueryRegistry& m_QueryRegistry;
	};
}

#include "WorldView.inl"