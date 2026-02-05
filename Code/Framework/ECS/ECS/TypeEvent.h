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

		using Add = void(ecs::EventBuffer&, const MemBuffer&);
		Add* m_Add = nullptr;
	};
}
