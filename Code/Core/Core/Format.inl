#pragma once

#include <stdio.h>

template <typename... TArgs>
str::String str::Format(const char* format, TArgs... args)
{
	const int length = std::snprintf(nullptr, 0, format, args...);

	str::String result;
	result.resize(static_cast<size_t>(length));

	std::snprintf(&result[0], static_cast<size_t>(length) + 1, format, args...);
	return result;
}
