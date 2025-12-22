#pragma once

#include "ECS/ComponentRegistry.h"
#include "ECS/EntityStorage.h"
#include "ECS/EventRegistry.h"
#include "ECS/FrameBuffer.h"
#include "ECS/QueryRegistry.h"
#include "ECS/ResourceRegistry.h"
#include "ECS/SingletonRegistry.h"
#include "ECS/SystemRegistry.h"

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

		template<class TType>
		bool IsRegistered() const;

		template<class TWorldView>
		TWorldView WorldView();

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
		// Event

		template<class TEvent>
		void RegisterEvent();

		template<class TEvent, typename... TArgs>
		auto AddEvent(TArgs&&... args)->decltype(auto);

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

		/// \brief Sets a systems priority which influences when it will update.
		template<class TSystem>
		void RegisterSystemPriority(const int32 priority);

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

		ecs::ComponentRegistry m_ComponentRegistry;
		ecs::EventRegistry m_EventRegistry;
		ecs::ResourceRegistry m_ResourceRegistry;
		ecs::SingletonRegistry m_SingletonRegistry;
		ecs::SystemRegistry m_SystemRegistry;
	};
}

#include "EntityWorld.inl"