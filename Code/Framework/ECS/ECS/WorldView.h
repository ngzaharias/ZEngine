#pragma once

template<typename Type>
class Set;

namespace ecs
{
	class EntityWorld;
	class QueryRegistry;

	struct Entity;

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

		auto CreateEntity()->ecs::Entity;

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
		// Component - Event

		template<class TComponent, typename... TArgs>
		auto AddEventComponent(TArgs&&... args)->decltype(auto);

		//////////////////////////////////////////////////////////////////////////
		// Singleton

		template<class TComponent>
		auto ReadSingleton()->const TComponent&;

		template<class TComponent>
		auto WriteSingleton()->TComponent&;

		//////////////////////////////////////////////////////////////////////////
		// Resource

		template<class TResource>
		auto ReadResource()->const TResource&;

		template<class TResource>
		auto WriteResource()->TResource&;

		//////////////////////////////////////////////////////////////////////////
		// Query

		template<class TQuery>
		auto HasAny()-> bool;

		template<class TQuery>
		auto Query()-> const Set<Entity>&;

	private:
		ecs::EntityWorld& m_EntityWorld;
		ecs::QueryRegistry& m_QueryRegistry;
	};
}

#include "WorldView.inl"