#pragma once

#include "Core/String.h"

namespace str
{
	template <typename... TArgs>
	str::String Format(const char* format, TArgs... args);
}

#include "Format.inl"