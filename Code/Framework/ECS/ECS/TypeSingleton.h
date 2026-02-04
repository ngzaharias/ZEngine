#pragma once

#include "Core/String.h"
#include "ECS/SingletonId.h"

class MemBuffer;

namespace ecs
{
	class SingletonStorage;
}

namespace ecs
{
	struct TypeSingleton
	{
		str::String m_Name = {};
		ecs::SingletonId m_TypeId = -1;
	};
}
