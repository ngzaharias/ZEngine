#pragma once

#include "ECS/EventBuffer.h"

namespace ecs
{
	class EventStorage
	{
	public:
		template<class TEvent>
		void RegisterEvent();

		template<class TEvent, typename... TArgs>
		auto AddEvent(TArgs&&... args)->TEvent&;

		template<class TEvent>
		bool HasEvents() const;

		template<class TEvent>
		auto GetEvents() const -> decltype(auto);

		auto GetMainBuffer() -> ecs::EventBuffer&;
		auto GetMainBuffer() const -> const ecs::EventBuffer&;

		auto GetNextBuffer() -> ecs::EventBuffer&;
		auto GetNextBuffer() const -> const ecs::EventBuffer&;

		auto GetSyncBuffer() -> ecs::EventBuffer&;
		auto GetSyncBuffer() const -> const ecs::EventBuffer&;

		void FlushChanges();

	private:
		// events that were added locally on the previous frame
		ecs::EventBuffer m_MainBuffer;
		// events that were added locally for the next frame
		ecs::EventBuffer m_NextBuffer;
		// events that were added remotely for the next frame
		ecs::EventBuffer m_SyncBuffer;
	};
}

#include "EventStorage.inl"