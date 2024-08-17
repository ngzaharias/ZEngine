#pragma once

#include "Core/Array.h"
#include "Core/Map.h"
#include "Core/SparseArray.h"
#include "ECS/Component.h"
#include "ECS/ComponentMask.h"
#include "ECS/ComponentStorage.h"
#include "ECS/Entity.h"

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

		using EntityMap = Map<ecs::Entity, EntityChange>;
		using Components = SparseArray<ecs::ComponentId, ecs::IComponentStorage*>;

	public:
		template<class TComponent>
		void RegisterComponent();

		auto CreateEntity()->ecs::Entity;
		void DestroyEntity(const ecs::Entity& entity);

		template<class TComponent, typename... TArgs>
		auto AddComponent(const ecs::Entity& entity, TArgs&&... args)->decltype(auto);
		template<class TComponent>
		void RemoveComponent(const ecs::Entity& entity);
		template<class TComponent>
		void UpdateComponent(const ecs::Entity& entity);

	private:
		ecs::Entity m_HandlesUnused = { 0, 0 };
		Array<ecs::Entity> m_HandlesRecycled = { };

		Components m_Components = { };
		EntityMap m_EntityChanges = { };
	};
}

#include "FrameBuffer.inl"
