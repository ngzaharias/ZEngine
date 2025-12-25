#pragma once

#include "Core/TypeInfo.h"
#include "Core/SparseArray.h"
#include "ECS/ComponentId.h"
#include "ECS/ComponentEntry.h"
#include "ECS/ComponentTag.h"

class GameTime;

namespace ecs
{
	class ComponentRegistry
	{
	public:
		template<class TComponent>
		void Register();

		template<class TComponent>
		bool IsRegistered() const;
		bool IsRegistered(const ecs::ComponentId id) const;

		template<class TComponent>
		auto GetEntry() const -> const ecs::ComponentEntry&;
		auto GetEntry(const ecs::ComponentId id) const -> const ecs::ComponentEntry&;

	private:
		SparseArray<ecs::ComponentId, ecs::ComponentEntry> m_Entries = { };
	};
}

#include "ComponentRegistry.inl"

