#pragma once

#include "Math/Defines.h"
#include "Math/Types.h"

class Vector2f;
class Vector3f;
class Vector4f;

/// \brief A geometric object that has length and direction that can be used to represent positions and/or directions in 2d.
class Vector2f
{
public:
	/// \brief Construct with components initialized to 0.
	constexpr Vector2f() noexcept : x(0.f), y(0.f) {}
	/// \brief Construct with both components initialized to the same value.
	constexpr explicit Vector2f(const float xy) noexcept : x(xy), y(xy) {}
	/// \brief Construct with components initialized to values x and y.
	constexpr explicit Vector2f(const float x, const float y) noexcept : x(x), y(y) {}

	[[nodiscard]] constexpr bool operator<(const Vector2f& rhs) const noexcept { return (x != rhs.x) ? (x < rhs.x) : (y < rhs.y); }
	[[nodiscard]] constexpr bool operator<=(const Vector2f& rhs) const noexcept { return (x != rhs.x) ? (x <= rhs.x) : (y <= rhs.y); }
	[[nodiscard]] constexpr bool operator>(const Vector2f& rhs) const noexcept { return (x != rhs.x) ? (x > rhs.x) : (y > rhs.y); }
	[[nodiscard]] constexpr bool operator>=(const Vector2f& rhs) const noexcept { return (x != rhs.x) ? (x >= rhs.x) : (y >= rhs.y); }
	[[nodiscard]] constexpr bool operator==(const Vector2f& rhs) const noexcept { return (x == rhs.x) && (y == rhs.y); }
	[[nodiscard]] constexpr bool operator!=(const Vector2f& rhs) const noexcept { return (x != rhs.x) || (y != rhs.y); }

	[[nodiscard]] constexpr Vector2f operator+(const Vector2f& rhs) const noexcept { return Vector2f(x + rhs.x, y + rhs.y); }
	[[nodiscard]] constexpr Vector2f operator-(const Vector2f& rhs) const noexcept { return Vector2f(x - rhs.x, y - rhs.y); }
	[[nodiscard]] constexpr Vector2f operator*(const float rhs) const noexcept { return Vector2f(x * rhs, y * rhs); }
	[[nodiscard]] constexpr Vector2f operator/(const float rhs) const noexcept { return Vector2f(x / rhs, y / rhs); }

	constexpr Vector2f& operator+=(const Vector2f& rhs) noexcept { x += rhs.x; y += rhs.y; return *this; }
	constexpr Vector2f& operator-=(const Vector2f& rhs) noexcept { x -= rhs.x; y -= rhs.y; return *this; }
	constexpr Vector2f& operator*=(const float rhs) noexcept { x *= rhs; y *= rhs; return *this; }
	constexpr Vector2f& operator/=(const float rhs) noexcept { x /= rhs; y /= rhs; return *this; }

	[[nodiscard]] constexpr Vector2f operator+() const noexcept { return *this; }
	[[nodiscard]] constexpr Vector2f operator-() const noexcept { return Vector2f(-x, -y); }

	/// \brief Returns the length of this vector.
	[[nodiscard]] float Length() const noexcept;
	/// \brief Returns the squared length of this vector.
	[[nodiscard]] constexpr float LengthSqr() const noexcept;

	/// \brief Normalize this vector have a length of 1 unit.
	/// If the length of the vector is 0 then it makes it a NaN vector.
	void Normalize() noexcept;

	/// \brief Returns a normalized vector with a length of 1 unit.
	/// If the length of the vector is 0 then it returns a NaN vector.
	[[nodiscard]] Vector2f Normalized() const noexcept;

	/// \brief Converts this vector to a Vector3f with 0 in place of Z.
	constexpr Vector3f XY0() const noexcept;
	/// \brief Converts this vector to a Vector3f with 0 in place of Y, and Y in place of Z.
	constexpr Vector3f X0Y() const noexcept;

	/// \brief Shorthand for writing Vector2f(1.f, 0.f).
	static const Vector2f AxisX;
	/// \brief Shorthand for writing Vector2f(0.f, 1.f).
	static const Vector2f AxisY;
	/// \brief Shorthand for writing Vector2f(1.f).
	static const Vector2f One;
	/// \brief Shorthand for writing Vector2f(0.f).
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
	constexpr Vector2i()  noexcept : x(0), y(0) {}
	constexpr explicit Vector2i(const int32 value)  noexcept : x(value), y(value) {}
	constexpr explicit Vector2i(const int32 x, const int32 y)  noexcept : x(x), y(y) {}

	[[nodiscard]] constexpr bool operator<(const Vector2i& rhs) const noexcept { return (x != rhs.x) ? (x < rhs.x) : (y < rhs.y); }
	[[nodiscard]] constexpr bool operator<=(const Vector2i& rhs) const noexcept { return (x != rhs.x) ? (x <= rhs.x) : (y <= rhs.y); }
	[[nodiscard]] constexpr bool operator>(const Vector2i& rhs) const noexcept { return (x != rhs.x) ? (x > rhs.x) : (y > rhs.y); }
	[[nodiscard]] constexpr bool operator>=(const Vector2i& rhs) const noexcept { return (x != rhs.x) ? (x >= rhs.x) : (y >= rhs.y); }
	[[nodiscard]] constexpr bool operator==(const Vector2i& rhs) const noexcept { return (x == rhs.x) && (y == rhs.y); }
	[[nodiscard]] constexpr bool operator!=(const Vector2i& rhs) const noexcept { return (x != rhs.x) || (y != rhs.y); }

	[[nodiscard]] constexpr Vector2i operator+(const Vector2i& rhs) const noexcept { return Vector2i(x + rhs.x, y + rhs.y); }
	[[nodiscard]] constexpr Vector2i operator-(const Vector2i& rhs) const noexcept { return Vector2i(x - rhs.x, y - rhs.y); }
	[[nodiscard]] constexpr Vector2i operator*(const int32 rhs) const noexcept { return Vector2i(x * rhs, y * rhs); }
	[[nodiscard]] constexpr Vector2i operator/(const int32 rhs) const noexcept { return Vector2i(x / rhs, y / rhs); }

	constexpr Vector2i& operator+=(const Vector2i& rhs) noexcept { x += rhs.x; y += rhs.y; return *this; }
	constexpr Vector2i& operator-=(const Vector2i& rhs) noexcept { x -= rhs.x; y -= rhs.y; return *this; }
	constexpr Vector2i& operator*=(const int32 rhs) noexcept { x *= rhs; y *= rhs; return *this; }
	constexpr Vector2i& operator/=(const int32 rhs) noexcept { x /= rhs; y /= rhs; return *this; }

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
	constexpr Vector2u() noexcept : x(0), y(0) {}
	constexpr explicit Vector2u(const uint32 value) noexcept : x(value), y(value) {}
	constexpr explicit Vector2u(const uint32 x, const uint32 y) noexcept : x(x), y(y) {}

	[[nodiscard]] constexpr bool operator<(const Vector2u& rhs) const noexcept { return (x != rhs.x) ? (x < rhs.x) : (y < rhs.y); }
	[[nodiscard]] constexpr bool operator<=(const Vector2u& rhs) const noexcept { return (x != rhs.x) ? (x <= rhs.x) : (y <= rhs.y); }
	[[nodiscard]] constexpr bool operator>(const Vector2u& rhs) const noexcept { return (x != rhs.x) ? (x > rhs.x) : (y > rhs.y); }
	[[nodiscard]] constexpr bool operator>=(const Vector2u& rhs) const noexcept { return (x != rhs.x) ? (x >= rhs.x) : (y >= rhs.y); }
	[[nodiscard]] constexpr bool operator==(const Vector2u& rhs) const noexcept { return (x == rhs.x) && (y == rhs.y); }
	[[nodiscard]] constexpr bool operator!=(const Vector2u& rhs) const noexcept { return (x != rhs.x) || (y != rhs.y); }

	[[nodiscard]] constexpr Vector2u operator+(const Vector2u& rhs) const noexcept { return Vector2u(x + rhs.x, y + rhs.y); }
	[[nodiscard]] constexpr Vector2u operator-(const Vector2u& rhs) const noexcept { return Vector2u(x - rhs.x, y - rhs.y); }
	[[nodiscard]] constexpr Vector2u operator*(const uint32 rhs) const noexcept { return Vector2u(x * rhs, y * rhs); }
	[[nodiscard]] constexpr Vector2u operator/(const uint32 rhs) const noexcept { return Vector2u(x / rhs, y / rhs); }

	constexpr Vector2u& operator+=(const Vector2u& rhs) noexcept { x += rhs.x; y += rhs.y; return *this; }
	constexpr Vector2u& operator-=(const Vector2u& rhs) noexcept { x -= rhs.x; y -= rhs.y; return *this; }
	constexpr Vector2u& operator*=(const uint32 rhs) noexcept { x *= rhs; y *= rhs; return *this; }
	constexpr Vector2u& operator/=(const uint32 rhs) noexcept { x /= rhs; y /= rhs; return *this; }

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
	/// \brief Construct with components initialized to 0.
	constexpr Vector3f() noexcept : x(0.f), y(0.f), z(0.f) {}
	/// \brief Construct with both components initialized to the same value.
	constexpr explicit Vector3f(const float xyz) noexcept : x(xyz), y(xyz), z(xyz) {}
	/// \brief Construct with components initialized to values x, y and z.
	constexpr explicit Vector3f(const float x, const float y, const float z) noexcept : x(x), y(y), z(z) {}
	/// \brief Construct with components initialized to vector x and y, and value z.
	constexpr explicit Vector3f(const Vector2f& value, const float z) noexcept : x(value.x), y(value.y), z(z) {}

	[[nodiscard]] constexpr bool operator<(const Vector3f& rhs) const noexcept { return (x != rhs.x) ? (x < rhs.x) : (y != rhs.y) ? (y < rhs.y) : (z < rhs.z); }
	[[nodiscard]] constexpr bool operator<=(const Vector3f& rhs) const noexcept { return (x != rhs.x) ? (x <= rhs.x) : (y != rhs.y) ? (y <= rhs.y) : (z <= rhs.z); }
	[[nodiscard]] constexpr bool operator>(const Vector3f& rhs) const noexcept { return (x != rhs.x) ? (x > rhs.x) : (y != rhs.y) ? (y > rhs.y) : (z > rhs.z); }
	[[nodiscard]] constexpr bool operator>=(const Vector3f& rhs) const noexcept { return (x != rhs.x) ? (x >= rhs.x) : (y != rhs.y) ? (y >= rhs.y) : (z >= rhs.z); }
	[[nodiscard]] constexpr bool operator==(const Vector3f& rhs) const noexcept { return (x == rhs.x) && (y == rhs.y) && (z == rhs.z); }
	[[nodiscard]] constexpr bool operator!=(const Vector3f& rhs) const noexcept { return (x != rhs.x) || (y != rhs.y) || (z != rhs.z); }

	[[nodiscard]] constexpr Vector3f operator+(const Vector3f& rhs) const noexcept { return Vector3f(x + rhs.x, y + rhs.y, z + rhs.z); }
	[[nodiscard]] constexpr Vector3f operator-(const Vector3f& rhs) const noexcept { return Vector3f(x - rhs.x, y - rhs.y, z - rhs.z); }
	[[nodiscard]] constexpr Vector3f operator*(const float rhs) const noexcept { return Vector3f(x * rhs, y * rhs, z * rhs); }
	[[nodiscard]] constexpr Vector3f operator/(const float rhs) const noexcept { return Vector3f(x / rhs, y / rhs, z / rhs); }

	constexpr Vector3f& operator+=(const Vector3f& rhs) noexcept { x += rhs.x; y += rhs.y; z += rhs.z; return *this; }
	constexpr Vector3f& operator-=(const Vector3f& rhs) noexcept { x -= rhs.x; y -= rhs.y; z -= rhs.z; return *this; }
	constexpr Vector3f& operator*=(const float rhs) noexcept { x *= rhs; y *= rhs; z *= rhs; return *this; }
	constexpr Vector3f& operator/=(const float rhs) noexcept { x /= rhs; y /= rhs; z /= rhs; return *this; }

	[[nodiscard]] constexpr Vector3f operator+() const noexcept { return *this; }
	[[nodiscard]] constexpr Vector3f operator-() const noexcept { return Vector3f(-x, -y, -z); }

	/// \brief Returns the length of this vector.
	float Length() const noexcept;
	/// \brief Returns the length of this vector on the XY plane.
	float LengthXY() const noexcept;
	/// \brief Returns the length of this vector on the XZ plane.
	float LengthXZ() const noexcept;
	/// \brief Returns the squared length of this vector.
	constexpr float LengthSqr() const noexcept;
	/// \brief Returns the squared length of this vector on the XY plane.
	constexpr float LengthXYSqr() const noexcept;
	/// \brief Returns the squared length of this vector on the XZ plane.
	constexpr float LengthXZSqr() const noexcept;

	/// \brief Normalize this vector have a length of 1 unit.
	/// If the length of the vector is 0 then it makes it a NaN vector.
	void Normalize() noexcept;

	/// \brief Returns a normalized vector with a length of 1 unit.
	/// If the length of the vector is 0 then it returns a NaN vector.
	[[nodiscard]] Vector3f Normalized() const noexcept;

	/// \brief Converts this vector to a Vector2f.
	constexpr Vector2f XY() const noexcept;
	/// \brief Converts this vector to a Vector2f with Z in place of Y.
	constexpr Vector2f XZ() const noexcept;
	/// \brief Converts this vector to a Vector3f with 0 in place of Z.
	constexpr Vector3f XY0() const noexcept;
	/// \brief Converts this vector to a Vector3f with 0 in place of Y.
	constexpr Vector3f X0Z() const noexcept;

	/// \brief Shorthand for writing Vector3f(1.f, 0.f, 0.f).
	static const Vector3f AxisX;
	/// \brief Shorthand for writing Vector3f(0.f, 1.f, 0.f).
	static const Vector3f AxisY;
	/// \brief Shorthand for writing Vector3f(0.f, 0.f, 1.f).
	static const Vector3f AxisZ;
	/// \brief Shorthand for writing Vector3f(1.f, 1.f, 1.f).
	static const Vector3f One;
	/// \brief Shorthand for writing Vector3f(0.f, 0.f, 0.f).
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
	constexpr Vector3i() noexcept : x(0), y(0), z(0) {}
	constexpr explicit Vector3i(const int32 value) noexcept : x(value), y(value), z(value) {}
	constexpr explicit Vector3i(const int32 x, const int32 y, const int32 z) noexcept : x(x), y(y), z(z) {}
	constexpr explicit Vector3i(const Vector2i& value, const int32 z) noexcept : x(value.x), y(value.y), z(z) {}

	[[nodiscard]] constexpr bool operator<(const Vector3i& rhs) const noexcept { return (x != rhs.x) ? (x < rhs.x) : (y != rhs.y) ? (y < rhs.y) : (z < rhs.z); }
	[[nodiscard]] constexpr bool operator<=(const Vector3i& rhs) const noexcept { return (x != rhs.x) ? (x <= rhs.x) : (y != rhs.y) ? (y <= rhs.y) : (z <= rhs.z); }
	[[nodiscard]] constexpr bool operator>(const Vector3i& rhs) const noexcept { return (x != rhs.x) ? (x > rhs.x) : (y != rhs.y) ? (y > rhs.y) : (z > rhs.z); }
	[[nodiscard]] constexpr bool operator>=(const Vector3i& rhs) const noexcept { return (x != rhs.x) ? (x >= rhs.x) : (y != rhs.y) ? (y >= rhs.y) : (z >= rhs.z); }
	[[nodiscard]] constexpr bool operator==(const Vector3i& rhs) const noexcept { return (x == rhs.x) && (y == rhs.y) && (z == rhs.z); }
	[[nodiscard]] constexpr bool operator!=(const Vector3i& rhs) const noexcept { return (x != rhs.x) || (y != rhs.y) || (z != rhs.z); }

	[[nodiscard]] constexpr Vector3i operator+(const Vector3i& rhs) const noexcept { return Vector3i(x + rhs.x, y + rhs.y, z + rhs.z); }
	[[nodiscard]] constexpr Vector3i operator-(const Vector3i& rhs) const noexcept { return Vector3i(x - rhs.x, y - rhs.y, z - rhs.z); }

	constexpr Vector3i& operator+=(const Vector3i& rhs) noexcept { x += rhs.x; y += rhs.y; z += rhs.z; return *this; }
	constexpr Vector3i& operator-=(const Vector3i& rhs) noexcept { x -= rhs.x; y -= rhs.y; z -= rhs.z; return *this; }

	[[nodiscard]] constexpr Vector3i operator+() const noexcept { return *this; }
	[[nodiscard]] constexpr Vector3i operator-() const noexcept { return Vector3i(-x, -y, -z); }

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
	/// \brief Construct with components initialized to 0.
	constexpr Vector4f() noexcept : x(0.f), y(0.f), z(0.f), w(0.f) { }
	/// \brief Construct with both components initialized to the same value.
	constexpr explicit Vector4f(const float xyzw) noexcept : x(xyzw), y(xyzw), z(xyzw), w(xyzw) { }
	/// \brief Construct with components initialized to values x, y, z and w.
	constexpr explicit Vector4f(const float x, const float y, const float z, const float w) noexcept : x(x), y(y), z(z), w(w) { }
	/// \brief Construct with components initialized to vector x, y and z, and value w.
	constexpr explicit Vector4f(const Vector3f& value, const float w) noexcept : x(value.x), y(value.y), z(value.z), w(w) { }

	[[nodiscard]] constexpr bool operator==(const Vector4f& rhs) const noexcept { return (x == rhs.x) && (y == rhs.y) && (z == rhs.z) && (w == rhs.w); }
	[[nodiscard]] constexpr bool operator!=(const Vector4f& rhs) const noexcept { return (x != rhs.x) || (y != rhs.y) || (z != rhs.z) || (w != rhs.w); }

	[[nodiscard]] constexpr Vector4f operator+(const Vector4f& rhs) const noexcept { return Vector4f(x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w); }
	[[nodiscard]] constexpr Vector4f operator-(const Vector4f& rhs) const noexcept { return Vector4f(x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w); }
	[[nodiscard]] constexpr Vector4f operator*(const float rhs) const noexcept { return Vector4f(x * rhs, y * rhs, z * rhs, w * rhs); }
	[[nodiscard]] constexpr Vector4f operator/(const float rhs) const noexcept { return Vector4f(x / rhs, y / rhs, z / rhs, w / rhs); }

	constexpr Vector4f& operator+=(const Vector4f& rhs) noexcept { x += rhs.x; y += rhs.y; z += rhs.z; w += rhs.w; return *this; }
	constexpr Vector4f& operator-=(const Vector4f& rhs) noexcept { x -= rhs.x; y -= rhs.y; z -= rhs.z; w -= rhs.w; return *this; }
	constexpr Vector4f& operator*=(const float rhs) noexcept { x *= rhs; y *= rhs; z *= rhs; w *= rhs; return *this; }
	constexpr Vector4f& operator/=(const float rhs) noexcept { x /= rhs; y /= rhs; z /= rhs; w /= rhs; return *this; }

	[[nodiscard]] constexpr Vector4f operator+() const noexcept { return *this; }
	[[nodiscard]] constexpr Vector4f operator-() const noexcept { return Vector4f(-x, -y, -z, -w); }

	/// \brief Returns the length of this vector.
	float Length() const noexcept;
	/// \brief Returns the squared length of this vector.
	constexpr float LengthSqr() const noexcept;

	/// \brief Normalize this vector have a length of 1 unit.
	/// If the length of the vector is 0 then it makes it a NaN vector.
	void Normalize() noexcept;

	/// \brief Returns a normalized vector with a length of 1 unit.
	/// If the length of the vector is 0 then it returns a NaN vector.
	[[nodiscard]] Vector4f Normalized() const noexcept;

	/// \brief Shorthand for writing Vector4f(0.f, 0.f, 0.f, 1.f).
	static const Vector4f AxisW;
	/// \brief Shorthand for writing Vector4f(1.f, 0.f, 0.f, 0.f).
	static const Vector4f AxisX;
	/// \brief Shorthand for writing Vector4f(0.f, 1.f, 0.f, 0.f).
	static const Vector4f AxisY;
	/// \brief Shorthand for writing Vector4f(0.f, 0.f, 1.f, 0.f).
	static const Vector4f AxisZ;
	/// \brief Shorthand for writing Vector4f(0.f, 0.f, 0.f, 1.f).
	static const Vector4f Identity;
	/// \brief Shorthand for writing Vector4f(0.f, 0.f, 0.f, 0.f).
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

#include "Math/Vector.inl"