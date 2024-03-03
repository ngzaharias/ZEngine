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

	public:

		//////////////////////////////////////////////////////////////////////////
		// Entity

		bool IsAlive(const Entity& entity) const;

		auto CreateEntity()->ecs::Entity;

		void DestroyEntity(const Entity& entity);

		//////////////////////////////////////////////////////////////////////////
		// Component

		template<class TComponent>
		bool HasComponent(const Entity& entity, const bool alive = true) const;

		template<class TComponent>
		auto GetComponent(const Entity& entity, const bool alive = true)->TComponent&;

		template<class TComponent, typename... TArgs>
		auto AddComponent(const Entity& entity, TArgs&&... args)->decltype(auto);
		template<class TComponent, typename... TArgs>
		auto AddEventComponent(TArgs&&... args)->decltype(auto);

		template<class TComponent>
		void RemoveComponent(const Entity& entity);

		//////////////////////////////////////////////////////////////////////////
		// Component - Singleton

		template<class TComponent>
		bool HasSingleton(const bool alive = true) const;

		template<class TComponent>
		auto GetSingleton(const bool alive = true)->TComponent&;

		template<class TComponent, typename... TArgs>
		auto AddSingleton(TArgs&&... args)->decltype(auto);

		template<class TComponent>
		void RemoveSingleton();

		//////////////////////////////////////////////////////////////////////////
		// Resource

		template<class TResource>
		bool HasResource();

		template<class TResource>
		auto GetResource()->TResource&;

		template<class TResource>
		void AddResource(TResource& resource);

		template<class TResource>
		void RemoveResource();

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