#pragma once

#include "Core/Map.h"
#include "Core/SparseArray.h"
#include "ECS/ComponentId.h"
#include "ECS/ComponentMask.h"
#include "ECS/ComponentStorage.h"
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

		using Components = SparseArray<ecs::ComponentId, ecs::IComponentStorage*>;
		using EntityMap = Map<ecs::Entity, ecs::ComponentMask>;
		using EntitySet = Array<ecs::Entity>;

	public:
		EntityStorage();

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

	private:
		void FlushChanges(ecs::QueryRegistry& queryRegistry);

	public:
		ecs::EntityBuffer m_MainBuffer;
		ecs::EntityBuffer m_SyncBuffer;

		Components m_AliveComponents;
		Components m_DeadComponents;
		EntityMap m_AliveEntities;
		EntitySet m_DeadEntities;
	};
}

#include "EntityStorage.inl"