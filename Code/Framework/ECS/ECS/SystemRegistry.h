#pragma once

#include "Core/Array.h"
#include "Core/Map.h"
#include "Core/SparseArray.h"
#include "Core/TypeInfo.h"
#include "ECS/Component.h"
#include "ECS/ComponentId.h"
#include "ECS/ComponentTag.h"
#include "ECS/System.h"
#include "ECS/SystemEntry.h"

class GameTime;

namespace ecs
{
	class EntityWorld;
	class System;
	struct SystemTag;
}

namespace ecs
{
	using SystemDependencies = Map<TypeId, Set<TypeId>>;
	using SystemEntries = SparseArray<TypeId, ecs::SystemEntry>;
	using SystemOrder = Array<ecs::SystemEntry*>;

	class SystemRegistry
	{
	public:
		void Initialise(ecs::EntityWorld& entityWorld);
		void Shutdown(ecs::EntityWorld& entityWorld);

		void Update(ecs::EntityWorld& entityWorld, const GameTime& gameTime);

		template<class TSystem>
		bool IsRegistered() const;

		template<class TSystem, typename... TArgs>
		void Register(TArgs&&... args);

		template<class TSystem>
		TSystem& GetSystem();

		const SystemOrder& GetOrder() const { return m_Order; }
		const SystemEntries& GetEntries() const { return m_Entries; }
		const SystemDependencies& GetReads() const { return m_Reads; }
		const SystemDependencies& GetWrites() const { return m_Writes; }

	private:
		template<typename TSystem>
		static void InitialiseFunction(ecs::EntityWorld& entityWorld, ecs::System& system);
		template<typename TSystem>
		static void ShutdownFunction(ecs::EntityWorld& entityWorld, ecs::System& system);
		template<typename TSystem>
		static void UpdateFunction(ecs::EntityWorld& entityWorld, ecs::System& system, const GameTime& gameTime);

	private:
		SystemOrder m_Order = {};
		SystemEntries m_Entries = {};
		SystemDependencies m_Reads = {};
		SystemDependencies m_Writes = {};
	};
}

#include "SystemRegistry.inl"

