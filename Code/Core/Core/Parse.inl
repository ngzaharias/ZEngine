#pragma once

#include <charconv>

#pragma warning( push )
#pragma warning( disable : 4348 )

template <typename Type, typename std::enable_if_t<std::is_same<Type, bool>::value, Type>::type*>
Optional<Type> str::Parse(const StringView& string)
{
	if (str::Compare_NoCase(string, "true"))
		return true;
	if (str::Compare_NoCase(string, "false"))
		return false;
	return { };
}

template <typename Type, typename std::enable_if_t<std::is_enum<Type>::value, Type>::type*>
Optional<Type> str::ParseEnum(const str::StringView& string)
{
	const auto result = str::Parse<int32>(string);
	return result 
		? static_cast<Type>(*result) 
		: Optional<Type>();
}

template <typename Type, typename std::enable_if<std::is_floating_point<Type>::value, Type>::type*>
Optional<Type> str::Parse(const StringView& string)
{
	// #note: std::from_chars doesn't handle '+'
	const int32 offset = !string.empty() && string.at(0) == '+' ? 1 : 0;

	Type value;
	const int32 start = offset;
	const int32 finish = offset + static_cast<int32>(string.size());
	const auto [ptr, ec] = std::from_chars(string.data() + start, string.data() + finish, value);
	switch (ec)
	{
	case std::errc():
		return value;
	default:
		return { };
	}
}

template <typename Type, typename std::enable_if<std::is_integral<Type>::value, Type>::type*>
Optional<Type> str::Parse(const StringView& string, const int32 base /*= 10*/)
{
	// #note: std::from_chars doesn't handle '+'
	const int32 offset = !string.empty() && string.at(0) == '+' ? 1 : 0;

	Type value;
	const int32 start = offset;
	const int32 finish = offset + static_cast<int32>(string.size());
	const auto [ptr, ec] = std::from_chars(string.data() + start, string.data() + finish, value, base);
	switch (ec)
	{
	case std::errc():
		return value;
	default:
		return { };
	}
}

#pragma warning( pop )
