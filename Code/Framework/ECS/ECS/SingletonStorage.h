#pragma once

#include "Core/Set.h"
#include "ECS/SingletonContainer.h"
#include "ECS/SingletonId.h"

namespace ecs
{
	class SingletonStorage
	{
		friend class EntityWorld;

		using Containers = SparseArray<ecs::SingletonId, ecs::ISingletonContainer*>;
		using Updated = Set<ecs::SingletonId>;

	public:
		ecs::ISingletonContainer& GetAt(const ecs::SingletonId typeId);
		const ecs::ISingletonContainer& GetAt(const ecs::SingletonId typeId) const;

		template<typename TSingleton>
		ecs::SingletonContainer<TSingleton>& GetAt();
		template<typename TSingleton>
		const ecs::SingletonContainer<TSingleton>& GetAt() const;

		template<typename TSingleton>
		bool WasUpdated() const;

		template<class TSystem>
		bool IsRegistered() const;

		template<typename TSingleton>
		void RegisterSingleton();

		template<typename TSingleton>
		auto GetSingleton() -> TSingleton&;

		template<typename TSingleton>
		void UpdateSingleton();

	private:
		void FlushChanges();

	public:
		Containers m_Containers = {};
		Updated m_UpdatedCurr = {};
		Updated m_UpdatedNext = {};
	};
}

#include "SingletonStorage.inl"