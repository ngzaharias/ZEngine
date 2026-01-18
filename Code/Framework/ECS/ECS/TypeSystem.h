#pragma once

#include "Core/String.h"
#include "ECS/SystemId.h"

namespace ecs
{
	struct TypeSystem
	{
		str::String m_Name = {};
		ecs::SystemId m_TypeId = -1;
	};
}
