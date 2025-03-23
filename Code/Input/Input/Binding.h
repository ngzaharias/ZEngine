#pragma once

#include "Core/Name.h"
#include "Core/Variant.h"
#include "Input/Key.h"

namespace input
{
	struct Binding
	{
		input::Key m_Key = EKeyboard::Unknown;
		str::Name m_Name = {};
		bool m_Consume = true;
	};
}