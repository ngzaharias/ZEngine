#pragma once

#include "Core/Map.h"
#include "Core/SparseArray.h"
#include "ECS/ComponentId.h"
#include "ECS/ComponentMask.h"
#include "ECS/ComponentStorage.h"
#include "ECS/ComponentTag.h"
#include "ECS/Entity.h"
#include "ECS/SingletonStorage.h"

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

		using Components = SparseArray<ecs::ComponentId, ecs::IComponentStorage*>;
		using EntityMap = Map<ecs::Entity, ecs::ComponentMask>;
		using EntitySet = Array<ecs::Entity>;
		using Singletons = SparseArray<TypeId, ecs::ISingletonStorage*>;

	public:
		//////////////////////////////////////////////////////////////////////////
		// Entity

		bool IsAlive(const ecs::Entity& entity) const;

		//////////////////////////////////////////////////////////////////////////
		// Component

		template<class TComponent>
		void RegisterComponent();

		template<class TComponent>
		bool HasComponent(const ecs::Entity& entity, const bool alive = true) const;

		template<class TComponent>
		auto GetComponent(const ecs::Entity& entity, const bool alive = true) const -> TComponent&;

		template<class TComponent>
		auto TryComponent(const ecs::Entity& entity, const bool alive = true) const -> TComponent*;

		//////////////////////////////////////////////////////////////////////////
		// Singleton

		template<class TSingleton, typename... TArgs>
		void RegisterSingleton(TArgs&&... args);

		template<class TSingleton>
		auto GetSingleton() -> TSingleton&;

	private:
		void FlushChanges(ecs::FrameBuffer& frameBuffer, ecs::QueryRegistry& queryRegistry);

	public:
		Components m_AliveComponents;
		Components m_DeadComponents;
		EntityMap m_AliveEntities;
		EntitySet m_DeadEntities;
		Singletons m_Singletons;

		Set<TypeId> m_SingletonsUpdated;
	};
}

#include "EntityStorage.inl"