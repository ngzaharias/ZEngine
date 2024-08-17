#pragma once

#include "Core/Guid.h"
#include "Core/Name.h"
#include "Math/Rotator.h"
#include "Math/Vector.h"

template<typename Type>
Type toml::Parse(const toml::Node& value, const Type& _default)
{
	if constexpr (std::is_enum<Type>::value)
	{
		const auto result = value.value<int64>();
		return result ? static_cast<Type>(*result) : _default;
	}
	else
	{
		const auto result = value.value<Type>();
		return result ? *result : _default;
	}
}

template<typename Type>
Type toml::Parse(const toml::Value& value, const str::StringView& member, const Type& _default)
{
	if constexpr (std::is_enum<Type>::value)
	{
		using EnumType = std::underlying_type<Type>;
		const auto result = value[member].value<EnumType>();
		if (!result)
			return _default;

		return static_cast<Type>(*result);
	}
	else
	{
		const auto result = value[member].value<Type>();
		if (!result)
			return _default;
		return *result;
	}
}

template<>
inline str::Guid toml::Parse(const toml::Value& value, const str::StringView& member, const str::Guid& _default)
{
	const auto result = value[member].value<str::String>();
	if (!result)
		return _default;

	const str::String string = *result;
	if (string.empty())
		return _default;

	return GUID(string);
}

template<>
inline str::Name toml::Parse(const toml::Value& value, const str::StringView& member, const str::Name& _default)
{
	const auto result = value[member].value<str::String>();
	if (!result)
		return _default;

	const str::String string = *result;
	if (string.empty())
		return _default;

	return NAME(string);
}

template<>
inline Rotator toml::Parse(const toml::Value& value, const str::StringView& member, const Rotator& _default)
{
	const auto result = value[member];
	if (!result || !result.is_table())
		return _default;

	Rotator rotator;
	rotator.m_Pitch = result["Pitch"].value<float>().value_or(0.f);
	rotator.m_Yaw = result["Yaw"].value<float>().value_or(0.f);
	rotator.m_Roll = result["Roll"].value<float>().value_or(0.f);
	return rotator;
}

template<>
inline Vector2f toml::Parse(const toml::Value& value, const str::StringView& member, const Vector2f& _default)
{
	const auto result = value[member];
	if (!result || !result.is_table())
		return _default;

	Vector2f vector;
	vector.x = result["X"].value<float>().value_or(0.f);
	vector.y = result["Y"].value<float>().value_or(0.f);
	return vector;
}

template<>
inline Vector2i toml::Parse(const toml::Value& value, const str::StringView& member, const Vector2i& _default)
{
	const auto result = value[member];
	if (!result || !result.is_table())
		return _default;

	Vector2i vector;
	vector.x = result["X"].value<int32>().value_or(0);
	vector.y = result["Y"].value<int32>().value_or(0);
	return vector;
}

template<>
inline Vector2u toml::Parse(const toml::Value& value, const str::StringView& member, const Vector2u& _default)
{
	const auto result = value[member];
	if (!result || !result.is_table())
		return _default;

	Vector2u vector;
	vector.x = result["X"].value<uint32>().value_or(0);
	vector.y = result["Y"].value<uint32>().value_or(0);
	return vector;
}

template<>
inline Vector3f toml::Parse(const toml::Value& value, const str::StringView& member, const Vector3f& _default)
{
	const auto result = value[member];
	if (!result || !result.is_table())
		return _default;

	Vector3f vector;
	vector.x = result["X"].value<float>().value_or(0.f);
	vector.y = result["Y"].value<float>().value_or(0.f);
	vector.z = result["Z"].value<float>().value_or(0.f);
	return vector;
}

template<>
inline Vector3i toml::Parse(const toml::Value& value, const str::StringView& member, const Vector3i& _default)
{
	const auto result = value[member];
	if (!result || !result.is_table())
		return _default;

	Vector3i vector;
	vector.x = result["X"].value<int32>().value_or(0);
	vector.y = result["Y"].value<int32>().value_or(0);
	vector.z = result["Z"].value<int32>().value_or(0);
	return vector;
}

template<>
inline Vector4f toml::Parse(const toml::Value& value, const str::StringView& member, const Vector4f& _default)
{
	const auto result = value[member];
	if (!result || !result.is_table())
		return _default;

	Vector4f vector;
	vector.x = result["X"].value<float>().value_or(0.f);
	vector.y = result["Y"].value<float>().value_or(0.f);
	vector.z = result["Z"].value<float>().value_or(0.f);
	vector.w = result["W"].value<float>().value_or(0.f);
	return vector;
}