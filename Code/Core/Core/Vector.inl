#include <Core/Math.h>

#ifdef VECTOR_ASSERTS_ENABLED
#include <cassert>
#define VECTOR_ASSERT(condition, ...) assert(condition)
#else
#define VECTOR_ASSERT(...)
#endif

inline float Vector2f::Length() const noexcept
{
	return math::Sqrt(x * x + y * y);
}

inline constexpr float Vector2f::LengthSqr() const noexcept
{
	return x * x + y * y;
}

inline void Vector2f::Limit(const float value) noexcept
{
	const float length = Length();
	if (length > value)
		*this *= (value / length);

	VECTOR_ASSERT(!std::isnan(x), "Vector2f::Limit resulted in NaN!");
}

inline void Vector2f::Normalize() noexcept
{
	*this *= 1.f / Length();
	VECTOR_ASSERT(!std::isnan(x), "Vector2f::Normalize resulted in NaN!");
}

inline Vector2f Vector2f::Limited(const float value) const noexcept
{
	Vector2f result(*this);
	result.Limit(value);
	return result;
}

inline Vector2f Vector2f::Normalized() const noexcept
{
	Vector2f result(*this);
	result.Normalize();
	return result;
}

inline constexpr Vector3f Vector2f::X0Y() const noexcept
{
	return Vector3f(x, 0.f, y);
}

inline constexpr Vector3f Vector2f::XY0() const noexcept
{
	return Vector3f(*this, 0.f);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline float Vector3f::Length() const noexcept
{
	return math::Sqrt(x * x + y * y + z * z);
}

inline float Vector3f::LengthXZ() const noexcept
{
	return math::Sqrt(x * x + z * z);
}

inline float Vector3f::LengthXY() const noexcept
{
	return math::Sqrt(x * x + y * y);
}

inline constexpr float Vector3f::LengthSqr() const noexcept
{
	return x * x + y * y + z * z;
}

inline constexpr float Vector3f::LengthXYSqr() const noexcept
{
	return x * x + y * y;
}

inline constexpr float Vector3f::LengthXZSqr() const noexcept
{
	return x * x + z * z;
}

inline void Vector3f::Normalize() noexcept
{
	*this *= 1.f / Length();
	VECTOR_ASSERT(!std::isnan(x), "Vector3f::Normalize resulted in NaN!");
}

inline Vector3f Vector3f::Normalized() const noexcept
{
	Vector3f value(*this);
	value.Normalize();
	return value;
}

inline constexpr Vector2f Vector3f::XY() const noexcept
{
	return Vector2f(x, y);
}

inline constexpr Vector2f Vector3f::XZ() const noexcept
{
	return Vector2f(x, z);
}

inline constexpr Vector3f Vector3f::X0Z() const noexcept
{
	return Vector3f(x, 0.f, z);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline float Vector4f::Length() const noexcept
{
	return math::Sqrt(x * x + y * y + z * z + w * w);
}

inline constexpr float Vector4f::LengthSqr() const noexcept
{
	return x * x + y * y + z * z + w * w;
}

inline void Vector4f::Normalize() noexcept
{
	*this *= 1.f / Length();
	VECTOR_ASSERT(!std::isnan(x), "Vector4f::Normalize resulted in NaN!");
}

inline Vector4f Vector4f::Normalized() const noexcept
{
	Vector4f value(*this);
	value.Normalize();
	return value;
}

#undef VECTOR_ASSERT