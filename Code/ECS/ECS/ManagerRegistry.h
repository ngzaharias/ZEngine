#pragma once

#include <Core/SparseArray.h>

#include <ECS/Manager.h>
#include <ECS/ManagerEntry.h>

class GameTime;

namespace ecs
{
	class ManagerRegistry
	{
	public:
		template<class TManager, typename... TArgs>
		void Register(TArgs&&... args);

		void Initialise();
		void Shutdown();

		template<class TManager>
		bool IsRegistered() const;

		template<class TManager>
		TManager& GetManager();

	private:
		SparseArray<ecs::ManagerId, ecs::ManagerEntry> m_Entries = { };
	};
}

#include "ManagerRegistry.inl"

