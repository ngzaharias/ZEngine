#pragma once

#include "Core/Array.h"
#include "Core/Map.h"
#include "Core/SparseArray.h"
#include "ECS/ComponentContainer.h"
#include "ECS/ComponentId.h"
#include "ECS/ComponentTag.h"
#include "ECS/Entity.h"
#include "ECS/EntityChange.h"

namespace ecs
{
	class EntityBuffer final
	{
		friend class EntityStorage;
		friend class ReplicationPeer;

		using Containers = SparseArray<ecs::ComponentId, ecs::IComponentContainer*>;
		using Changes = Map<ecs::Entity, ecs::EntityChange>;
		using Handles = Array<ecs::Entity>;

	public:
		//////////////////////////////////////////////////////////////////////////
		// Entity

		auto CreateEntity() -> ecs::Entity;

		void DestroyEntity(const ecs::Entity& entity);

		void RecycleEntity(const ecs::Entity& entity);

		//////////////////////////////////////////////////////////////////////////
		// Component

		template<class TComponent>
		void RegisterComponent();

		template<class TComponent, typename... TArgs>
		auto AddComponent(const ecs::Entity& entity, TArgs&&... args) -> TComponent&;

		template<class TComponent>
		void UpdateComponent(const ecs::Entity& entity);

		template<class TComponent>
		void RemoveComponent(const ecs::Entity& entity);

		//////////////////////////////////////////////////////////////////////////
		// Changes

		Changes& GetChanges();
		const Changes& GetChanges() const;

		//////////////////////////////////////////////////////////////////////////
		// Container

		Containers& GetContainers();
		const Containers& GetContainers() const;

		ecs::IComponentContainer& GetContainerAt(const ecs::ComponentId typeId);
		const ecs::IComponentContainer& GetContainerAt(const ecs::ComponentId typeId) const;

		template<typename TComponent>
		ecs::ComponentContainer<TComponent>& GetContainerAt();
		template<typename TComponent>
		const ecs::ComponentContainer<TComponent>& GetContainerAt() const;

		ecs::IComponentContainer* TryContainerAt(const ecs::ComponentId typeId);
		const ecs::IComponentContainer* TryContainerAt(const ecs::ComponentId typeId) const;

		template<typename TComponent>
		ecs::ComponentContainer<TComponent>* TryContainerAt();
		template<typename TComponent>
		const ecs::ComponentContainer<TComponent>* TryContainerAt() const;

	private:
		int32 m_Index = 0;

		Containers m_Components = {};
		Changes m_EntityChanges = {};
		Handles m_HandlesRecycled = {};
	};
}

#include "EntityBuffer.inl"
