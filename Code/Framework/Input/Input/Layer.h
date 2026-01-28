#pragma once

#include "Core/Array.h"
#include "Core/Delegate.h"
#include "Core/Flags.h"
#include "Core/Set.h"
#include "Input/Binding.h"
#include "Math/Math.h"

class Vector2f;

namespace input
{
	enum class EKey;
}

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
		using Callback = Delegate<void(
			const Vector2f& mousePos,
			const Vector2f& mouseDelta,
			const Vector2f& scrollDelta,
			Set<input::EKey>& held, 
			Set<input::EKey>& pressed, 
			Set<input::EKey>& released)>;

		int32 m_Priority = 0;
		Array<Binding> m_Bindings = {};
		Flags<EConsume> m_Consume = EConsume::None;

		Callback m_Callback = {};
	};
}