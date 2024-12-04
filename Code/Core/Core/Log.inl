#pragma once

#include "Core/String.h"
#include <format>

template <typename... TArgs>
inline void core::LogFormatted(const ELog channel, const char* format, TArgs&&... args)
{
	if constexpr (sizeof...(args) > 0)
	{
		const str::String string = std::vformat(format, std::make_format_args(args...));
		LogMessage(channel, string.c_str());
	}
	else
	{
		LogMessage(channel, format);
	}
}
