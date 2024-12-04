#pragma once

class Matrix3x3;
class Matrix4x4;
class Rotator;
class Vector2f;
class Vector3f;
class Vector4f;

/// \brief Represents the rotation of an object in 3D space. 
/// - Doesn't suffer form gimbal lock, unlike matrices.
/// - Only requires 4 floats, which makes it ideal for syncing rotations.
class Quaternion
{
public:
	constexpr Quaternion() noexcept;
	constexpr Quaternion(const float x, const float y, const float z, const float w) noexcept;
	constexpr Quaternion(const Vector3f& value, const float w) noexcept;
	constexpr Quaternion(const Vector4f& value) noexcept;

	[[nodiscard]] constexpr bool operator==(const Quaternion& rhs) const noexcept { return (x == rhs.x) && (y == rhs.y) && (z == rhs.z) && (w == rhs.w); }
	[[nodiscard]] constexpr bool operator!=(const Quaternion& rhs) const noexcept { return (x != rhs.x) || (y != rhs.y) || (z != rhs.z) || (w != rhs.w); }

	[[nodiscard]] constexpr Quaternion operator+(const Quaternion& rhs) const noexcept { return Quaternion(x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w); }
	[[nodiscard]] constexpr Quaternion operator-(const Quaternion& rhs) const noexcept { return Quaternion(x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w); }
	[[nodiscard]] constexpr Quaternion operator*(const Quaternion& rhs) const noexcept;

	constexpr Quaternion& operator+=(const Quaternion& rhs) noexcept { *this = *this + rhs; return *this; }
	constexpr Quaternion& operator-=(const Quaternion& rhs) noexcept { *this = *this - rhs; return *this; }
	constexpr Quaternion& operator*=(const Quaternion& rhs) noexcept { *this = *this * rhs; return *this; }

	[[nodiscard]] constexpr Quaternion operator*(const float rhs) const noexcept { return Quaternion(x * rhs, y * rhs, z * rhs, w * rhs); }
	[[nodiscard]] constexpr Quaternion operator/(const float rhs) const noexcept { return Quaternion(x / rhs, y / rhs, z / rhs, w / rhs); }

	constexpr Quaternion& operator*=(const float rhs) noexcept { *this = *this * rhs; return *this; }
	constexpr Quaternion& operator/=(const float rhs) noexcept { *this = *this / rhs; return *this; }

	[[nodiscard]] constexpr Quaternion operator+() const noexcept { return *this; }
	[[nodiscard]] constexpr Quaternion operator-() const noexcept { return Quaternion(-x, -y, -z, -w); }

	float Length() const noexcept;
	constexpr float LengthSqr() const noexcept { return x * x + y * y + z * z + w * w; }

	void Inverse() noexcept;
	void Normalize() noexcept;
	void NormalizeUnsafe() noexcept;

	[[nodiscard]] constexpr Quaternion Inversed() const noexcept;
	[[nodiscard]] Quaternion Normalized() const noexcept;
	[[nodiscard]] Quaternion NormalizedUnsafe() const noexcept;

	void ToAxisAngle(Vector3f& out_Axis, float& out_Angle) const noexcept;

	[[nodiscard]] static float Angle(const Quaternion& a, const Quaternion& b) noexcept;
	[[nodiscard]] static constexpr float Dot(const Quaternion& a, const Quaternion& b) noexcept;
	[[nodiscard]] static constexpr Quaternion Lerp(const Quaternion& from, const Quaternion& to, const float t) noexcept;
	[[nodiscard]] static Quaternion Slerp(const Quaternion& from, const Quaternion& to, const float t) noexcept;

	[[nodiscard]] static Quaternion FromAxisAngle(const Vector3f& axis, const float radians) noexcept;
	[[nodiscard]] static Quaternion FromMatrix(const Matrix3x3& value) noexcept;
	[[nodiscard]] static Quaternion FromMatrix(const Matrix4x4& value) noexcept;
	[[nodiscard]] static Quaternion FromRotator(const Rotator& value) noexcept;

	static const Quaternion AxisW;
	static const Quaternion AxisX;
	static const Quaternion AxisY;
	static const Quaternion AxisZ;
	static const Quaternion Identity;
	static const Quaternion Zero;

public:
	float x, y, z, w;
};

inline const Quaternion Quaternion::AxisW(0.f, 0.f, 0.f, 1.f);
inline const Quaternion Quaternion::AxisX(1.f, 0.f, 0.f, 0.f);
inline const Quaternion Quaternion::AxisY(0.f, 1.f, 0.f, 0.f);
inline const Quaternion Quaternion::AxisZ(0.f, 0.f, 1.f, 0.f);
inline const Quaternion Quaternion::Identity(0.f, 0.f, 0.f, 1.f);
inline const Quaternion Quaternion::Zero(0.f, 0.f, 0.f, 0.f);

constexpr Vector2f operator*(const Vector2f& lhs, const Quaternion& rhs) noexcept;
constexpr Vector3f operator*(const Vector3f& lhs, const Quaternion& rhs) noexcept;

constexpr Vector2f& operator*=(Vector2f& lhs, const Quaternion& rhs) noexcept;
constexpr Vector3f& operator*=(Vector3f& lhs, const Quaternion& rhs) noexcept;

#include "Math/Quaternion.inl"