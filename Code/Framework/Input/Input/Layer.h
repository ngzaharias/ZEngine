#pragma once

#include "Core/Array.h"
#include "Core/Flags.h"
#include "Input/Binding.h"
#include "Math/Math.h"

namespace input
{
	enum EConsume : uint32
	{
		None     = 0,
		Keyboard = BIT32(0),
		Gamepad  = BIT32(1),
		Mouse    = BIT32(2),
	};

	struct Layer
	{
		int32 m_Priority = 0;
		Array<Binding> m_Bindings = {};
		Flags<EConsume> m_Consume = EConsume::None;
	};
}