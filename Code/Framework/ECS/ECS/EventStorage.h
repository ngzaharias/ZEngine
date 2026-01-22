#pragma once

#include "ECS/EventBuffer.h"

namespace ecs
{
	class EventStorage
	{
		friend class EntityWorld;
		friend class ReplicationHost;
		friend class ReplicationPeer;

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
		ecs::EventBuffer m_MainBufferCurr;
		// events that were added locally and will be processed next frame
		ecs::EventBuffer m_MainBufferNext;
		// events that were added remotely and are being processed this frame
		ecs::EventBuffer m_SyncBufferCurr;
	};
}

#include "EventStorage.inl"