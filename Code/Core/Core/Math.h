#pragma once

#include <cmath>

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

namespace math
{
	/// \brief 
	template<typename Type>
	inline constexpr Type Clamp(const Type& value, const Type& min, const Type& max)
	{
		return value < min ? min : value > max ? max : value;
	}

	/// \brief 
	template<typename Type>
	inline constexpr Type Ceiling(const float value)
	{
		return static_cast<Type>(std::ceilf(value));
	}

	/// \brief 
	template<typename Type>
	inline constexpr Type Floor(const float value)
	{
		return static_cast<Type>(std::floorf(value));
	}

	/// \brief 
	template<typename Type>
	inline constexpr Type Lerp(const Type& a, const Type& b, const float t)
	{
		return a + (b - a) * t;
	}

	/// \brief 
	template<typename Type>
	inline constexpr Type Min(const Type& a, const Type& b)
	{
		return (a < b) ? a : b;
	}

	/// \brief 
	template<typename Type>
	inline constexpr Type Max(const Type& a, const Type& b)
	{
		return (a > b) ? a : b;
	}

	/// \brief 
	template<typename Type>
	inline constexpr Type Remap(Type value, const Type& fromA, const Type& fromB, const Type& toA, const Type& toB)
	{
		value -= fromA;
		value /= (fromB - fromA);
		return value * (toB - toA) + toA;
	}

	/// \brief 
	template<typename Type>
	inline constexpr Type Round(const float value)
	{
		return static_cast<Type>(std::roundf(value));
	}

	/// \brief 
	inline constexpr float Sign(float value)
	{
		return (value < 0.0f) ? -1.0f : 1.0f;
	}

	/// \brief 
	template<typename Type>
	inline constexpr float Sqr(const Type value)
	{
		return value * value;
	}

	/// \brief 
	inline float Sqrt(const float value)
	{
		return sqrtf(value);
	}

	/// \brief 
	inline constexpr float ToDegrees(float radians)
	{
		return radians * 57.2958f;
	}

	/// \brief 
	inline constexpr float ToRadians(float degrees)
	{
		return degrees * 0.0174533f;
	}
}
