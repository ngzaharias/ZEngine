#include "Core/StringHelpers.h"

#include <random>

// #todo: move functions like ToUpper/TrimLeft into the string class
// and replace the implementations here with a return value.

namespace
{
	std::mt19937_64 s_MT = std::mt19937_64(std::random_device{}());

	constexpr const char* s_GuidFormatCompact = "%08x%04hx%04hx%04hx%04hx%04hx%04hx";
	constexpr const char* s_GuidFormatHyphenated = "%08x-%04hx-%04hx-%04hx-%04hx%04hx%04hx";
}

bool str::Compare(const str::StringView& stringA, const str::StringView& stringB)
{
	return stringA.size() == stringB.size()
		&& std::equal(stringA.begin(), stringA.end(), stringB.begin(), stringB.end());
}

bool str::Compare_NoCase(const str::StringView& stringA, const str::StringView& stringB)
{
	return stringA.size() == stringB.size()
		&& std::equal(stringA.begin(), stringA.end(), stringB.begin(), stringB.end(), [](char a, char b) { return tolower(a) == tolower(b); });
}

bool str::Contains(const str::StringView& string, const str::StringView& substring)
{
	const auto search = std::search(string.begin(), string.end(), substring.begin(), substring.end());
	return search != string.end();
}

bool str::Contains_NoCase(const str::StringView& string, const str::StringView& substring)
{
	const auto search = std::search(string.begin(), string.end(), substring.begin(), substring.end(), [](char a, char b) { return tolower(a) == tolower(b); });
	return search != string.end();
}

bool str::ContainsAll(const str::StringView& string, const str::StringViews& substrings)
{
	for (const str::StringView& substring : substrings)
	{
		if (!Contains(string, substring))
			return false;
	}
	return true;
}

bool str::ContainsAll_NoCase(const str::StringView& string, const str::StringViews& substrings)
{
	for (const str::StringView& substring : substrings)
	{
		if (!Contains_NoCase(string, substring))
			return false;
	}
	return true;
}

bool str::ContainsAny(const str::StringView& string, const str::StringViews& substrings)
{
	for (const str::StringView& substring : substrings)
	{
		if (Contains(string, substring))
			return true;
	}
	return false;
}

bool str::ContainsAny_NoCase(const str::StringView& string, const str::StringViews& substrings)
{
	for (const str::StringView& substring : substrings)
	{
		if (Contains_NoCase(string, substring))
			return true;
	}
	return false;
}

bool str::Equals(const str::StringView& string, const str::StringView& substring)
{
	return std::equal(string.begin(), string.end(), substring.begin(), substring.end());
}

bool str::Equals_NoCase(const str::StringView& string, const str::StringView& substring)
{
	return std::equal(string.begin(), string.end(), substring.begin(), substring.end(), [](char a, char b) { return tolower(a) == tolower(b); });
}

byte str::HexToByte(char value)
{
	if (value > 47 && value < 58) return (byte)(value - 48); // 0-9
	if (value > 96 && value < 103) return (byte)(value - 87); // a-f
	if (value > 64 && value < 71) return (byte)(value - 55); // A-F
	return 0;
}

str::StringViews str::Split(const str::StringView& string, const str::StringView& delimiters /*= s_Delimiters*/)
{
	str::StringViews substrings;
	auto begin = string.find_first_not_of(delimiters, 0);
	auto end = string.find_first_of(delimiters, begin);
	while (begin != std::string::npos || end != std::string::npos)
	{
		substrings.Append(string.substr(begin, end - begin));
		begin = string.find_first_not_of(delimiters, end);
		end = string.find_first_of(delimiters, begin);
	}
	return substrings;
}

void str::Trim(str::String& string, const str::StringView& substring)
{
	TrimLeft(string, substring);
	TrimRight(string, substring);
}

void str::TrimLeft(str::String& string, const str::StringView& substring)
{
	const auto a_size = string.size();
	const auto b_size = substring.size();
	const auto begin = 0;
	const auto find = string.find(substring, begin);
	if (find == begin)
		string.erase(begin, b_size);
}

void str::TrimRight(str::String& string, const str::StringView& substring)
{
	const auto a_size = string.size();
	const auto b_size = substring.size();
	const auto begin = string.size() - std::min(a_size, b_size);
	const auto find = string.find(substring, begin);
	if (find == begin)
		string.erase(begin, a_size);
}

void str::TrimWhitespace(str::String& string)
{
	const auto begin = string.find_first_not_of(strWhitespace, 0);
	const auto end = string.find_last_not_of(strWhitespace) + 1;
	if (begin != std::string::npos && end != std::string::npos)
	{
		string = string.substr(begin, end - begin);
	}
	else
	{
		string.clear();
	}
}

void str::ToLower(str::String& string)
{
	std::transform(string.begin(), string.end(), string.begin(), ::tolower);
}

void str::ToUpper(str::String& string)
{
	std::transform(string.begin(), string.end(), string.begin(), ::toupper);
}

str::String str::ToString(const int32 index, const int32 width)
{
	str::String string;
	string = std::format("{}", index);
	string = str::String(width - std::min((size_t)width, string.length()), '0') + string;
	return string;
}
