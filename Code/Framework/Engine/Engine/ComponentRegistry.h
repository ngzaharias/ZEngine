#pragma once

#include "Core/Map.h"
#include "Core/MemBuffer.h"
#include "Core/TypeInfo.h"
#include "ECS/ComponentId.h"
#include "ECS/ComponentTag.h"
#include "ECS/Entity.h"
#include "ECS/EntityWorld.h"
#include "Engine/ComponentEntry.h"
#include "Engine/ReplicationComponent.h"

namespace net
{
	class ComponentRegistry
	{
	public:
		template<typename TComponent>
		void Register();

	private:
		template<typename TComponent>
		void Add(ecs::EntityWorld& world, const ecs::Entity& entity, const MemBuffer& buffer);
		template<typename TComponent>
		void Remove(ecs::EntityWorld& world, const ecs::Entity& entity);
		template<typename TComponent>
		void Read(ecs::EntityWorld& world, const ecs::Entity& entity, MemBuffer& buffer);
		template<typename TComponent>
		void Write(ecs::EntityWorld& world, const ecs::Entity& entity, const MemBuffer& buffer);

	public:
		Map<ecs::ComponentId, ComponentEntry> m_Entries = {};
		Map<ecs::ComponentId, TypeId> m_LocalToWorld = {};
		Map<TypeId, ecs::ComponentId> m_WorldToLocal = {};
	};
}

#include "ComponentRegistry.inl"