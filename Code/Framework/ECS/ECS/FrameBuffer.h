#pragma once

#include "Core/Array.h"
#include "Core/Map.h"
#include "Core/SparseArray.h"
#include "ECS/ComponentId.h"
#include "ECS/ComponentMask.h"
#include "ECS/ComponentStorage.h"
#include "ECS/ComponentTag.h"
#include "ECS/Entity.h"
#include "ECS/EventStorage.h"

namespace ecs
{
	struct EntityChange
	{
		ecs::ComponentMask m_Added;
		ecs::ComponentMask m_Removed;
		ecs::ComponentMask m_Updated;
		bool m_IsDestroy = false;
	};

	class FrameBuffer
	{
		friend class EntityStorage;

		using Components = SparseArray<ecs::ComponentId, ecs::IComponentStorage*>;
		using EntityMap = Map<ecs::Entity, EntityChange>;
		using Events = SparseArray<TypeId, ecs::IEventStorage*>;
		using Singletons = Set<TypeId>;

	public:
		//////////////////////////////////////////////////////////////////////////
		// Entity

		auto CreateEntity()->ecs::Entity;
		void DestroyEntity(const ecs::Entity& entity);

		//////////////////////////////////////////////////////////////////////////
		// Component

		template<class TComponent>
		void RegisterComponent();

		template<class TComponent, typename... TArgs>
		auto AddComponent(const ecs::Entity& entity, TArgs&&... args)->decltype(auto);

		template<class TComponent>
		void RemoveComponent(const ecs::Entity& entity);

		template<class TComponent>
		void UpdateComponent(const ecs::Entity& entity);

		//////////////////////////////////////////////////////////////////////////
		// Event

		template<class TEvent>
		void RegisterEvent();

		template<class TEvent, typename... TArgs>
		auto AddEvent(TArgs&&... args)->decltype(auto);

		//////////////////////////////////////////////////////////////////////////
		// Singletons

		template<class TSingleton>
		void UpdateSingleton();

	private:
		ecs::Entity m_HandlesUnused = { 0, 0 };
		Array<ecs::Entity> m_HandlesRecycled = { };

		Components m_Components = { };
		EntityMap m_EntityChanges = { };
		Events m_Events = { };
		Singletons m_Singletons = { };
	};
}

#include "FrameBuffer.inl"
