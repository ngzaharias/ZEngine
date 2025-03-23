#pragma once

#include "Core/Variant.h"
#include "Input/Keyboard.h"
#include "Input/Mouse.h"

namespace input
{
	using Key = Variant<input::EKeyboard, input::EMouse>;
}