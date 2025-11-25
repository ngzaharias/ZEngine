#pragma once

#include "Core/SparseArray.h"

#include "ECS/Event.h"
#include "ECS/EventEntry.h"

class GameTime;

namespace ecs
{
	class EventRegistry
	{
	public:
		template<class TEvent>
		void Register();

		template<class TEvent>
		bool IsRegistered() const;
		bool IsRegistered(const ecs::EventId componentId) const;

		template<class TEvent>
		auto GetEntry() const -> const ecs::EventEntry&;
		auto GetEntry(const ecs::EventId eventId) const -> const ecs::EventEntry&;

	private:
		SparseArray<ecs::EventId, ecs::EventEntry> m_Entries = { };
	};
}

#include "EventRegistry.inl"

