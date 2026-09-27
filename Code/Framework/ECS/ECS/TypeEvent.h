#pragma once

#include "Core/String.h"
#include "Core/TypeInfo.h"

class MemBuffer;

namespace ecs
{
	class EventBuffer;
}

namespace ecs
{
	struct TypeEvent
	{
		TypeId m_GlobalId = -1;
		TypeId m_LocalId = -1;
		bool m_IsReplicated = false;

		using AddEvent = void(ecs::EventBuffer&, const MemBuffer&);
		AddEvent* m_AddEvent = nullptr;
	};

	template<typename TEvent>
	void AddEvent(ecs::EventBuffer& buffer, const MemBuffer& data)
	{
		TEvent& event = buffer.AddEvent<TEvent>();
		data.Read(event);
	}
}
