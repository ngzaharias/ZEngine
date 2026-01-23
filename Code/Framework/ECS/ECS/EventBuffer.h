#pragma once

#include "Core/SparseArray.h"
#include "ECS/EventId.h"
#include "ECS/EventContainer.h"
#include "ECS/EventTag.h"

namespace ecs
{
	class EventBuffer final
	{
		using Containers = SparseArray<ecs::EventId, ecs::IEventContainer*>;

	public:
		template<class TEvent>
		void RegisterEvent();

		template<class TEvent, typename... TArgs>
		auto AddEvent(TArgs&&... args)->TEvent&;

		Containers& GetAll();
		const Containers& GetAll() const;

		ecs::IEventContainer& GetAt(const ecs::EventId typeId);
		const ecs::IEventContainer& GetAt(const ecs::EventId typeId) const;

		template<typename TEvent>
		ecs::EventContainer<TEvent>& GetAt();
		template<typename TEvent>
		const ecs::EventContainer<TEvent>& GetAt() const;

		ecs::IEventContainer* TryAt(const ecs::EventId typeId);
		const ecs::IEventContainer* TryAt(const ecs::EventId typeId) const;

		template<typename TEvent>
		ecs::EventContainer<TEvent>* TryAt();
		template<typename TEvent>
		const ecs::EventContainer<TEvent>* TryAt() const;

	private:
		Containers m_Containers = {};
	};
}

#include "EventBuffer.inl"
