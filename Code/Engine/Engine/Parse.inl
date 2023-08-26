#pragma once

#include "Parse.h"

#pragma warning( push )
#pragma warning( disable : 4348 )

template <typename Type, typename std::enable_if_t<std::is_same<Type, bool>::value, Type>::type* = nullptr>
Type str::Parse(const StringView& string)
{
	return !string.IsEmpty() && str::Compare_NoCase(string, "true") == 0;
}

template <typename Type, typename std::enable_if<std::is_floating_point<Type>::value, Type>::type* = nullptr>
Type str::Parse(const StringView& string)
{
	Type value;
	const auto [ptr, ec] = std::from_chars(string.data(), string.data() + string.size(), value);
	switch (ec)
	{
	case std::errc():
		return value;
	default:
		return 0;
	}
}

template <typename Type, typename std::enable_if<std::is_integral<Type>::value, Type>::type* = nullptr>
Type str::Parse(const StringView& string, const int32 base /*= 10*/)
{
	Type value;
	const auto [ptr, ec] = std::from_chars(string.data(), string.data() + string.size(), value, base);
	switch (ec)
	{
	case std::errc():
		return value;
	default:
		return 0;
	}
}

#pragma warning( pop )
