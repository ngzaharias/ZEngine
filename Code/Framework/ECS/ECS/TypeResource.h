#pragma once

#include "Core/String.h"
#include "Core/TypeInfo.h"

namespace ecs
{
	struct TypeResource
	{
		TypeId m_GlobalId = -1;
		TypeId m_LocalId = -1;
	};
}
