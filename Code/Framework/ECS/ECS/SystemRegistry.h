#pragma once

#include "Core/SparseArray.h"
#include "ECS/System.h"
#include "ECS/SystemEntry.h"

class GameTime;

namespace ecs
{
	class EntityWorld;
	class System;

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
		void RegisterPriority(const int32 priority);

		template<class TSystem>
		TSystem& GetSystem();

	private:
		template<typename TSystem>
		static void InitialiseFunction(ecs::EntityWorld& entityWorld, ecs::System& system);
		template<typename TSystem>
		static void ShutdownFunction(ecs::EntityWorld& entityWorld, ecs::System& system);
		template<typename TSystem>
		static void UpdateFunction(ecs::EntityWorld& entityWorld, ecs::System& system, const GameTime& gameTime);

	private:
		Array<ecs::SystemEntry*> m_Priorities = { };
		SparseArray<ecs::SystemId, ecs::SystemEntry> m_Entries = { };
	};
}

#include "SystemRegistry.inl"

