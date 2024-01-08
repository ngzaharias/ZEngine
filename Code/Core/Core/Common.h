#pragma once

class Matrix3x3;
class Matrix4x4;
class Quaternion;
class Vector2f;
class Vector3f;
class Vector4f;

inline constexpr float Absolute(const float value) noexcept
{
	return value < 0.f ? -value : value;
}

inline constexpr bool IsNearly(const float a, const float b, const float threshold = 0.001f) noexcept
{
	return Absolute(a - b) < threshold;
}

/// \brief Check if the distance between two quaternions is less than the threshold.
constexpr bool IsNearly(const Quaternion& a, const Quaternion& b, const float threshold = 0.001f) noexcept;

/// \brief Check if the distance between two vectors is less than the threshold.
constexpr bool IsNearly(const Vector2f& a, const Vector2f& b, const float threshold = 0.001f) noexcept;
/// \brief Check if the distance between two vectors is less than the threshold.
constexpr bool IsNearly(const Vector3f& a, const Vector3f& b, const float threshold = 0.001f) noexcept;
/// \brief Check if the distance between two vectors is less than the threshold.
constexpr bool IsNearly(const Vector4f& a, const Vector4f& b, const float threshold = 0.001f) noexcept;

/// \brief Check if the distance between two matrices is less than the threshold.
constexpr bool IsNearly(const Matrix3x3& a, const Matrix3x3& b, const float threshold = 0.001f) noexcept;
/// \brief Check if the distance between two matrices is less than the threshold.
constexpr bool IsNearly(const Matrix4x4& a, const Matrix4x4& b, const float threshold = 0.001f) noexcept;

#include "Common.inl"