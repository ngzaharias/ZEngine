#pragma once

#include "Core/String.h"
#include "ECS/EventId.h"

class MemBuffer;

namespace ecs
{
	class EntityWorld;
}

namespace ecs
{
	struct TypeEvent
	{
		using Add = void(ecs::EntityWorld&, const MemBuffer&);
		using Read = void(ecs::EntityWorld&, MemBuffer&);

		Add* m_Add = nullptr;
		Read* m_Read = nullptr;

		str::String m_Name = {};
		ecs::EventId m_TypeId = -1;
	};
}
