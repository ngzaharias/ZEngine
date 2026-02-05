#pragma once

#include "Core/String.h"
#include "ECS/TypeBaseEnum.h"

namespace ecs
{
	struct TypeInfo
	{
		str::String m_Name = {};

		ETypeBase m_Base = {};
		TypeId m_LocalId = -1;
	};
}