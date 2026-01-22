#pragma once

#include "Core/Array.h"
#include "Core/Map.h"
#include "Core/SparseArray.h"
#include "ECS/ComponentId.h"
#include "ECS/ComponentStorage.h"
#include "ECS/ComponentTag.h"
#include "ECS/Entity.h"
#include "ECS/EntityChange.h"

namespace ecs
{
	class EntityBuffer final
	{
		friend class EntityStorage;

	public:
		using Components = SparseArray<ecs::ComponentId, ecs::IComponentStorage*>;
		using EntityMap = Map<ecs::Entity, ecs::EntityChange>;

		//////////////////////////////////////////////////////////////////////////
		// Entity

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
		void UpdateComponent(const ecs::Entity& entity);

	private:
		ecs::Entity m_HandlesUnused = { 0, 0 };
		Array<ecs::Entity> m_HandlesRecycled = {};

		Components m_AddedComponents = {};
		EntityMap m_EntityChanges = {};
	};
}

#include "EntityBuffer.inl"
