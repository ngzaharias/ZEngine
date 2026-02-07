#pragma once

#include "Core/Map.h"
#include "Core/MemBuffer.h"
#include "Core/TypeName.h"
#include "ECS/Component.h"
#include "ECS/ComponentTag.h"
#include "ECS/Entity.h"
#include "ECS/EntityBuffer.h"
#include "ECS/EntityStorage.h"
#include "ECS/Event.h"
#include "ECS/EventBuffer.h"
#include "ECS/EventTag.h"
#include "ECS/IsPrototype.h"
#include "ECS/IsReplicated.h"
#include "ECS/QueryRegistry.h"
#include "ECS/ResourceId.h"
#include "ECS/ResourceTag.h"
#include "ECS/ReplicationComponent.h"
#include "ECS/Singleton.h"
#include "ECS/SingletonStorage.h"
#include "ECS/SingletonTag.h"
#include "ECS/System.h"
#include "ECS/SystemId.h"
#include "ECS/SystemTag.h"
#include "ECS/TypeComponent.h"
#include "ECS/TypeEvent.h"
#include "ECS/TypeInfo.h"
#include "ECS/TypeResource.h"
#include "ECS/TypeSingleton.h"
#include "ECS/TypeSystem.h"

namespace ecs
{
	class TypeRegistry final
	{
		friend class EntityWorld;

	public:
		using TypeMap		= Map<TypeId,			ecs::TypeInfo>;
		using ComponentMap	= Map<ecs::ComponentId, ecs::TypeComponent>;
		using EventMap		= Map<ecs::EventId,		ecs::TypeEvent>;
		using ResourceMap	= Map<ecs::ResourceId,	ecs::TypeResource>;
		using SingletonMap	= Map<ecs::SingletonId, ecs::TypeSingleton>;
		using SystemMap		= Map<ecs::SystemId,	ecs::TypeSystem>;

		const TypeMap& GetTypeMap() const;
		const ecs::TypeInfo& GetTypeInfo(const TypeId typeId) const;

		const ComponentMap& GetComponentMap() const;
		const ecs::TypeComponent& GetComponentInfo(const ecs::ComponentId& typeId) const;
		const ecs::TypeComponent* TryComponentInfo(const ecs::ComponentId& typeId) const;

		const ecs::TypeEvent& GetTypeEvent(const ecs::EventId& typeId) const;

		const ecs::TypeEvent* TryTypeEvent(const ecs::EventId& typeId) const;

		//////////////////////////////////////////////////////////////////////////
		// Component

		template <typename TComponent>
		void RegisterComponent();

		bool HasComponent(ecs::EntityStorage& storage, const ecs::ComponentId typeId, const ecs::Entity& entity) const;
		void AddComponent(ecs::EntityStorage& storage, const ecs::ComponentId typeId, const ecs::Entity& entity) const;
		void AddComponent(ecs::EntityStorage& storage, const ecs::ComponentId typeId, const ecs::Entity& entity, const MemBuffer& data) const;
		void UpdateComponent(ecs::EntityStorage& storage, const ecs::ComponentId typeId, const ecs::Entity& entity) const;
		void UpdateComponent(ecs::EntityStorage& storage, const ecs::ComponentId typeId, const ecs::Entity& entity, const MemBuffer& data) const;
		void RemoveComponent(ecs::EntityStorage& storage, const ecs::ComponentId typeId, const ecs::Entity& entity) const;
		void ReadComponent(ecs::EntityStorage& storage, const ecs::ComponentId typeId, const ecs::Entity& entity, MemBuffer& data) const;
		void WriteComponent(ecs::EntityStorage& storage, const ecs::ComponentId typeId, const ecs::Entity& entity, const MemBuffer& data) const;

		template<typename TComponent>
		static bool HasComponentSolo(ecs::EntityStorage& storage, const ecs::Entity& entity);
		template<typename TComponent>
		static void AddComponentData(ecs::EntityStorage& storage, const ecs::Entity& entity, const MemBuffer& data);
		template<typename TComponent>
		static void AddComponentSolo(ecs::EntityStorage& storage, const ecs::Entity& entity);
		template<typename TComponent>
		static void UpdateComponentData(ecs::EntityStorage& storage, const ecs::Entity& entity, const MemBuffer& data);
		template<typename TComponent>
		static void UpdateComponentSolo(ecs::EntityStorage& storage, const ecs::Entity& entity);
		template<typename TComponent>
		static void RemoveComponentSolo(ecs::EntityStorage& storage, const ecs::Entity& entity);
		template<typename TComponent>
		static void ReadComponentData(ecs::EntityStorage& storage, const ecs::Entity& entity, MemBuffer& data);
		template<typename TComponent>
		static void WriteComponentData(ecs::EntityStorage& storage, const ecs::Entity& entity, const MemBuffer& data);

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

		//////////////////////////////////////////////////////////////////////////
		// System

		template <typename TSystem>
		void RegisterSystem();

	private:
		TypeMap			m_TypeMap = {};

		ComponentMap	m_ComponentMap = {};
		EventMap		m_EventMap = {};
		ResourceMap		m_ResourceMap = {};
		SingletonMap	m_SingletonMap = {};
		SystemMap		m_SystemMap = {};
	};
}

#include "TypeRegistry.inl"