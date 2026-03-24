#pragma once

#include "Core/Map.h"
#include "ECS/Component.h"
#include "ECS/EntityStorage.h"
#include "ECS/EventStorage.h"
#include "ECS/IsReplicated.h"
#include "ECS/QueryRegistry.h"
#include "ECS/ResourceRegistry.h"
#include "ECS/Singleton.h"
#include "ECS/SingletonStorage.h"
#include "ECS/SystemRegistry.h"
#include "ECS/TypeInfo.h"
#include "ECS/TypeRegistry.h"

#include <concepts>

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
		EntityWorld(ecs::TypeRegistry& typeRegistry);

		void Initialise();
		void Shutdown();

		void Update(const GameTime& gameTime);

		template<typename TType>
		bool IsRegistered() const;

		//////////////////////////////////////////////////////////////////////////
		// Entity

		bool IsAlive(const ecs::Entity& entity) const;

		auto CreateEntity() -> ecs::Entity;

		void DestroyEntity(const ecs::Entity& entity);

		//////////////////////////////////////////////////////////////////////////
		// Component

		template<typename TComponent>
		void RegisterComponent();

		template<typename TComponent, typename... TArgs>
		requires ecs::IsSoloOrStaticComponent<TComponent>
		auto AddComponent(TArgs&&... args)->TComponent&;
		template<typename TComponent, typename... TArgs>
		auto AddComponent(const ecs::Entity& entity, TArgs&&... args)->TComponent&;

		template<typename TComponent>
		requires ecs::IsSoloOrStaticComponent<TComponent>
		void RemoveComponent();
		template<typename TComponent>
		void RemoveComponent(const ecs::Entity& entity);

		template<typename TComponent>
		requires ecs::IsSoloOrStaticComponent<TComponent>
		bool HasComponent(const bool alive = true) const;
		template<typename TComponent>
		bool HasComponent(const ecs::Entity& entity, const bool alive = true) const;

		template<typename TComponent>
		requires ecs::IsSoloOrStaticComponent<TComponent>
		auto ReadComponent(const bool alive = true) -> const TComponent&;
		template<typename TComponent>
		auto ReadComponent(const ecs::Entity& entity, const bool alive = true) -> const TComponent&;

		template<typename TComponent>
		requires ecs::IsSoloOrStaticComponent<TComponent>
		auto WriteComponent(const bool alive = true)->TComponent&;
		template<typename TComponent>
		auto WriteComponent(const ecs::Entity& entity, const bool alive = true) -> TComponent&;

		//////////////////////////////////////////////////////////////////////////
		// Event

		template<typename TEvent>
		void RegisterEvent();

		template<typename TEvent, typename... TArgs>
		auto AddEvent(TArgs&&... args)->TEvent&;

		//////////////////////////////////////////////////////////////////////////
		// Resource

		template<typename TResource>
		void RegisterResource(TResource& resource);

		template<typename TResource>
		auto ReadResource() -> const TResource&;

		template<typename TResource>
		auto WriteResource() -> TResource&;

		//////////////////////////////////////////////////////////////////////////
		// Singleton

		template<typename TSingleton, typename... TArgs>
		void RegisterSingleton(TArgs&&... args);

		template<typename TSingleton>
		auto ReadSingleton() -> const TSingleton&;

		template<typename TSingleton>
		auto WriteSingleton() -> TSingleton&;

		//////////////////////////////////////////////////////////////////////////
		// System

		/// \brief Registers and creates a system.
		template<typename TSystem, typename... TArgs>
		void RegisterSystem(TArgs&&... args);

		template<typename TSystem>
		auto GetSystem() -> TSystem&;

		//////////////////////////////////////////////////////////////////////////
		// WorldView

		template<typename TWorldView>
		TWorldView WorldView();

		//////////////////////////////////////////////////////////////////////////
		// Debug

		str::String LogDependencies() const;
		str::String LogUpdateOrder() const;

	private:
		//////////////////////////////////////////////////////////////////////////
		// EntityView

		template<typename TComponent>
		auto GetComponentForView(const ecs::Entity& entity) const->TComponent*;
		template<typename...TComponents>
		auto GetComponentsForView(const ecs::Entity& entity) const->std::tuple<TComponents*...>;
		template<typename TComponent>
		auto TryComponentForView(const ecs::Entity& entity) const->TComponent*;
		template<typename...TComponents>
		auto TryComponentsForView(const ecs::Entity& entity) const->std::tuple<TComponents*...>;

	public:
		ecs::TypeRegistry& m_TypeRegistry;
		ecs::EntityStorage m_EntityStorage;
		ecs::EventStorage m_EventStorage;
		ecs::SingletonStorage m_SingletonStorage;

		ecs::QueryRegistry m_QueryRegistry;
		ecs::ResourceRegistry m_ResourceRegistry;
		ecs::SystemRegistry m_SystemRegistry;
	};
}

#include "EntityWorld.inl"