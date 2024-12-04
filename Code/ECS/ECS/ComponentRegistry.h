#pragma once

#include "Core/SparseArray.h"

#include "ECS/Component.h"
#include "ECS/ComponentEntry.h"

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
		bool IsRegistered(const ecs::ComponentId componentId) const;

		template<class TComponent>
		auto GetEntry() const -> const ecs::ComponentEntry&;
		auto GetEntry(const ecs::ComponentId componentId) const -> const ecs::ComponentEntry&;

	private:
		SparseArray<ecs::ComponentId, ecs::ComponentEntry> m_Entries = { };
	};
}

#include "ComponentRegistry.inl"

