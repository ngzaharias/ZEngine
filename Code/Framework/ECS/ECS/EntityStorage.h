#pragma once

#include "Core/Map.h"
#include "Core/SparseArray.h"
#include "ECS/ComponentContainer.h"
#include "ECS/ComponentId.h"
#include "ECS/ComponentMask.h"
#include "ECS/ComponentTag.h"
#include "ECS/Entity.h"
#include "ECS/EntityBuffer.h"

namespace ecs
{
	class EntityWorld;
	class FrameBuffer;
	class QueryRegistry;
}

namespace ecs
{
	class EntityStorage
	{
		friend class EntityWorld;

		using Components = SparseArray<ecs::ComponentId, ecs::IComponentContainer*>;
		using EntityMap = Map<ecs::Entity, ecs::ComponentMask>;
		using EntitySet = Array<ecs::Entity>;

	public:
		EntityStorage(ecs::QueryRegistry& queryRegistry);

		//////////////////////////////////////////////////////////////////////////
		// Entity

		bool IsAlive(const ecs::Entity& entity) const;

		auto CreateEntity() -> ecs::Entity;

		void DestroyEntity(const ecs::Entity& entity);

		//////////////////////////////////////////////////////////////////////////
		// Component

		template<class TComponent>
		void RegisterComponent();

		template<class TComponent, typename... TArgs>
		auto AddComponent(const ecs::Entity& entity, TArgs&&... args) -> TComponent&;

		template<class TComponent>
		void RemoveComponent(const ecs::Entity& entity);

		template<class TComponent>
		bool HasComponent(const ecs::Entity& entity, const bool alive = true) const;

		template<class TComponent>
		auto GetComponent(const ecs::Entity& entity, const bool alive = true) const -> TComponent&;

		template<class TComponent>
		auto TryComponent(const ecs::Entity& entity, const bool alive = true) const -> TComponent*;

	private:
		void FlushChanges();

	public:
		ecs::EntityBuffer m_MainBuffer;
		ecs::EntityBuffer m_SyncBuffer;
		ecs::QueryRegistry& m_QueryRegistry;

		Components m_AliveComponents;
		Components m_DeadComponents;
		EntityMap m_AliveEntities;
		EntitySet m_DeadEntities;
	};
}

#include "EntityStorage.inl"