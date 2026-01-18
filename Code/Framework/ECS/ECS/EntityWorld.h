#pragma once

#include "Core/Map.h"
#include "ECS/EntityStorage.h"
#include "ECS/EventBuffer.h"
#include "ECS/EventStorage.h"
#include "ECS/FrameBuffer.h"
#include "ECS/IsReplicated.h"
#include "ECS/QueryRegistry.h"
#include "ECS/ResourceRegistry.h"
#include "ECS/Singleton.h"
#include "ECS/SystemRegistry.h"
#include "ECS/TypeInfo.h"

class GameTime;

namespace ecs
{
	template<typename...>
	class EntityView_t;
	struct Entity;
}

namespace ecs
{
	class EntityWorld final
	{
		template<typename...>
		friend class EntityView_t;

	public:
		EntityWorld();

		void Initialise();
		void Shutdown();

		void Update(const GameTime& gameTime);

		template<typename TType>
		void RegisterType();

		template<typename TType>
		bool IsRegistered() const;

		template<typename TWorldView>
		TWorldView WorldView();

		void FlushData();
		void FlushSync();

		str::String LogDependencies() const;
		str::String LogUpdateOrder() const;

		//////////////////////////////////////////////////////////////////////////
		// Entity

		bool IsAlive(const ecs::Entity& entity) const;

		auto CreateEntity() -> ecs::Entity;

		void DestroyEntity(const ecs::Entity& entity);

		//////////////////////////////////////////////////////////////////////////
		// Component

		template<class TComponent>
		void RegisterComponent();

		template<class TComponent, typename... TArgs>
		auto AddComponent(const ecs::Entity& entity, TArgs&&... args)->TComponent&;

		template<class TComponent>
		void RemoveComponent(const ecs::Entity& entity);

		template<class TComponent>
		bool HasComponent(const ecs::Entity& entity, const bool alive = true) const;

		template<class TComponent>
		auto ReadComponent(const ecs::Entity& entity, const bool alive = true)->const TComponent&;

		template<class TComponent>
		auto WriteComponent(const ecs::Entity& entity, const bool alive = true)->TComponent&;

		//////////////////////////////////////////////////////////////////////////
		// Event

		template<class TEvent>
		void RegisterEvent();

		template<class TEvent, typename... TArgs>
		auto AddEvent(TArgs&&... args)->TEvent&;

		//////////////////////////////////////////////////////////////////////////
		// Resource

		template<class TResource>
		void RegisterResource(TResource& resource);

		template<class TResource>
		auto ReadResource() -> const TResource&;

		template<class TResource>
		auto WriteResource() -> TResource&;

		//////////////////////////////////////////////////////////////////////////
		// Singleton

		template<class TSingleton, typename... TArgs>
		void RegisterSingleton(TArgs&&... args);

		template<class TSingleton>
		auto ReadSingleton() -> const TSingleton&;

		template<class TSingleton>
		auto WriteSingleton() -> TSingleton&;

		//////////////////////////////////////////////////////////////////////////
		// System

		/// \brief Registers and creates a system.
		template<class TSystem, typename... TArgs>
		void RegisterSystem(TArgs&&... args);

		template<class TSystem>
		auto GetSystem() -> TSystem&;

	private:
		//////////////////////////////////////////////////////////////////////////
		// EntityView

		template <typename TComponent>
		auto GetComponentForView(const ecs::Entity& entity) const->TComponent*;
		template <typename...TComponents>
		auto GetComponentsForView(const ecs::Entity& entity) const->std::tuple<TComponents*...>;
		template <typename TComponent>
		auto TryComponentForView(const ecs::Entity& entity) const->TComponent*;
		template <typename...TComponents>
		auto TryComponentsForView(const ecs::Entity& entity) const->std::tuple<TComponents*...>;

	public:
		ecs::FrameBuffer m_FrameBuffer;
		ecs::EntityStorage m_EntityStorage;
		ecs::QueryRegistry m_QueryRegistry;

		ecs::EventBuffer m_EventBuffer;
		ecs::EventBuffer m_EventSync;
		ecs::EventStorage m_EventStorage;

		ecs::ResourceRegistry m_ResourceRegistry;
		ecs::SystemRegistry m_SystemRegistry;

		Map<TypeId, ecs::TypeInfo> m_TypeMap;
		Map<ecs::ComponentId, TypeId> m_ComponentRemap;
	};
}

#include "EntityWorld.inl"