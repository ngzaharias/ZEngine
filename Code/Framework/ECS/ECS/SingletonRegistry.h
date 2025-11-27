#pragma once

#include "Core/SparseArray.h"

#include "ECS/Singleton.h"
#include "ECS/SingletonEntry.h"

class GameTime;

namespace ecs
{
	class SingletonRegistry
	{
	public:
		template<class TSingleton>
		void Register();

		template<class TSingleton>
		bool IsRegistered() const;
		bool IsRegistered(const ecs::SingletonId id) const;

		template<class TSingleton>
		auto GetEntry() const -> const ecs::SingletonEntry&;
		auto GetEntry(const ecs::SingletonId id) const -> const ecs::SingletonEntry&;

	private:
		SparseArray<ecs::SingletonId, ecs::SingletonEntry> m_Entries = { };
	};
}

#include "SingletonRegistry.inl"

