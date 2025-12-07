#pragma once

#include "Core/Types.h"

namespace container
{
	enum class EMember : uint16
	{
		None = 0,
		// Reserved for user defined types
		Custom_Min = 100,
		Custom_Max = UINT16_MAX,
	};
}