#pragma once

#include "Core/Set.h"
#include "ECS/EntityWorld.h"

namespace ut
{
	struct RAIIHelper
	{
		RAIIHelper();
		~RAIIHelper();

		void Initialise();

		void Update();
		void Update(const int32 count);

		bool IsAlive(const ecs::Entity& entity) const;

		auto CreateEntity()->ecs::Entity;
		void DestroyEntity(const ecs::Entity& entity);

		template<typename TComponent>
		auto AddComponent(const ecs::Entity& entity)->TComponent&;
		template<typename TComponent>
		void RemoveComponent(const ecs::Entity& entity);
		template<class TComponent>
		bool HasComponent(const ecs::Entity& entity) const;
		template<class TComponent>
		auto ReadComponent(const ecs::Entity& entity)->const TComponent&;
		template<class TComponent>
		auto WriteComponent(const ecs::Entity& entity)->TComponent&;

		ecs::EntityWorld m_EntityWorld;
		Set<ecs::Entity> m_Entities = { };
	};
}

#include "UTHelpers.inl"