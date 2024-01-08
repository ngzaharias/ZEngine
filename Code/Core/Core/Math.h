#pragma once

#include <cmath>

using int32 = int32_t;

constexpr float KINDA_LARGE_FLOAT = 9999999.0f;
constexpr float KINDA_SMALL_FLOAT = 0.0000001f;

constexpr float PI_TWO  = 6.2831853071795864769252867665590f;
constexpr float PI_ONE  = 3.1415926535897932384626433832795f;
constexpr float PI_HALF = 1.5707963267948966192313216916398f;

constexpr float SQUARE_ROOT_TWO = 1.4142135623730950488016887242097f;
constexpr float SQUARE_ROOT_THREE = 1.732050807568877f;

constexpr float DIAGONAL_1D = 1.f;
constexpr float DIAGONAL_2D = SQUARE_ROOT_TWO;
constexpr float DIAGONAL_3D = SQUARE_ROOT_THREE;

// https://cplusplus.com/forum/articles/3638/

namespace math
{
	/// \brief Clamps value between min and max so that it doesn't exceed either.
	template<typename Type = float>
	inline constexpr Type Clamp(const Type& value, const Type& min, const Type& max) noexcept
	{
		return value < min ? min : value > max ? max : value;
	}

	/// \brief Rounds value to the nearest whole value towards +infinity.
	template<typename Type = float>
	inline Type Ceiling(const float value) noexcept
	{
		return static_cast<Type>(std::ceilf(value));
	}

	/// \brief Rounds value to the nearest whole value towards +infinity to the nearest multiplier.
	template<typename Type = float>
	inline Type Ceiling(const float value, const float multiplier) noexcept
	{
		return static_cast<Type>(std::ceilf(value / multiplier)) * multiplier;
	}

	/// \brief Rounds value to the nearest whole value towards -infinity.
	template<typename Type = float>
	inline Type Floor(const float value) noexcept
	{
		return static_cast<Type>(std::floorf(value));
	}

	/// \brief Rounds value to the nearest whole value towards -infinity to the nearest multiplier.
	template<typename Type = float>
	inline Type Floor(const float value, const float multiplier) noexcept
	{
		return static_cast<Type>(std::floorf(value / multiplier)) * multiplier;
	}

	/// \brief Linearly interpolations from a -> b based on t.
	template<typename Type = float>
	inline constexpr Type Lerp(const Type& a, const Type& b, const float t) noexcept
	{
		return a + (b - a) * t;
	}

	/// \brief Returns the smaller number of the two.
	template<typename Type = float>
	inline constexpr Type Min(const Type& a, const Type& b) noexcept
	{
		return (a < b) ? a : b;
	}

	/// \brief Returns the bigger number of the two.
	template<typename Type = float>
	inline constexpr Type Max(const Type& a, const Type& b) noexcept
	{
		return (a > b) ? a : b;
	}

	/// \brief Converts value from one range to another range.
	template<typename Type = float>
	inline constexpr Type Remap(Type value, const Type& fromA, const Type& fromB, const Type& toA, const Type& toB) noexcept
	{
		value -= fromA;
		value /= (fromB - fromA);
		return value * (toB - toA) + toA;
	}

	/// \brief Rounds the value towards the nearest whole number away from zero.
	template<typename Type = float>
	inline Type Round(const float value) noexcept
	{
		return static_cast<Type>(std::roundf(value));
	}

	/// \brief Rounds the value towards the nearest multiplier away from zero.
	template<typename Type = float>
	inline Type Round(const float value, const float multiplier) noexcept
	{
		return Round<Type>(value / multiplier) * multiplier;
	}

	/// \brief Returns -1 for a negative number and +1 for a positive number.
	inline constexpr float Sign(float value) noexcept
	{
		return (value < 0.0f) ? -1.0f : 1.0f;
	}

	/// \brief Returns the squared value of the value.
	template<typename Type = float>
	inline constexpr float Sqr(const Type value) noexcept
	{
		return value * value;
	}

	/// \brief Returns the squared root of the value.
	inline float Sqrt(const float value) noexcept
	{
		return sqrtf(value);
	}

	/// \brief Converts a radian to an euler angle.
	inline constexpr float ToDegrees(float radians) noexcept
	{
		return radians * 57.2958f;
	}

	/// \brief Converts a an euler angle to a radian.
	inline constexpr float ToRadians(float degrees) noexcept
	{
		return degrees * 0.0174533f;
	}
}
