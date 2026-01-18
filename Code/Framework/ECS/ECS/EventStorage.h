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

		template<class TEvent>
		auto GetEvents() const -> const Array<TEvent>&;

	private:
		void ConsumeAll(ecs::EventBuffer& buffer);
		void RemoveAll();

	private:
		ecs::EventBuffer m_Buffer;
	};
}

#include "EventStorage.inl"