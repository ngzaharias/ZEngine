#pragma once

#include "Core/String.h"
#include "ECS/SingletonId.h"

class MemBuffer;

namespace ecs
{
	class EntityWorld;
}

namespace ecs
{
	struct TypeSingleton
	{
		using Read = void(ecs::EntityWorld&, MemBuffer&);
		using Write = void(ecs::EntityWorld&, const MemBuffer&);

		Read* m_Read = nullptr;
		Write* m_Write = nullptr;

		str::String m_Name = {};
		ecs::SingletonId m_TypeId = -1;
	};
}
