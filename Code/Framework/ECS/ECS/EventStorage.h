#pragma once

#include "ECS/EventBuffer.h"

namespace ecs
{
	class EventStorage
	{
		friend class EntityWorld;

	public:
		template<class TEvent>
		void RegisterEvent();

		template<class TEvent, typename... TArgs>
		auto AddEvent(TArgs&&... args)->TEvent&;

		template<class TEvent>
		bool HasEvents() const;

		template<class TEvent>
		auto GetEvents() const -> decltype(auto);

	private:
		void FlushChanges();

	private:
		// events that were added locally and are being processed this frame
		ecs::EventBuffer m_BufferLocalCurr;
		// events that were added locally and will be processed next frame
		ecs::EventBuffer m_BufferLocalNext;
		// events that were added remotely and are being processed this frame
		ecs::EventBuffer m_BufferRemoteCurr;
	};
}

#include "EventStorage.inl"