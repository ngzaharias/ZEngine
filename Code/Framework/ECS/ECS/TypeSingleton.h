#pragma once

#include "Core/String.h"
#include "Core/TypeInfo.h"

class MemBuffer;

namespace ecs
{
	class SingletonStorage;
}

namespace ecs
{
	struct TypeSingleton
	{
		TypeId m_GlobalId = -1;
		TypeId m_LocalId = -1;
	};
}
