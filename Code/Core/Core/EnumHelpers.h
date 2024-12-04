#pragma once

#include "Core/StringView.h"
#include "Core/Types.h"

#include <magic_enum/magic_enum.hpp>

template<typename TEnum>
constexpr int32 EnumToCount()
{
	return static_cast<int32>(magic_enum::enum_count<TEnum>());
}

template<typename TEnum>
constexpr int32 EnumToIndex(const TEnum value)
{
	return static_cast<int32>(*magic_enum::enum_index(value));
}

template<typename TEnum>
constexpr str::StringView EnumToString()
{
	return magic_enum::enum_type_name<TEnum>();
}

template<typename TEnum>
constexpr str::StringView EnumToString(const TEnum value)
{
	return magic_enum::enum_name(value);
}

template<typename TEnum>
constexpr auto EnumToValue(const TEnum value)
{
	return magic_enum::enum_integer<TEnum>(value);
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

template<typename TEnum>
constexpr TEnum IndexToEnum(const uint32 index)
{
	return magic_enum::enum_value<TEnum>(index);
}

template<typename TEnum>
constexpr TEnum StringToEnum(const str::StringView& value)
{
	return *magic_enum::enum_cast<TEnum>(value);
}

template<typename TEnum>
constexpr TEnum ValueToEnum(const int32 value)
{
	return *magic_enum::enum_cast<TEnum>(value);
}