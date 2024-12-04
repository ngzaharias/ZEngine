#pragma once

#include "Core/Optional.h"
#include "Core/StringHelpers.h"
#include "Core/StringView.h"
#include "Core/Types.h"

#include <type_traits>

namespace str
{
	/// \brief Parse a string to a boolean.
	template <typename Type, typename std::enable_if_t<std::is_same<Type, bool>::value, Type>::type* = nullptr>
	Optional<Type> Parse(const str::StringView& string);

	/// \brief Parse a string to an enum.
	/// Works by converting it to int32 first and then static_cast to the enum type.
	template <typename Type, typename std::enable_if_t<std::is_enum<Type>::value, Type>::type* = nullptr>
	Optional<Type> ParseEnum(const str::StringView& string);

	/// \brief Parse a string to a floating point type.
	template <typename Type, typename std::enable_if<std::is_floating_point<Type>::value, Type>::type* = nullptr>
	Optional<Type> Parse(const str::StringView& string);

	/// \brief Parse a string to an integral type.
	template <typename Type, typename std::enable_if<std::is_integral<Type>::value, Type>::type* = nullptr>
	Optional<Type> Parse(const str::StringView& string, const int32 base = 10);
}

#include "Parse.inl"