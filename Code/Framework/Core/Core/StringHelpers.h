#pragma once

#include "Core/Array.h"
#include "Core/String.h"
#include "Core/StringView.h"
#include "Core/Tuple.h"
#include "Core/Types.h"

namespace str
{
	using StringViews = Array<str::StringView>;

	constexpr str::StringView strDelimiters = " .,:;'\"<>[]{}()\\|/";
	constexpr str::StringView strWhitespace = " \t\f\v\n\r";

	bool Compare(const str::StringView& stringA, const str::StringView& stringB);
	bool Compare_NoCase(const str::StringView& stringA, const str::StringView& stringB);

	bool Contains(const str::StringView& string, const str::StringView& substring);
	bool Contains_NoCase(const str::StringView& string, const str::StringView& substring);

	bool ContainsAll(const str::StringView& string, const str::StringViews& substrings);
	bool ContainsAll_NoCase(const str::StringView& string, const str::StringViews& substrings);
	bool ContainsAny(const str::StringView& string, const str::StringViews& substrings);
	bool ContainsAny_NoCase(const str::StringView& string, const str::StringViews& substrings);

	bool Equals(const str::StringView& string, const str::StringView& substring);
	bool Equals_NoCase(const str::StringView& string, const str::StringView& substring);

	byte HexToByte(char value);

	/// \brief Splits a string into multiple substrings using the provided delimiters.
	str::StringViews Split(const str::StringView& string, const str::StringView& delimiters = strDelimiters);

	/// \brief Trims the substring from the left and right of the string.
	/// String: "MY STRING MY"
	/// Substring: "MY"
	/// Result: " STRING "
	void Trim(str::String& string, const str::StringView& substring);
	/// \brief Trims the substring from the left of the string.
	/// String: "MY STRING MY"
	/// Substring: "MY"
	/// Result: " STRING MY"
	void TrimLeft(str::String& string, const str::StringView& substring);
	/// \brief Trims the substring from the right of the string.
	/// String: "MY STRING MY"
	/// Substring: "MY"
	/// Result: "MY STRING "
	void TrimRight(str::String& string, const str::StringView& substring);

	/// \brief Trims all whitespace before the first and after the last non-whitespace character.
	/// String: " MY STRING " 
	/// Result: "MY STRING"
	void TrimWhitespace(str::String& string);

	/// \brief Converts all characters to their lowercase counterpart if it exists.
	/// String: "My StRiNg"
	/// Result: "my string"
	void ToLower(str::String& string);
	/// \brief Converts all characters to their uppercase counterpart if it exists.
	/// String: "My StRiNg"
	/// Result: "MY STRING" 
	void ToUpper(str::String& string);

	/// \brief Converts an integer to a string with leading zeros.
	/// Index:	7
	/// Width:  3
	/// Result: "007"
	str::String ToString(const int32 index, const int32 width);
}
