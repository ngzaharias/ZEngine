#pragma once

#include "Core/String.h"
#include "ECS/ResourceId.h"

namespace ecs
{
	struct TypeResource
	{
		str::String m_Name = {};
		ecs::ResourceId m_TypeId = -1;
	};
}
