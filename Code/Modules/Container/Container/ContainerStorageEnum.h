#pragma once

#include "Core/Set.h"
#include "ECS/Component.h"
#include "ECS/Entity.h"

namespace container
{
	/// \brief 
	enum class EStorage : uint16
	{
		Main = 0,
		// Reserved for user defined types
		Custom_Min = 100,
		Custom_Max = UINT16_MAX,
	};
}