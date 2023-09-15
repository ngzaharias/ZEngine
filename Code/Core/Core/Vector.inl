#include <Core/Math.h>

inline float Vector2f::Length() const noexcept
{
	return math::Sqrt(x * x + y * y);
}

inline constexpr float Vector2f::LengthSqr() const noexcept
{
	return x * x + y * y;
}

inline void Vector2f::Normalize() noexcept
{
	const float length = Length();
	if (length > KINDA_SMALL_FLOAT)
	{
		*this *= 1.f / length;
	}
	else
	{
		x = y = 0.f;
	}
}

inline void Vector2f::NormalizeUnsafe() noexcept
{
	*this *= 1.f / Length();
}

inline Vector2f Vector2f::Normalized() const noexcept
{
	Vector2f value(*this);
	value.Normalize();
	return value;
}

inline Vector2f Vector2f::NormalizedUnsafe() const noexcept
{
	Vector2f value(*this);
	value.NormalizeUnsafe();
	return value;
}

inline constexpr Vector3f Vector2f::X0Y() const noexcept
{
	return Vector3f(x, 0.f, y);
}

inline constexpr Vector3f Vector2f::XY0() const noexcept
{
	return Vector3f(*this, 0.f);
}

inline float Vector2f::Distance(const Vector2f& a, const Vector2f& b) noexcept
{
	return (b - a).Length();
}

inline constexpr float Vector2f::DistanceSqr(const Vector2f& a, const Vector2f& b) noexcept
{
	return (b - a).LengthSqr();
}

inline constexpr float Vector2f::Dot(const Vector2f& a, const Vector2f& b) noexcept
{
	return a.x * b.x + a.y * b.y;
}

inline constexpr Vector2f Vector2f::Divide(const Vector2f& a, const Vector2f& b) noexcept
{
	return Vector2f(a.x / b.x, a.y / b.y);
}

inline constexpr Vector2f Vector2f::Max(const Vector2f& a, const Vector2f& b) noexcept
{
	return Vector2f((a.x > b.x) ? a.x : b.x, (a.y > b.y) ? a.y : b.y);
}

inline constexpr Vector2f Vector2f::Min(const Vector2f& a, const Vector2f& b) noexcept
{
	return Vector2f((a.x < b.x) ? a.x : b.x, (a.y < b.y) ? a.y : b.y);
}

inline constexpr Vector2f Vector2f::Multiply(const Vector2f& a, const Vector2f& b) noexcept
{
	return Vector2f(a.x * b.x, a.y * b.y);
}

inline constexpr Vector2f Vector2f::Reflect(const Vector2f& vector, const Vector2f& normal) noexcept
{
	// -2 * (V dot N)*N + V
	const float dot2 = Dot(vector, normal) * -2.0f;
	return Multiply(Vector2f(dot2), normal) + vector;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

inline float Vector3f::Length() const noexcept
{
	return math::Sqrt(x * x + y * y + z * z);
}

inline float Vector3f::LengthXZ() const noexcept
{
	return math::Sqrt(x * x + z * z);
}

inline constexpr float Vector3f::LengthSqr() const noexcept
{
	return x * x + y * y + z * z;
}

inline constexpr float Vector3f::LengthXZSqr() const noexcept
{
	return x * x + z * z;
}

inline void Vector3f::Normalize() noexcept
{
	const float dot = Dot(*this, *this);
	if (dot > KINDA_SMALL_FLOAT)
	{
		*this *= 1.f / Length();
	}
	else
	{
		x = y = z = 0.f;
	}
}

inline void Vector3f::NormalizeUnsafe() noexcept
{
	*this *= 1.f / Length();
}

inline Vector3f Vector3f::Normalized() const noexcept
{
	Vector3f value(*this);
	value.Normalize();
	return value;
}

inline Vector3f Vector3f::NormalizedUnsafe() const noexcept
{
	Vector3f value(*this);
	value.NormalizeUnsafe();
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

inline constexpr Vector3f Vector3f::Cross(const Vector3f& a, const Vector3f& b) noexcept
{
	return Vector3f(
		(a.y * b.z) - (a.z * b.y),
		(a.z * b.x) - (a.x * b.z), 
		(a.x * b.y) - (a.y * b.x));
}

inline float Vector3f::Distance(const Vector3f& a, const Vector3f& b) noexcept
{
	return (b - a).Length();
}

inline float Vector3f::DistanceXZ(const Vector3f& a, const Vector3f& b) noexcept
{
	return (b - a).LengthXZ();
}

inline constexpr float Vector3f::DistanceSqr(const Vector3f& a, const Vector3f& b) noexcept
{
	return (b - a).LengthSqr();
}

inline constexpr float Vector3f::DistanceXZSqr(const Vector3f& a, const Vector3f& b) noexcept
{
	return (b - a).LengthXZSqr();
}

inline constexpr float Vector3f::Dot(const Vector3f& a, const Vector3f& b) noexcept
{
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

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
	const float length = Length();
	if (length > KINDA_SMALL_FLOAT)
	{
		*this *= 1.f / length;
	}
	else
	{
		x = y = z = w = 0.f;
	}
}

inline void Vector4f::NormalizeUnsafe() noexcept
{
	*this *= 1.f / Length();
}

inline Vector4f Vector4f::Normalized() const noexcept
{
	Vector4f value(*this);
	value.Normalize();
	return value;
}

inline Vector4f Vector4f::NormalizedUnsafe() const noexcept
{
	Vector4f value(*this);
	value.NormalizeUnsafe();
	return value;
}