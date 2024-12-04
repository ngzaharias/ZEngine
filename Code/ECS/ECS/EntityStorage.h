#pragma once

#include "Core/Map.h"
#include "Core/SparseArray.h"
#include "ECS/Component.h"
#include "ECS/ComponentMask.h"
#include "ECS/ComponentStorage.h"
#include "ECS/Entity.h"

namespace ecs
{
	class EntityWorld;
	class FrameBuffer;
	class QueryRegistry;

	class EntityStorage
	{
		friend class EntityWorld;

		using EntityMap = Map<ecs::Entity, ecs::ComponentMask>;
		using EntitySet = Array<ecs::Entity>;
		using Components = SparseArray<ecs::ComponentId, ecs::IComponentStorage*>;

	public:
		bool IsAlive(const ecs::Entity& entity) const;

		template<class TComponent>
		bool IsRegistered() const;

		template<class TComponent>
		void RegisterComponent();

		template<class TComponent>
		bool HasComponent(const ecs::Entity& entity, const bool alive = true) const;

		template<class TComponent>
		auto GetComponent(const ecs::Entity& entity, const bool alive = true)->TComponent&;

	private:
		void FlushChanges(ecs::FrameBuffer& frameBuffer, ecs::QueryRegistry& queryRegistry);

	public:
		Components m_AliveComponents;
		Components m_DeadComponents;
		EntityMap m_AliveEntities;
		EntitySet m_DeadEntities;
	};
}

#include "EntityStorage.inl"