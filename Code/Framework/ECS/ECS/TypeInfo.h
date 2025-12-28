#pragma once

#include "Core/String.h"
#include "ECS/TypeBaseEnum.h"

namespace ecs
{
	struct TypeInfo
	{
		ETypeBase m_Base = {};
		str::String m_Name = {};
	};
}