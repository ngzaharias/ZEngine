#pragma once

class Matrix3x3;
class Matrix4x4;
class Quaternion;

class Rotator
{
public:
	constexpr Rotator() noexcept : m_Pitch(), m_Yaw(), m_Roll() {}
	constexpr Rotator(const float value) noexcept : m_Pitch(value), m_Yaw(value), m_Roll(value) {}
	constexpr Rotator(const float pitch, const float yaw, const float roll) noexcept : m_Pitch(pitch), m_Yaw(yaw), m_Roll(roll) {}

	[[nodiscard]] constexpr bool operator==(const Rotator& rhs) const noexcept { return (m_Pitch == rhs.m_Pitch) && (m_Yaw == rhs.m_Yaw) && (m_Roll == rhs.m_Roll); }
	[[nodiscard]] constexpr bool operator!=(const Rotator& rhs) const noexcept { return (m_Pitch != rhs.m_Pitch) || (m_Yaw != rhs.m_Yaw) || (m_Roll != rhs.m_Roll); }

	[[nodiscard]] constexpr Rotator operator+(const Rotator& rhs) const noexcept { return Rotator(m_Pitch + rhs.m_Pitch, m_Yaw + rhs.m_Yaw, m_Roll + rhs.m_Roll); }
	[[nodiscard]] constexpr Rotator operator-(const Rotator& rhs) const noexcept { return Rotator(m_Pitch - rhs.m_Pitch, m_Yaw - rhs.m_Yaw, m_Roll - rhs.m_Roll); }

	constexpr Rotator& operator+=(const Rotator& rhs) noexcept { m_Pitch += rhs.m_Pitch; m_Yaw += rhs.m_Yaw; m_Roll += rhs.m_Roll; return *this; }
	constexpr Rotator& operator-=(const Rotator& rhs) noexcept { m_Pitch -= rhs.m_Pitch; m_Yaw -= rhs.m_Yaw; m_Roll -= rhs.m_Roll; return *this; }

	[[nodiscard]] constexpr Rotator operator*(const float rhs) const noexcept { return Rotator(m_Pitch * rhs, m_Yaw * rhs, m_Roll * rhs); }
	[[nodiscard]] constexpr Rotator operator/(const float rhs) const noexcept { return Rotator(m_Pitch / rhs, m_Yaw / rhs, m_Roll / rhs); }

	constexpr Rotator& operator*=(const float rhs) noexcept { m_Pitch *= rhs; m_Yaw *= rhs; m_Roll *= rhs; return *this; }
	constexpr Rotator& operator/=(const float rhs) noexcept { m_Pitch /= rhs; m_Yaw /= rhs; m_Roll /= rhs; return *this; }

	[[nodiscard]] constexpr Rotator operator+() const noexcept { return *this; }
	[[nodiscard]] constexpr Rotator operator-() const noexcept { return Rotator(-m_Pitch, -m_Yaw, -m_Roll); }

	[[nodiscard]] static Rotator FromMatrix(const Matrix3x3& value);
	[[nodiscard]] static Rotator FromMatrix(const Matrix4x4& value);
	[[nodiscard]] static Rotator FromQuaternion(const Quaternion& value);

	static const Rotator Zero;

public:
	// In Degrees (not Radians)
	float m_Pitch, m_Yaw, m_Roll;
};

inline const Rotator Rotator::Zero(0.f);
