#pragma once

#include "Core/Map.h"
#include "Core/SparseArray.h"
#include "ECS/Component.h"
#include "ECS/ComponentMask.h"
#include "ECS/ComponentStorage.h"
#include "ECS/Entity.h"
#include "ECS/Event.h"
#include "ECS/EventStorage.h"

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
		using Events = SparseArray<ecs::EventId, ecs::IEventStorage*>;

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
		auto GetComponent(const ecs::Entity& entity, const bool alive = true) -> TComponent&;

		//////////////////////////////////////////////////////////////////////////
		// Event

		template<class TEvent>
		void RegisterEvent();

		template<class TEvent>
		auto GetEvents() const -> const Array<TEvent>&;

	private:
		void FlushChanges(ecs::FrameBuffer& frameBuffer, ecs::QueryRegistry& queryRegistry);

	public:
		Components m_AliveComponents;
		Components m_DeadComponents;
		EntityMap m_AliveEntities;
		EntitySet m_DeadEntities;
		Events m_Events;
	};
}

#include "EntityStorage.inl"