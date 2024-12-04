#pragma once

#include "Core/Set.h"
#include "ECS/ComponentRegistry.h"
#include "ECS/EntityStorage.h"
#include "ECS/FrameBuffer.h"
#include "ECS/QueryRegistry.h"
#include "ECS/ResourceRegistry.h"
#include "ECS/System.h"
#include "ECS/SystemRegistry.h"

class GameTime;

namespace ecs
{
	struct Entity;

	class EntityWorld final
	{
	public:
		EntityWorld();

		void Initialise();
		void Shutdown();

		void Update(const GameTime& gameTime);

		bool IsInitialised() const;

		template<class TType>
		bool IsRegistered() const;

		template<class TWorldView>
		TWorldView GetWorldView();

		//////////////////////////////////////////////////////////////////////////
		// Entity

		bool IsAlive(const ecs::Entity& entity) const;

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
		bool HasComponent(const ecs::Entity& entity, const bool alive = true) const;

		template<class TComponent>
		auto ReadComponent(const ecs::Entity& entity, const bool alive = true)->const TComponent&;

		template<class TComponent>
		auto WriteComponent(const ecs::Entity& entity, const bool alive = true)->TComponent&;

		//////////////////////////////////////////////////////////////////////////
		// Component - Event

		template<class TComponent, typename... TArgs>
		auto AddEventComponent(TArgs&&... args)->decltype(auto);

		//////////////////////////////////////////////////////////////////////////
		// Singleton

		template<class TSingleton, typename... TArgs>
		void RegisterSingleton(TArgs&&... args);

		template<class TSingleton>
		auto ReadSingleton()->const TSingleton&;

		template<class TSingleton>
		auto WriteSingleton()->TSingleton&;

		//////////////////////////////////////////////////////////////////////////
		// Resource

		template<class TResource>
		void RegisterResource(TResource& resource);

		template<class TResource>
		auto ReadResource()->const TResource&;

		template<class TResource>
		auto WriteResource()->TResource&;

		//////////////////////////////////////////////////////////////////////////
		// System

		/// \brief Registers and creates a system.
		template<class TSystem, typename... TArgs>
		void RegisterSystem(TArgs&&... args);

		/// \brief Sets a systems priority which influences when it will update.
		template<class TSystem>
		void RegisterSystemPriority(const int32 priority);

		template<class TSystem>
		auto GetSystem()->TSystem&;

	public:
		ecs::FrameBuffer m_FrameBuffer;
		ecs::EntityStorage m_EntityStorage;
		ecs::QueryRegistry m_QueryRegistry;

		ecs::ComponentRegistry m_ComponentRegistry;
		ecs::ResourceRegistry m_ResourceRegistry;
		ecs::SystemRegistry m_SystemRegistry;


		Array<ecs::Entity> m_EventEntities = { };
		ecs::Entity m_SingletonEntity = { };
		bool m_IsInitialised = false;
	};
}

#include "EntityWorld.inl"