#pragma once

#include "Core/Map.h"
#include "Core/MemBuffer.h"
#include "ECS/ComponentTag.h"
#include "ECS/Entity.h"
#include "ECS/EntityWorld.h"
#include "ECS/EventTag.h"
#include "ECS/ResourceTag.h"
#include "ECS/SingletonTag.h"
#include "ECS/SystemTag.h"
#include "ECS/TypeComponent.h"
#include "ECS/TypeEvent.h"
#include "ECS/TypeResource.h"
#include "ECS/TypeSingleton.h"
#include "ECS/TypeSystem.h"

namespace ecs
{
	class TypeRegistry final
	{
	public:
		using ComponentMap = Map<ecs::ComponentId, ecs::TypeComponent>;
		using EventMap = Map<ecs::EventId, ecs::TypeEvent>;
		using ResourceMap = Map<ecs::ResourceId, ecs::TypeResource>;
		using SingletonMap = Map<ecs::SingletonId, ecs::TypeSingleton>;
		using SystemMap = Map<ecs::SystemId, ecs::TypeSystem>;

		const ecs::TypeEvent& GetTypeEvent(const ecs::EventId& typeId) const;

		//////////////////////////////////////////////////////////////////////////
		// Component

		template <typename TComponent>
		void RegisterComponent();

		void AddComponent(ecs::EntityWorld& world, const ecs::ComponentId typeId, const ecs::Entity& entity, const MemBuffer& data) const;

		void UpdateComponent(ecs::EntityWorld& world, const ecs::ComponentId typeId, const ecs::Entity& entity, const MemBuffer& data) const;

		void RemoveComponent(ecs::EntityWorld& world, const ecs::ComponentId typeId, const ecs::Entity& entity) const;

		template<typename TComponent>
		static void AddComponent(ecs::EntityWorld& world, const ecs::Entity& entity, const MemBuffer& data);

		template<typename TComponent>
		static void RemoveComponent(ecs::EntityWorld& world, const ecs::Entity& entity);

		template<typename TComponent>
		static void ReadComponent(ecs::EntityWorld& world, const ecs::Entity& entity, MemBuffer& data);

		template<typename TComponent>
		static void WriteComponent(ecs::EntityWorld& world, const ecs::Entity& entity, const MemBuffer& data);

		//////////////////////////////////////////////////////////////////////////
		// Event

		template <typename TEvent>
		void RegisterEvent();

		void AddEvent(ecs::EventBuffer& buffer, const ecs::EventId typeId, const MemBuffer& data) const;

		template<typename TEvent>
		static void AddEventMethod(ecs::EventBuffer& buffer, const MemBuffer& data);

		//////////////////////////////////////////////////////////////////////////
		// Resource

		template <typename TResource>
		void RegisterResource();

		//////////////////////////////////////////////////////////////////////////
		// Singleton

		template <typename TSingleton>
		void RegisterSingleton();

		void UpdateSingleton(ecs::EntityWorld& world, const ecs::SingletonId typeId, const MemBuffer& data) const;

		template<typename TSingleton>
		static void ReadSingleton(ecs::EntityWorld& world, MemBuffer& data);

		template<typename TSingleton>
		static void WriteSingleton(ecs::EntityWorld& world, const MemBuffer& data);

		//////////////////////////////////////////////////////////////////////////
		// System

		template <typename TSystem>
		void RegisterSystem();

	private:
		ComponentMap m_ComponentMap = {};
		EventMap m_EventMap = {};
		ResourceMap m_ResourceMap = {};
		SingletonMap m_SingletonMap = {};
		SystemMap m_SystemMap = {};
	};
}

#include "TypeRegistry.inl"