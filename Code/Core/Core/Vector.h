#pragma once

#include <Core/Defines.h>
#include <Core/Types.h>

class Vector2f;
class Vector3f;
class Vector4f;

/// \brief
class Vector2f
{
public:
	constexpr Vector2f() noexcept : x(), y() {}
	constexpr explicit Vector2f(const float value) noexcept : x(value), y(value) {}
	constexpr explicit Vector2f(const float x, const float y) noexcept : x(x), y(y) {}

	constexpr bool operator==(const Vector2f& rhs) const noexcept { return (x == rhs.x) && (y == rhs.y); }
	constexpr bool operator!=(const Vector2f& rhs) const noexcept { return (x != rhs.x) || (y != rhs.y); }

	constexpr Vector2f operator+(const Vector2f& rhs) const noexcept { return Vector2f(x + rhs.x, y + rhs.y); }
	constexpr Vector2f operator-(const Vector2f& rhs) const noexcept { return Vector2f(x - rhs.x, y - rhs.y); }

	constexpr Vector2f& operator+=(const Vector2f& rhs) noexcept { x += rhs.x; y += rhs.y; return *this; }
	constexpr Vector2f& operator-=(const Vector2f& rhs) noexcept { x -= rhs.x; y -= rhs.y; return *this; }

	constexpr Vector2f operator*(const float rhs) const noexcept { return Vector2f(x * rhs, y * rhs); }
	constexpr Vector2f operator/(const float rhs) const noexcept { return Vector2f(x / rhs, y / rhs); }

	constexpr Vector2f& operator*=(const float rhs) noexcept { x *= rhs; y *= rhs; return *this; }
	constexpr Vector2f& operator/=(const float rhs) noexcept { x /= rhs; y /= rhs; return *this; }

	constexpr Vector2f operator+() const noexcept { return *this; }
	constexpr Vector2f operator-() const noexcept { return Vector2f(-x, -y); }

	float Length() const noexcept;
	constexpr float LengthSqr() const noexcept;

	void Normalize() noexcept;
	void NormalizeUnsafe() noexcept;

	[[nodiscard]] Vector2f Normalized() const noexcept;
	[[nodiscard]] Vector2f NormalizedUnsafe() const noexcept;

	constexpr Vector3f X0Y() const noexcept;
	constexpr Vector3f XY0() const noexcept;

	static float Distance(const Vector2f& a, const Vector2f& b) noexcept;
	static constexpr float DistanceSqr(const Vector2f& a, const Vector2f& b) noexcept;
	static constexpr float Dot(const Vector2f& a, const Vector2f& b) noexcept;
	static constexpr Vector2f Divide(const Vector2f& a, const Vector2f& b) noexcept;
	static constexpr Vector2f Multiply(const Vector2f& a, const Vector2f& b) noexcept;
	static constexpr Vector2f Reflect(const Vector2f& vector, const Vector2f& normal) noexcept;

	static const Vector2f AxisX;
	static const Vector2f AxisY;
	static const Vector2f One;
	static const Vector2f Zero;

public:
	float x, y;
};

inline constexpr Vector2f Vector2f::AxisX(1.f, 0.f);
inline constexpr Vector2f Vector2f::AxisY(0.f, 1.f);
inline constexpr Vector2f Vector2f::One(1.f);
inline constexpr Vector2f Vector2f::Zero(0.f);

/// \brief
class Vector2i
{
public:
	constexpr Vector2i()  noexcept : x(), y() {}
	constexpr explicit Vector2i(const int32 value)  noexcept : x(value), y(value) {}
	constexpr explicit Vector2i(const int32 x, const int32 y)  noexcept : x(x), y(y) {}

	constexpr bool operator==(const Vector2i& rhs) const noexcept { return (x == rhs.x) && (y == rhs.y); }
	constexpr bool operator!=(const Vector2i& rhs) const noexcept { return (x != rhs.x) || (y != rhs.y); }

	constexpr Vector2i operator+() const noexcept { return *this; }
	constexpr Vector2i operator-() const noexcept { return Vector2i(-x, -y); }

	static const Vector2i AxisX;
	static const Vector2i AxisY;
	static const Vector2i One;
	static const Vector2i Zero;

public:
	int32 x, y;
};

inline constexpr Vector2i Vector2i::AxisX(1, 0);
inline constexpr Vector2i Vector2i::AxisY(0, 1);
inline constexpr Vector2i Vector2i::One(1);
inline constexpr Vector2i Vector2i::Zero(0);

/// \brief
class Vector2u
{
public:
	constexpr Vector2u() noexcept : x(), y() {}
	constexpr explicit Vector2u(const uint32 value) noexcept : x(value), y(value) {}
	constexpr explicit Vector2u(const uint32 x, const uint32 y) noexcept : x(x), y(y) {}

	constexpr bool operator==(const Vector2u& rhs) const noexcept { return (x == rhs.x) && (y == rhs.y); }
	constexpr bool operator!=(const Vector2u& rhs) const noexcept { return (x != rhs.x) || (y != rhs.y); }

	static const Vector2u AxisX;
	static const Vector2u AxisY;
	static const Vector2u One;
	static const Vector2u Zero;

public:
	uint32 x, y;
};

inline constexpr Vector2u Vector2u::AxisX(1, 0);
inline constexpr Vector2u Vector2u::AxisY(0, 1);
inline constexpr Vector2u Vector2u::One(1);
inline constexpr Vector2u Vector2u::Zero(0);

/// \brief
class Vector3f
{
public:
	constexpr Vector3f() noexcept : x(), y(), z() {}
	constexpr explicit Vector3f(const float value) noexcept : x(value), y(value), z(value) {}
	constexpr explicit Vector3f(const float x, const float y, const float z) noexcept : x(x), y(y), z(z) {}
	constexpr explicit Vector3f(const Vector2f& value, const float z) noexcept : x(value.x), y(value.y), z(z) {}

	constexpr bool operator==(const Vector3f& rhs) const noexcept { return (x == rhs.x) && (y == rhs.y) && (z == rhs.z); }
	constexpr bool operator!=(const Vector3f& rhs) const noexcept { return (x != rhs.x) || (y != rhs.y) || (z != rhs.z); }

	constexpr Vector3f operator+(const Vector3f& rhs) const noexcept { return Vector3f(x + rhs.x, y + rhs.y, z + rhs.z); }
	constexpr Vector3f operator-(const Vector3f& rhs) const noexcept { return Vector3f(x - rhs.x, y - rhs.y, z - rhs.z); }

	constexpr Vector3f& operator+=(const Vector3f& rhs) noexcept { x += rhs.x; y += rhs.y; z += rhs.z; return *this; }
	constexpr Vector3f& operator-=(const Vector3f& rhs) noexcept { x -= rhs.x; y -= rhs.y; z -= rhs.z; return *this; }

	constexpr Vector3f operator*(const float rhs) const noexcept { return Vector3f(x * rhs, y * rhs, z * rhs); }
	constexpr Vector3f operator/(const float rhs) const noexcept { return Vector3f(x / rhs, y / rhs, z / rhs); }

	constexpr Vector3f& operator*=(const float rhs) noexcept { x *= rhs; y *= rhs; z *= rhs; return *this; }
	constexpr Vector3f& operator/=(const float rhs) noexcept { x /= rhs; y /= rhs; z /= rhs; return *this; }

	constexpr Vector3f operator+() const noexcept { return *this; }
	constexpr Vector3f operator-() const noexcept { return Vector3f(-x, -y, -z); }

	float Length() const noexcept;
	float LengthXZ() const noexcept;
	constexpr float LengthSqr() const noexcept;
	constexpr float LengthXZSqr() const noexcept;

	void Normalize() noexcept;
	void NormalizeUnsafe() noexcept;

	[[nodiscard]] Vector3f Normalized() const noexcept;
	[[nodiscard]] Vector3f NormalizedUnsafe() const noexcept;

	constexpr Vector2f XY() const noexcept;
	constexpr Vector2f XZ() const noexcept;
	constexpr Vector3f X0Z() const noexcept;

	static constexpr Vector3f Cross(const Vector3f& a, const Vector3f& b) noexcept;
	static float Distance(const Vector3f& a, const Vector3f& b) noexcept;
	static float DistanceXZ(const Vector3f& a, const Vector3f& b) noexcept;
	static constexpr float DistanceSqr(const Vector3f& a, const Vector3f& b) noexcept;
	static constexpr float DistanceXZSqr(const Vector3f& a, const Vector3f& b) noexcept;
	static constexpr float Dot(const Vector3f& a, const Vector3f& b) noexcept;

	static const Vector3f AxisX;
	static const Vector3f AxisY;
	static const Vector3f AxisZ;
	static const Vector3f One;
	static const Vector3f Zero;

public:
	float x, y, z;
};

inline constexpr Vector3f Vector3f::AxisX(1.f, 0.f, 0.f);
inline constexpr Vector3f Vector3f::AxisY(0.f, 1.f, 0.f);
inline constexpr Vector3f Vector3f::AxisZ(0.f, 0.f, 1.f);
inline constexpr Vector3f Vector3f::One(1.f);
inline constexpr Vector3f Vector3f::Zero(0.f);

/// \brief
class Vector3i
{
public:
	constexpr Vector3i() noexcept : x(), y(), z() {}
	constexpr explicit Vector3i(const int32 value) noexcept : x(value), y(value), z(value) {}
	constexpr explicit Vector3i(const int32 x, const int32 y, const int32 z) noexcept : x(x), y(y), z(z) {}
	constexpr explicit Vector3i(const Vector2i& value, const int32 z) noexcept : x(value.x), y(value.y), z(z) {}

	constexpr bool operator==(const Vector3i& rhs) const noexcept { return (x == rhs.x) && (y == rhs.y) && (z == rhs.z); }
	constexpr bool operator!=(const Vector3i& rhs) const noexcept { return (x != rhs.x) || (y != rhs.y) || (z != rhs.z); }

	constexpr Vector3i operator+() const noexcept { return *this; }
	constexpr Vector3i operator-() const noexcept { return Vector3i(-x, -y, -z); }

	static const Vector3i AxisX;
	static const Vector3i AxisY;
	static const Vector3i AxisZ;
	static const Vector3i One;
	static const Vector3i Zero;

public:
	int32 x, y, z;
};

inline constexpr Vector3i Vector3i::AxisX(1, 0, 0);
inline constexpr Vector3i Vector3i::AxisY(0, 1, 0);
inline constexpr Vector3i Vector3i::AxisZ(0, 0, 1);
inline constexpr Vector3i Vector3i::One(1);
inline constexpr Vector3i Vector3i::Zero(0);

/// \brief
class ALIGN_16 Vector4f
{
public:
	constexpr Vector4f() noexcept : x(), y(), z(), w() { }
	constexpr explicit Vector4f(const float xyzw) noexcept : x(xyzw), y(xyzw), z(xyzw), w(xyzw) { }
	constexpr explicit Vector4f(const float x, const float y, const float z, const float w) noexcept : x(x), y(y), z(z), w(w) { }
	constexpr explicit Vector4f(const Vector3f& value, const float w) noexcept : x(value.x), y(value.y), z(value.z), w(w) { }

	constexpr bool operator==(const Vector4f& rhs) const noexcept { return (x == rhs.x) && (y == rhs.y) && (z == rhs.z) && (w == rhs.w); }
	constexpr bool operator!=(const Vector4f& rhs) const noexcept { return (x != rhs.x) || (y != rhs.y) || (z != rhs.z) || (w != rhs.w); }

	constexpr Vector4f operator+(const Vector4f& rhs) const noexcept { return Vector4f(x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w); }
	constexpr Vector4f operator-(const Vector4f& rhs) const noexcept { return Vector4f(x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w); }

	constexpr Vector4f& operator+=(const Vector4f& rhs) noexcept { x += rhs.x; y += rhs.y; z += rhs.z; w += rhs.w; return *this; }
	constexpr Vector4f& operator-=(const Vector4f& rhs) noexcept { x -= rhs.x; y -= rhs.y; z -= rhs.z; w -= rhs.w; return *this; }

	constexpr Vector4f operator*(const float rhs) const noexcept { return Vector4f(x * rhs, y * rhs, z * rhs, w * rhs); }
	constexpr Vector4f operator/(const float rhs) const noexcept { return Vector4f(x / rhs, y / rhs, z / rhs, w / rhs); }

	constexpr Vector4f& operator*=(const float rhs) noexcept { x *= rhs; y *= rhs; z *= rhs; w *= rhs; return *this; }
	constexpr Vector4f& operator/=(const float rhs) noexcept { x /= rhs; y /= rhs; z /= rhs; w /= rhs; return *this; }

	constexpr Vector4f operator+() const noexcept { return *this; }
	constexpr Vector4f operator-() const noexcept { return Vector4f(-x, -y, -z, -w); }

	float Length() const noexcept;
	constexpr float LengthSqr() const noexcept;

	void Normalize() noexcept;
	void NormalizeUnsafe() noexcept;

	[[nodiscard]] Vector4f Normalized() const noexcept;
	[[nodiscard]] Vector4f NormalizedUnsafe() const noexcept;

	static const Vector4f AxisW;
	static const Vector4f AxisX;
	static const Vector4f AxisY;
	static const Vector4f AxisZ;
	static const Vector4f Identity;
	static const Vector4f Zero;

public:
	float x, y, z, w;
};

inline constexpr Vector4f Vector4f::AxisW(0.f, 0.f, 0.f, 1.f);
inline constexpr Vector4f Vector4f::AxisX(1.f, 0.f, 0.f, 0.f);
inline constexpr Vector4f Vector4f::AxisY(0.f, 1.f, 0.f, 0.f);
inline constexpr Vector4f Vector4f::AxisZ(0.f, 0.f, 1.f, 0.f);
inline constexpr Vector4f Vector4f::Identity(0.f, 0.f, 0.f, 1.f);
inline constexpr Vector4f Vector4f::Zero(0.f, 0.f, 0.f, 0.f);

#include <Core/Vector.inl>