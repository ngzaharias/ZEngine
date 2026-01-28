#pragma once

#include "Core/String.h"
#include "ECS/EventId.h"

class MemBuffer;

namespace ecs
{
	class EventBuffer;
}

namespace ecs
{
	struct TypeEvent
	{
		using Add = void(ecs::EventBuffer&, const MemBuffer&);

		Add* m_Add = nullptr;

		str::String m_Name = {};
		ecs::EventId m_TypeId = -1;
	};
}
