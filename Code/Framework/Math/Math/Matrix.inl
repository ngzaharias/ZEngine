#pragma once

#include <cmath>
#include <cstring>
#include <utility>

#include "Math/Quaternion.h"
#include "Math/Rotator.h"
#include "Math/Vector.h"

constexpr Matrix3x3::Matrix3x3() noexcept
	: m_Data()
{
}

constexpr Matrix3x3::Matrix3x3(
	const float v00, const float v01, const float v02,
	const float v10, const float v11, const float v12,
	const float v20, const float v21, const float v22) noexcept
	: m_Data{ { v00, v01, v02 }, { v10, v11, v12 }, { v20, v21, v22 } }
{
}

constexpr Matrix3x3::Matrix3x3(const Vector3f& v0, const Vector3f& v1, const Vector3f& v2) noexcept
	: m_Data{ v0.x, v0.y, v0.z, v1.x, v1.y, v1.z, v2.x, v2.y, v2.z }
{
}

constexpr Matrix3x3::Matrix3x3(const Matrix4x4& value) noexcept
	: m_Data{
		value.m_Data[0][0], value.m_Data[0][1], value.m_Data[0][2],
		value.m_Data[1][0], value.m_Data[1][1], value.m_Data[1][2],
		value.m_Data[2][0], value.m_Data[2][1], value.m_Data[2][2] }
{
}

inline constexpr bool Matrix3x3::operator==(const Matrix3x3& rhs) const noexcept
{
	return m_Data[0][0] == rhs.m_Data[0][0]
		&& m_Data[0][1] == rhs.m_Data[0][1]
		&& m_Data[0][2] == rhs.m_Data[0][2]

		&& m_Data[1][0] == rhs.m_Data[1][0]
		&& m_Data[1][1] == rhs.m_Data[1][1]
		&& m_Data[1][2] == rhs.m_Data[1][2]

		&& m_Data[2][0] == rhs.m_Data[2][0]
		&& m_Data[2][1] == rhs.m_Data[2][1]
		&& m_Data[2][2] == rhs.m_Data[2][2];
}

inline constexpr bool Matrix3x3::operator!=(const Matrix3x3& rhs) const noexcept
{
	return m_Data[0][0] != rhs.m_Data[0][0]
		|| m_Data[0][1] != rhs.m_Data[0][1]
		|| m_Data[0][2] != rhs.m_Data[0][2]

		|| m_Data[1][0] != rhs.m_Data[1][0]
		|| m_Data[1][1] != rhs.m_Data[1][1]
		|| m_Data[1][2] != rhs.m_Data[1][2]

		|| m_Data[2][0] != rhs.m_Data[2][0]
		|| m_Data[2][1] != rhs.m_Data[2][1]
		|| m_Data[2][2] != rhs.m_Data[2][2];
}

inline constexpr Matrix3x3 Matrix3x3::operator+(const Matrix3x3& rhs) const noexcept
{
	return Matrix3x3{
		m_Data[0][0] + rhs.m_Data[0][0],
		m_Data[0][1] + rhs.m_Data[0][1],
		m_Data[0][2] + rhs.m_Data[0][2],

		m_Data[1][0] + rhs.m_Data[1][0],
		m_Data[1][1] + rhs.m_Data[1][1],
		m_Data[1][2] + rhs.m_Data[1][2],

		m_Data[2][0] + rhs.m_Data[2][0],
		m_Data[2][1] + rhs.m_Data[2][1],
		m_Data[2][2] + rhs.m_Data[2][2] };
}

inline constexpr Matrix3x3 Matrix3x3::operator-(const Matrix3x3& rhs) const noexcept
{
	return Matrix3x3{
		m_Data[0][0] - rhs.m_Data[0][0],
		m_Data[0][1] - rhs.m_Data[0][1],
		m_Data[0][2] - rhs.m_Data[0][2],

		m_Data[1][0] - rhs.m_Data[1][0],
		m_Data[1][1] - rhs.m_Data[1][1],
		m_Data[1][2] - rhs.m_Data[1][2],

		m_Data[2][0] - rhs.m_Data[2][0],
		m_Data[2][1] - rhs.m_Data[2][1],
		m_Data[2][2] - rhs.m_Data[2][2] };
}

inline constexpr Matrix3x3 Matrix3x3::operator*(const Matrix3x3& rhs) const noexcept
{
	return Matrix3x3{
		m_Data[0][0] * rhs.m_Data[0][0] + m_Data[0][1] * rhs.m_Data[1][0] + m_Data[0][2] * rhs.m_Data[2][0],
		m_Data[0][0] * rhs.m_Data[0][1] + m_Data[0][1] * rhs.m_Data[1][1] + m_Data[0][2] * rhs.m_Data[2][1],
		m_Data[0][0] * rhs.m_Data[0][2] + m_Data[0][1] * rhs.m_Data[1][2] + m_Data[0][2] * rhs.m_Data[2][2],

		m_Data[1][0] * rhs.m_Data[0][0] + m_Data[1][1] * rhs.m_Data[1][0] + m_Data[1][2] * rhs.m_Data[2][0],
		m_Data[1][0] * rhs.m_Data[0][1] + m_Data[1][1] * rhs.m_Data[1][1] + m_Data[1][2] * rhs.m_Data[2][1],
		m_Data[1][0] * rhs.m_Data[0][2] + m_Data[1][1] * rhs.m_Data[1][2] + m_Data[1][2] * rhs.m_Data[2][2],

		m_Data[2][0] * rhs.m_Data[0][0] + m_Data[2][1] * rhs.m_Data[1][0] + m_Data[2][2] * rhs.m_Data[2][0],
		m_Data[2][0] * rhs.m_Data[0][1] + m_Data[2][1] * rhs.m_Data[1][1] + m_Data[2][2] * rhs.m_Data[2][1],
		m_Data[2][0] * rhs.m_Data[0][2] + m_Data[2][1] * rhs.m_Data[1][2] + m_Data[2][2] * rhs.m_Data[2][2] };
}

// https://stackoverflow.com/questions/983999/simple-3x3-matrix-inverse-code-c
inline void Matrix3x3::Inverse() noexcept
{
	const float det = 
		m_Data[0][0] * (m_Data[1][1] * m_Data[2][2] - m_Data[2][1] * m_Data[1][2]) -
		m_Data[0][1] * (m_Data[1][0] * m_Data[2][2] - m_Data[1][2] * m_Data[2][0]) +
		m_Data[0][2] * (m_Data[1][0] * m_Data[2][1] - m_Data[1][1] * m_Data[2][0]);

	const float invdet = 1 / det;

	float inv[3][3];
	inv[0][0] = (m_Data[1][1] * m_Data[2][2] - m_Data[2][1] * m_Data[1][2]) * invdet;
	inv[0][1] = (m_Data[0][2] * m_Data[2][1] - m_Data[0][1] * m_Data[2][2]) * invdet;
	inv[0][2] = (m_Data[0][1] * m_Data[1][2] - m_Data[0][2] * m_Data[1][1]) * invdet;

	inv[1][0] = (m_Data[1][2] * m_Data[2][0] - m_Data[1][0] * m_Data[2][2]) * invdet;
	inv[1][1] = (m_Data[0][0] * m_Data[2][2] - m_Data[0][2] * m_Data[2][0]) * invdet;
	inv[1][2] = (m_Data[1][0] * m_Data[0][2] - m_Data[0][0] * m_Data[1][2]) * invdet;

	inv[2][0] = (m_Data[1][0] * m_Data[2][1] - m_Data[2][0] * m_Data[1][1]) * invdet;
	inv[2][1] = (m_Data[2][0] * m_Data[0][1] - m_Data[0][0] * m_Data[2][1]) * invdet;
	inv[2][2] = (m_Data[0][0] * m_Data[1][1] - m_Data[1][0] * m_Data[0][1]) * invdet;

	std::memcpy(&m_Data, &inv, sizeof(float) * 9);
}

inline void Matrix3x3::Transpose() noexcept
{
	std::swap(m_Data[0][1], m_Data[1][0]);
	std::swap(m_Data[0][2], m_Data[2][0]);
	std::swap(m_Data[1][2], m_Data[2][1]);
}

inline void Matrix3x3::Rotate(const Rotator& value) noexcept
{
	*this *= Matrix3x3::FromRotate(value);
}

inline void Matrix3x3::Rotate(const Quaternion& value) noexcept
{
	*this *= Matrix3x3::FromRotate(value);
}

inline Matrix3x3 Matrix3x3::Inversed() const noexcept
{
	Matrix3x3 value(*this);
	value.Inverse();
	return value;
}

inline Matrix3x3 Matrix3x3::Transposed() const noexcept
{
	Matrix3x3 value(*this);
	value.Transpose();
	return value;
}

// https://stackoverflow.com/questions/1996957/conversion-euler-to-matrix-and-matrix-to-euler
inline void Matrix3x3::SetRotate(const Rotator& value) noexcept
{
	const float x = math::ToRadians(value.m_Pitch);
	const float y = math::ToRadians(value.m_Yaw);
	const float z = math::ToRadians(value.m_Roll);
	const float sinX = std::sin(x);
	const float sinY = std::sin(y);
	const float sinZ = std::sin(z);
	const float cosX = std::cos(x);
	const float cosY = std::cos(y);
	const float cosZ = std::cos(z);

	m_Data[0][0] = cosY * cosZ - sinY * sinX * sinZ;
	m_Data[0][1] = -sinZ * cosX;
	m_Data[0][2] = sinY * cosZ + cosY * sinX * sinZ;

	m_Data[1][0] = cosY * sinZ + sinY * sinX * cosZ;
	m_Data[1][1] = cosZ * cosX;
	m_Data[1][2] = sinZ * sinY - cosY * sinX * cosZ;

	m_Data[2][0] = -sinY * cosX;
	m_Data[2][1] = -sinX;
	m_Data[2][2] = cosY * cosX;
}

inline constexpr void Matrix3x3::SetRotate(const Quaternion& value) noexcept
{
	m_Data[0][0] = 1.f - (2.f * value.y * value.y) - (2.f * value.z * value.z);
	m_Data[0][1] = (2.f * value.x * value.y) + (2.f * value.z * value.w);
	m_Data[0][2] = (2.f * value.x * value.z) - (2.f * value.y * value.w);

	m_Data[1][0] = (2.f * value.x * value.y) - (2.f * value.z * value.w);
	m_Data[1][1] = 1.f - (2.f * value.x * value.x) - (2.f * value.z * value.z);
	m_Data[1][2] = (2.f * value.y * value.z) + (2.f * value.x * value.w);

	m_Data[2][0] = (2.f * value.x * value.z) + (2.f * value.y * value.w);
	m_Data[2][1] = (2.f * value.y * value.z) - (2.f * value.x * value.w);
	m_Data[2][2] = 1.f - (2.f * value.x * value.x) - (2.f * value.y * value.y);
}

// https://stackoverflow.com/questions/1996957/conversion-euler-to-matrix-and-matrix-to-euler
inline Matrix3x3 Matrix3x3::FromRotate(const Rotator& value) noexcept
{
	const float x = math::ToRadians(value.m_Pitch);
	const float y = math::ToRadians(value.m_Yaw);
	const float z = math::ToRadians(value.m_Roll);

	const float cosP = std::cosf(x);
	const float sinP = std::sinf(x);
	const float cosY = std::cosf(y);
	const float sinY = std::sinf(y);
	const float cosR = std::cosf(z);
	const float sinR = std::sinf(z);

	Matrix3x3 matrix;
	matrix[0][0] = cosY * cosR + sinY * sinP * sinR;
	matrix[0][1] = cosP * sinR;
	matrix[0][2] = sinR * cosY * sinP - sinY * cosR;
	matrix[1][0] = cosR * sinY * sinP - sinR * cosY;
	matrix[1][1] = cosR * cosP;
	matrix[1][2] = sinY * sinR + cosR * cosY * sinP;
	matrix[2][0] = cosP * sinY;
	matrix[2][1] = -sinP;
	matrix[2][2] = cosP * cosY;
	return matrix;
}

inline constexpr Matrix3x3 Matrix3x3::FromRotate(const Quaternion& value) noexcept
{
	Matrix3x3 matrix;
	matrix[0][0] = 1.f - (2.f * value.y * value.y) - (2.f * value.z * value.z);
	matrix[0][1] = (2.f * value.x * value.y) + (2.f * value.z * value.w);
	matrix[0][2] = (2.f * value.x * value.z) - (2.f * value.y * value.w);

	matrix[1][0] = (2.f * value.x * value.y) - (2.f * value.z * value.w);
	matrix[1][1] = 1.f - (2.f * value.x * value.x) - (2.f * value.z * value.z);
	matrix[1][2] = (2.f * value.y * value.z) + (2.f * value.x * value.w);

	matrix[2][0] = (2.f * value.x * value.z) + (2.f * value.y * value.w);
	matrix[2][1] = (2.f * value.y * value.z) - (2.f * value.x * value.w);
	matrix[2][2] = 1.f - (2.f * value.x * value.x) - (2.f * value.y * value.y);
	return matrix;
}

inline constexpr Vector2f operator*(const Vector2f& lhs, const Matrix3x3& rhs) noexcept
{
	return Vector2f(
		rhs.m_Data[0][0] * lhs.x + rhs.m_Data[1][0] * lhs.y,
		rhs.m_Data[0][1] * lhs.x + rhs.m_Data[1][1] * lhs.y);
}

inline constexpr Vector3f operator*(const Vector3f& lhs, const Matrix3x3& rhs) noexcept
{
	return Vector3f(
		rhs.m_Data[0][0] * lhs.x + rhs.m_Data[1][0] * lhs.y + rhs.m_Data[2][0] * lhs.z,
		rhs.m_Data[0][1] * lhs.x + rhs.m_Data[1][1] * lhs.y + rhs.m_Data[2][1] * lhs.z,
		rhs.m_Data[0][2] * lhs.x + rhs.m_Data[1][2] * lhs.y + rhs.m_Data[2][2] * lhs.z);
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

constexpr Matrix4x4::Matrix4x4() noexcept
	: m_Data()
{
}

constexpr Matrix4x4::Matrix4x4(
	const float v00, const float v01, const float v02, const float v03,
	const float v10, const float v11, const float v12, const float v13,
	const float v20, const float v21, const float v22, const float v23,
	const float v30, const float v31, const float v32, const float v33) noexcept
	: m_Data{ 
		{ v00, v01, v02, v03 }, 
		{ v10, v11, v12, v13 }, 
		{ v20, v21, v22, v23 }, 
		{ v30, v31, v32, v33 } }
{
}

constexpr Matrix4x4::Matrix4x4(const Vector4f& v0, const Vector4f& v1, const Vector4f& v2, const Vector4f& v3) noexcept
	: m_Data{ 
		v0.x, v0.y, v0.z, v0.w,
		v1.x, v1.y, v1.z, v1.w,
		v2.x, v2.y, v2.z, v2.w,
		v3.x, v3.y, v3.z, v3.w }
{
}

constexpr Matrix4x4::Matrix4x4(const Matrix3x3& value) noexcept
	: m_Data{
		value.m_Data[0][0], value.m_Data[0][1], value.m_Data[0][2], 0.f,
		value.m_Data[1][0], value.m_Data[1][1], value.m_Data[1][2], 0.f,
		value.m_Data[2][0], value.m_Data[2][1], value.m_Data[2][2], 1.f }
{
}

inline constexpr bool Matrix4x4::operator==(const Matrix4x4& rhs) const noexcept
{
	return m_Data[0][0] == rhs.m_Data[0][0]
		&& m_Data[0][1] == rhs.m_Data[0][1]
		&& m_Data[0][2] == rhs.m_Data[0][2]
		&& m_Data[0][3] == rhs.m_Data[0][3]

		&& m_Data[1][0] == rhs.m_Data[1][0]
		&& m_Data[1][1] == rhs.m_Data[1][1]
		&& m_Data[1][2] == rhs.m_Data[1][2]
		&& m_Data[1][3] == rhs.m_Data[1][3]

		&& m_Data[2][0] == rhs.m_Data[2][0]
		&& m_Data[2][1] == rhs.m_Data[2][1]
		&& m_Data[2][2] == rhs.m_Data[2][2]
		&& m_Data[2][3] == rhs.m_Data[2][3]

		&& m_Data[3][0] == rhs.m_Data[3][0]
		&& m_Data[3][1] == rhs.m_Data[3][1]
		&& m_Data[3][2] == rhs.m_Data[3][2]
		&& m_Data[3][3] == rhs.m_Data[3][3];
}

inline constexpr bool Matrix4x4::operator!=(const Matrix4x4& rhs) const noexcept
{
	return m_Data[0][0] != rhs.m_Data[0][0]
		|| m_Data[0][1] != rhs.m_Data[0][1]
		|| m_Data[0][2] != rhs.m_Data[0][2]
		|| m_Data[0][3] != rhs.m_Data[0][3]

		|| m_Data[1][0] != rhs.m_Data[1][0]
		|| m_Data[1][1] != rhs.m_Data[1][1]
		|| m_Data[1][2] != rhs.m_Data[1][2]
		|| m_Data[1][3] != rhs.m_Data[1][3]

		|| m_Data[2][0] != rhs.m_Data[2][0]
		|| m_Data[2][1] != rhs.m_Data[2][1]
		|| m_Data[2][2] != rhs.m_Data[2][2]
		|| m_Data[2][3] != rhs.m_Data[2][3]

		|| m_Data[3][0] != rhs.m_Data[3][0]
		|| m_Data[3][1] != rhs.m_Data[3][1]
		|| m_Data[3][2] != rhs.m_Data[3][2]
		|| m_Data[3][3] != rhs.m_Data[3][3];
}

inline constexpr Matrix4x4 Matrix4x4::operator+(const Matrix4x4& rhs) const noexcept
{
	return Matrix4x4{
		m_Data[0][0] + rhs.m_Data[0][0],
		m_Data[0][1] + rhs.m_Data[0][1],
		m_Data[0][2] + rhs.m_Data[0][2],
		m_Data[0][3] + rhs.m_Data[0][3],

		m_Data[1][0] + rhs.m_Data[1][0],
		m_Data[1][1] + rhs.m_Data[1][1],
		m_Data[1][2] + rhs.m_Data[1][2],
		m_Data[1][3] + rhs.m_Data[1][3],

		m_Data[2][0] + rhs.m_Data[2][0],
		m_Data[2][1] + rhs.m_Data[2][1],
		m_Data[2][2] + rhs.m_Data[2][2],
		m_Data[2][3] + rhs.m_Data[2][3],

		m_Data[3][0] + rhs.m_Data[3][0],
		m_Data[3][1] + rhs.m_Data[3][1],
		m_Data[3][2] + rhs.m_Data[3][2],
		m_Data[3][3] + rhs.m_Data[3][3] };
}

inline constexpr Matrix4x4 Matrix4x4::operator-(const Matrix4x4& rhs) const noexcept
{
	return Matrix4x4{
		m_Data[0][0] - rhs.m_Data[0][0],
		m_Data[0][1] - rhs.m_Data[0][1],
		m_Data[0][2] - rhs.m_Data[0][2],
		m_Data[0][3] - rhs.m_Data[0][3],

		m_Data[1][0] - rhs.m_Data[1][0],
		m_Data[1][1] - rhs.m_Data[1][1],
		m_Data[1][2] - rhs.m_Data[1][2],
		m_Data[1][3] - rhs.m_Data[1][3],

		m_Data[2][0] - rhs.m_Data[2][0],
		m_Data[2][1] - rhs.m_Data[2][1],
		m_Data[2][2] - rhs.m_Data[2][2],
		m_Data[2][3] - rhs.m_Data[2][3],

		m_Data[3][0] - rhs.m_Data[3][0],
		m_Data[3][1] - rhs.m_Data[3][1],
		m_Data[3][2] - rhs.m_Data[3][2],
		m_Data[3][3] - rhs.m_Data[3][3] };
}

inline constexpr Matrix4x4 Matrix4x4::operator*(const Matrix4x4& rhs) const noexcept
{
	return Matrix4x4{
		m_Data[0][0] * rhs.m_Data[0][0] + m_Data[0][1] * rhs.m_Data[1][0] + m_Data[0][2] * rhs.m_Data[2][0] + m_Data[0][3] * rhs.m_Data[3][0],
		m_Data[0][0] * rhs.m_Data[0][1] + m_Data[0][1] * rhs.m_Data[1][1] + m_Data[0][2] * rhs.m_Data[2][1] + m_Data[0][3] * rhs.m_Data[3][1],
		m_Data[0][0] * rhs.m_Data[0][2] + m_Data[0][1] * rhs.m_Data[1][2] + m_Data[0][2] * rhs.m_Data[2][2] + m_Data[0][3] * rhs.m_Data[3][2],
		m_Data[0][0] * rhs.m_Data[0][3] + m_Data[0][1] * rhs.m_Data[1][3] + m_Data[0][2] * rhs.m_Data[2][3] + m_Data[0][3] * rhs.m_Data[3][3],

		m_Data[1][0] * rhs.m_Data[0][0] + m_Data[1][1] * rhs.m_Data[1][0] + m_Data[1][2] * rhs.m_Data[2][0] + m_Data[1][3] * rhs.m_Data[3][0],
		m_Data[1][0] * rhs.m_Data[0][1] + m_Data[1][1] * rhs.m_Data[1][1] + m_Data[1][2] * rhs.m_Data[2][1] + m_Data[1][3] * rhs.m_Data[3][1],
		m_Data[1][0] * rhs.m_Data[0][2] + m_Data[1][1] * rhs.m_Data[1][2] + m_Data[1][2] * rhs.m_Data[2][2] + m_Data[1][3] * rhs.m_Data[3][2],
		m_Data[1][0] * rhs.m_Data[0][3] + m_Data[1][1] * rhs.m_Data[1][3] + m_Data[1][2] * rhs.m_Data[2][3] + m_Data[1][3] * rhs.m_Data[3][3],

		m_Data[2][0] * rhs.m_Data[0][0] + m_Data[2][1] * rhs.m_Data[1][0] + m_Data[2][2] * rhs.m_Data[2][0] + m_Data[2][3] * rhs.m_Data[3][0],
		m_Data[2][0] * rhs.m_Data[0][1] + m_Data[2][1] * rhs.m_Data[1][1] + m_Data[2][2] * rhs.m_Data[2][1] + m_Data[2][3] * rhs.m_Data[3][1],
		m_Data[2][0] * rhs.m_Data[0][2] + m_Data[2][1] * rhs.m_Data[1][2] + m_Data[2][2] * rhs.m_Data[2][2] + m_Data[2][3] * rhs.m_Data[3][2],
		m_Data[2][0] * rhs.m_Data[0][3] + m_Data[2][1] * rhs.m_Data[1][3] + m_Data[2][2] * rhs.m_Data[2][3] + m_Data[2][3] * rhs.m_Data[3][3],

		m_Data[3][0] * rhs.m_Data[0][0] + m_Data[3][1] * rhs.m_Data[1][0] + m_Data[3][2] * rhs.m_Data[2][0] + m_Data[3][3] * rhs.m_Data[3][0],
		m_Data[3][0] * rhs.m_Data[0][1] + m_Data[3][1] * rhs.m_Data[1][1] + m_Data[3][2] * rhs.m_Data[2][1] + m_Data[3][3] * rhs.m_Data[3][1],
		m_Data[3][0] * rhs.m_Data[0][2] + m_Data[3][1] * rhs.m_Data[1][2] + m_Data[3][2] * rhs.m_Data[2][2] + m_Data[3][3] * rhs.m_Data[3][2],
		m_Data[3][0] * rhs.m_Data[0][3] + m_Data[3][1] * rhs.m_Data[1][3] + m_Data[3][2] * rhs.m_Data[2][3] + m_Data[3][3] * rhs.m_Data[3][3] };
}

// https://stackoverflow.com/questions/1148309/inverting-a-4x4-matrix
inline void Matrix4x4::Inverse() noexcept
{
	float inv[4][4];
	inv[0][0] = 
		m_Data[1][1] * m_Data[2][2] * m_Data[3][3] -
		m_Data[1][1] * m_Data[2][3] * m_Data[3][2] -
		m_Data[2][1] * m_Data[1][2] * m_Data[3][3] +
		m_Data[2][1] * m_Data[1][3] * m_Data[3][2] +
		m_Data[3][1] * m_Data[1][2] * m_Data[2][3] -
		m_Data[3][1] * m_Data[1][3] * m_Data[2][2];

	inv[1][0] = 
		-m_Data[1][0] * m_Data[2][2] * m_Data[3][3] +
		m_Data[1][0] * m_Data[2][3] * m_Data[3][2] +
		m_Data[2][0] * m_Data[1][2] * m_Data[3][3] -
		m_Data[2][0] * m_Data[1][3] * m_Data[3][2] -
		m_Data[3][0] * m_Data[1][2] * m_Data[2][3] +
		m_Data[3][0] * m_Data[1][3] * m_Data[2][2];

	inv[2][0] = 
		m_Data[1][0] * m_Data[2][1] * m_Data[3][3] -
		m_Data[1][0] * m_Data[2][3] * m_Data[3][1] -
		m_Data[2][0] * m_Data[1][1] * m_Data[3][3] +
		m_Data[2][0] * m_Data[1][3] * m_Data[3][1] +
		m_Data[3][0] * m_Data[1][1] * m_Data[2][3] -
		m_Data[3][0] * m_Data[1][3] * m_Data[2][1];

	inv[3][0] = 
		-m_Data[1][0] * m_Data[2][1] * m_Data[3][2] +
		m_Data[1][0] * m_Data[2][2] * m_Data[3][1] +
		m_Data[2][0] * m_Data[1][1] * m_Data[3][2] -
		m_Data[2][0] * m_Data[1][2] * m_Data[3][1] -
		m_Data[3][0] * m_Data[1][1] * m_Data[2][2] +
		m_Data[3][0] * m_Data[1][2] * m_Data[2][1];

	inv[0][1] = 
		-m_Data[0][1] * m_Data[2][2] * m_Data[3][3] +
		m_Data[0][1] * m_Data[2][3] * m_Data[3][2] +
		m_Data[2][1] * m_Data[0][2] * m_Data[3][3] -
		m_Data[2][1] * m_Data[0][3] * m_Data[3][2] -
		m_Data[3][1] * m_Data[0][2] * m_Data[2][3] +
		m_Data[3][1] * m_Data[0][3] * m_Data[2][2];

	inv[1][1] = 
		m_Data[0][0] * m_Data[2][2] * m_Data[3][3] -
		m_Data[0][0] * m_Data[2][3] * m_Data[3][2] -
		m_Data[2][0] * m_Data[0][2] * m_Data[3][3] +
		m_Data[2][0] * m_Data[0][3] * m_Data[3][2] +
		m_Data[3][0] * m_Data[0][2] * m_Data[2][3] -
		m_Data[3][0] * m_Data[0][3] * m_Data[2][2];

	inv[2][1] = 
		-m_Data[0][0] * m_Data[2][1] * m_Data[3][3] +
		m_Data[0][0] * m_Data[2][3] * m_Data[3][1] +
		m_Data[2][0] * m_Data[0][1] * m_Data[3][3] -
		m_Data[2][0] * m_Data[0][3] * m_Data[3][1] -
		m_Data[3][0] * m_Data[0][1] * m_Data[2][3] +
		m_Data[3][0] * m_Data[0][3] * m_Data[2][1];

	inv[3][1] = 
		m_Data[0][0] * m_Data[2][1] * m_Data[3][2] -
		m_Data[0][0] * m_Data[2][2] * m_Data[3][1] -
		m_Data[2][0] * m_Data[0][1] * m_Data[3][2] +
		m_Data[2][0] * m_Data[0][2] * m_Data[3][1] +
		m_Data[3][0] * m_Data[0][1] * m_Data[2][2] -
		m_Data[3][0] * m_Data[0][2] * m_Data[2][1];

	inv[0][2] = 
		m_Data[0][1] * m_Data[1][2] * m_Data[3][3] -
		m_Data[0][1] * m_Data[1][3] * m_Data[3][2] -
		m_Data[1][1] * m_Data[0][2] * m_Data[3][3] +
		m_Data[1][1] * m_Data[0][3] * m_Data[3][2] +
		m_Data[3][1] * m_Data[0][2] * m_Data[1][3] -
		m_Data[3][1] * m_Data[0][3] * m_Data[1][2];

	inv[1][2] = 
		-m_Data[0][0] * m_Data[1][2] * m_Data[3][3] +
		m_Data[0][0] * m_Data[1][3] * m_Data[3][2] +
		m_Data[1][0] * m_Data[0][2] * m_Data[3][3] -
		m_Data[1][0] * m_Data[0][3] * m_Data[3][2] -
		m_Data[3][0] * m_Data[0][2] * m_Data[1][3] +
		m_Data[3][0] * m_Data[0][3] * m_Data[1][2];

	inv[2][2] = 
		m_Data[0][0] * m_Data[1][1] * m_Data[3][3] -
		m_Data[0][0] * m_Data[1][3] * m_Data[3][1] -
		m_Data[1][0] * m_Data[0][1] * m_Data[3][3] +
		m_Data[1][0] * m_Data[0][3] * m_Data[3][1] +
		m_Data[3][0] * m_Data[0][1] * m_Data[1][3] -
		m_Data[3][0] * m_Data[0][3] * m_Data[1][1];

	inv[3][2] = 
		-m_Data[0][0] * m_Data[1][1] * m_Data[3][2] +
		m_Data[0][0] * m_Data[1][2] * m_Data[3][1] +
		m_Data[1][0] * m_Data[0][1] * m_Data[3][2] -
		m_Data[1][0] * m_Data[0][2] * m_Data[3][1] -
		m_Data[3][0] * m_Data[0][1] * m_Data[1][2] +
		m_Data[3][0] * m_Data[0][2] * m_Data[1][1];

	inv[0][3] = 
		-m_Data[0][1] * m_Data[1][2] * m_Data[2][3] +
		m_Data[0][1] * m_Data[1][3] * m_Data[2][2] +
		m_Data[1][1] * m_Data[0][2] * m_Data[2][3] -
		m_Data[1][1] * m_Data[0][3] * m_Data[2][2] -
		m_Data[2][1] * m_Data[0][2] * m_Data[1][3] +
		m_Data[2][1] * m_Data[0][3] * m_Data[1][2];

	inv[1][3] = 
		m_Data[0][0] * m_Data[1][2] * m_Data[2][3] -
		m_Data[0][0] * m_Data[1][3] * m_Data[2][2] -
		m_Data[1][0] * m_Data[0][2] * m_Data[2][3] +
		m_Data[1][0] * m_Data[0][3] * m_Data[2][2] +
		m_Data[2][0] * m_Data[0][2] * m_Data[1][3] -
		m_Data[2][0] * m_Data[0][3] * m_Data[1][2];

	inv[2][3] = 
		-m_Data[0][0] * m_Data[1][1] * m_Data[2][3] +
		m_Data[0][0] * m_Data[1][3] * m_Data[2][1] +
		m_Data[1][0] * m_Data[0][1] * m_Data[2][3] -
		m_Data[1][0] * m_Data[0][3] * m_Data[2][1] -
		m_Data[2][0] * m_Data[0][1] * m_Data[1][3] +
		m_Data[2][0] * m_Data[0][3] * m_Data[1][1];

	inv[3][3] = 
		m_Data[0][0] * m_Data[1][1] * m_Data[2][2] -
		m_Data[0][0] * m_Data[1][2] * m_Data[2][1] -
		m_Data[1][0] * m_Data[0][1] * m_Data[2][2] +
		m_Data[1][0] * m_Data[0][2] * m_Data[2][1] +
		m_Data[2][0] * m_Data[0][1] * m_Data[1][2] -
		m_Data[2][0] * m_Data[0][2] * m_Data[1][1];

	const float det = 1.f / (
		m_Data[0][0] * inv[0][0] + 
		m_Data[0][1] * inv[1][0] + 
		m_Data[0][2] * inv[2][0] + 
		m_Data[0][3] * inv[3][0] );

	for (int8 i = 0; i < 4; ++i)
		for (int8 j = 0; j < 4; ++j)
			inv[i][j] *= det;

	std::memcpy(&m_Data, &inv, sizeof(float) * 16);
}

inline void Matrix4x4::Transpose() noexcept
{
	std::swap(m_Data[0][1], m_Data[1][0]);
	std::swap(m_Data[0][2], m_Data[2][0]);
	std::swap(m_Data[0][3], m_Data[3][0]);

	std::swap(m_Data[1][2], m_Data[2][1]);
	std::swap(m_Data[1][3], m_Data[3][1]);

	std::swap(m_Data[2][3], m_Data[3][2]);
}

inline void Matrix4x4::Translate(const Vector3f& value) noexcept
{
	m_Data[3][0] += value.x;
	m_Data[3][1] += value.y;
	m_Data[3][2] += value.z;
}

inline void Matrix4x4::Rotate(const Quaternion& value) noexcept
{
	*this *= Matrix4x4::FromRotate(value);
}

inline void Matrix4x4::Scale(const float value) noexcept
{
	*this *= Matrix4x4::FromScale(value);
}

inline void Matrix4x4::Scale(const Vector3f& value) noexcept
{
	*this *= Matrix4x4::FromScale(value);
}

inline bool Matrix4x4::IsValid() const noexcept
{
	return Vector3f(m_Data[0][0], m_Data[0][1], m_Data[0][2]).Length() > KINDA_SMALL_FLOAT
		&& Vector3f(m_Data[1][0], m_Data[1][1], m_Data[1][2]).Length() > KINDA_SMALL_FLOAT
		&& Vector3f(m_Data[2][2], m_Data[2][1], m_Data[2][2]).Length() > KINDA_SMALL_FLOAT;
}


inline Matrix4x4 Matrix4x4::Inversed() const noexcept
{
	Matrix4x4 value(*this);
	value.Inverse();
	return value;
}

inline Matrix4x4 Matrix4x4::Transposed() const noexcept
{
	Matrix4x4 value(*this);
	value.Transpose();
	return value;
}

inline void Matrix4x4::SetTranslate(const Vector3f& value) noexcept
{
	m_Data[3][0] = value.x; 
	m_Data[3][1] = value.y; 
	m_Data[3][2] = value.z; 
}

inline void Matrix4x4::SetRotate(const Rotator& value) noexcept
{
	const float s0 = Vector3f(m_Data[0][0], m_Data[0][1], m_Data[0][2]).Length();
	const float s1 = Vector3f(m_Data[1][0], m_Data[1][1], m_Data[1][2]).Length();
	const float s2 = Vector3f(m_Data[2][0], m_Data[2][1], m_Data[2][2]).Length();

	const float x = math::ToRadians(value.m_Pitch);
	const float y = math::ToRadians(value.m_Yaw);
	const float z = math::ToRadians(value.m_Roll);
	const float sinX = std::sin(x);
	const float sinY = std::sin(y);
	const float sinZ = std::sin(z);
	const float cosX = std::cos(x);
	const float cosY = std::cos(y);
	const float cosZ = std::cos(z);

	m_Data[0][0] = cosY * cosZ - sinY * sinX * sinZ * s0;
	m_Data[0][1] = -sinZ * cosX * s0;
	m_Data[0][2] = sinY * cosZ + cosY * sinX * sinZ * s0;

	m_Data[1][0] = cosY * sinZ + sinY * sinX * cosZ * s1;
	m_Data[1][1] = cosZ * cosX * s1;
	m_Data[1][2] = sinZ * sinY - cosY * sinX * cosZ * s1;

	m_Data[2][0] = -sinY * cosX * s2;
	m_Data[2][1] = -sinX * s2;
	m_Data[2][2] = cosY * cosX * s2;
}

inline void Matrix4x4::SetRotate(const Quaternion& value) noexcept
{
	const float s0 = Vector3f(m_Data[0][0], m_Data[0][1], m_Data[0][2]).Length();
	const float s1 = Vector3f(m_Data[1][0], m_Data[1][1], m_Data[1][2]).Length();
	const float s2 = Vector3f(m_Data[2][0], m_Data[2][1], m_Data[2][2]).Length();
	
	m_Data[0][0] = 1.f - (2.f * value.y * value.y) - (2.f * value.z * value.z) * s0;
	m_Data[0][1] = (2.f * value.x * value.y) + (2.f * value.z * value.w) * s0;
	m_Data[0][2] = (2.f * value.x * value.z) - (2.f * value.y * value.w) * s0;

	m_Data[1][0] = (2.f * value.x * value.y) - (2.f * value.z * value.w) * s1;
	m_Data[1][1] = 1.f - (2.f * value.x * value.x) - (2.f * value.z * value.z) * s1;
	m_Data[1][2] = (2.f * value.y * value.z) + (2.f * value.x * value.w) * s1;

	m_Data[2][0] = (2.f * value.x * value.z) + (2.f * value.y * value.w) * s2;
	m_Data[2][1] = (2.f * value.y * value.z) - (2.f * value.x * value.w) * s2;
	m_Data[2][2] = 1.f - (2.f * value.x * value.x) - (2.f * value.y * value.y) * s2;
}

inline void Matrix4x4::SetScale(const float value) noexcept
{
	const Vector3f r0 = Vector3f(m_Data[0][0], m_Data[0][1], m_Data[0][2]).Normalized();
	const Vector3f r1 = Vector3f(m_Data[1][0], m_Data[1][1], m_Data[1][2]).Normalized();
	const Vector3f r2 = Vector3f(m_Data[2][0], m_Data[2][1], m_Data[2][2]).Normalized();

	m_Data[0][0] = r0.x * value;
	m_Data[0][1] = r0.y * value;
	m_Data[0][2] = r0.z * value;

	m_Data[1][0] = r1.x * value;
	m_Data[1][1] = r1.y * value;
	m_Data[1][2] = r1.z * value;

	m_Data[2][0] = r2.x * value;
	m_Data[2][1] = r2.y * value;
	m_Data[2][2] = r2.z * value;
}

inline void Matrix4x4::SetScale(const Vector3f& value) noexcept
{
	const Vector3f r0 = Vector3f(m_Data[0][0], m_Data[0][1], m_Data[0][2]).Normalized();
	const Vector3f r1 = Vector3f(m_Data[1][0], m_Data[1][1], m_Data[1][2]).Normalized();
	const Vector3f r2 = Vector3f(m_Data[2][0], m_Data[2][1], m_Data[2][2]).Normalized();

	m_Data[0][0] = r0.x * value.x;
	m_Data[0][1] = r0.y * value.x;
	m_Data[0][2] = r0.z * value.x;

	m_Data[1][0] = r1.x * value.y;
	m_Data[1][1] = r1.y * value.y;
	m_Data[1][2] = r1.z * value.y;

	m_Data[2][0] = r2.x * value.z;
	m_Data[2][1] = r2.y * value.z;
	m_Data[2][2] = r2.z * value.z;
}

inline Matrix4x4 Matrix4x4::FromTransform(const Vector3f& translate, const Rotator& rotate, const float scale) noexcept
{
	Matrix4x4 matrix = FromRotate(rotate);
	matrix.m_Data[0][0] *= scale;
	matrix.m_Data[0][1] *= scale;
	matrix.m_Data[0][2] *= scale;

	matrix.m_Data[1][0] *= scale;
	matrix.m_Data[1][1] *= scale;
	matrix.m_Data[1][2] *= scale;

	matrix.m_Data[2][0] *= scale;
	matrix.m_Data[2][1] *= scale;
	matrix.m_Data[2][2] *= scale;

	matrix.m_Data[3][0] = translate.x;
	matrix.m_Data[3][1] = translate.y;
	matrix.m_Data[3][2] = translate.z;
	return matrix;
}

inline Matrix4x4 Matrix4x4::FromTransform(const Vector3f& translate, const Rotator& rotate, const Vector3f& scale) noexcept
{
	Matrix4x4 matrix = FromRotate(rotate);
	matrix.m_Data[0][0] *= scale.x; 
	matrix.m_Data[0][1] *= scale.x; 
	matrix.m_Data[0][2] *= scale.x;

	matrix.m_Data[1][0] *= scale.y; 
	matrix.m_Data[1][1] *= scale.y; 
	matrix.m_Data[1][2] *= scale.y;

	matrix.m_Data[2][0] *= scale.z; 
	matrix.m_Data[2][1] *= scale.z; 
	matrix.m_Data[2][2] *= scale.z;

	matrix.m_Data[3][0] = translate.x;
	matrix.m_Data[3][1] = translate.y;
	matrix.m_Data[3][2] = translate.z;
	return matrix;
}

// https://stackoverflow.com/questions/1996957/conversion-euler-to-matrix-and-matrix-to-euler
inline Matrix4x4 Matrix4x4::FromRotate(const Rotator& value) noexcept
{
	const float x = math::ToRadians(value.m_Pitch);
	const float y = math::ToRadians(value.m_Yaw);
	const float z = math::ToRadians(value.m_Roll);

	const float cosP = std::cosf(x);
	const float sinP = std::sinf(x);
	const float cosY = std::cosf(y);
	const float sinY = std::sinf(y);
	const float cosR = std::cosf(z);
	const float sinR = std::sinf(z);

	Matrix4x4 matrix;
	matrix[0][0] = cosY * cosR + sinY * sinP * sinR;
	matrix[0][1] = cosP * sinR;
	matrix[0][2] = sinR * cosY * sinP - sinY * cosR;
	matrix[0][3] = 0.f;
	matrix[1][0] = cosR * sinY * sinP - sinR * cosY;
	matrix[1][1] = cosR * cosP;
	matrix[1][2] = sinY * sinR + cosR * cosY * sinP;
	matrix[1][3] = 0.f;
	matrix[2][0] = cosP * sinY;
	matrix[2][1] = -sinP;
	matrix[2][2] = cosP * cosY;
	matrix[2][3] = 0.f;
	matrix[3][0] = 0.f;
	matrix[3][1] = 0.f;
	matrix[3][1] = 0.f;
	matrix[3][3] = 1.f;
	return matrix;
}

inline constexpr Matrix4x4 Matrix4x4::FromTransform(const Vector3f& translate, const float scale) noexcept
{
	Matrix4x4 matrix = Identity;
	matrix.m_Data[0][0] *= scale;
	matrix.m_Data[0][1] *= scale;
	matrix.m_Data[0][2] *= scale;
	matrix.m_Data[0][3] = 0.f;

	matrix.m_Data[1][0] *= scale;
	matrix.m_Data[1][1] *= scale;
	matrix.m_Data[1][2] *= scale;
	matrix.m_Data[1][3] = 0.f;

	matrix.m_Data[2][0] *= scale;
	matrix.m_Data[2][1] *= scale;
	matrix.m_Data[2][2] *= scale;
	matrix.m_Data[2][3] = 0.f;

	matrix.m_Data[3][0] = translate.x;
	matrix.m_Data[3][1] = translate.y;
	matrix.m_Data[3][2] = translate.z;
	matrix.m_Data[3][3] = 1.f;
	return matrix;
}

inline constexpr Matrix4x4 Matrix4x4::FromTransform(const Vector3f& translate, const Vector3f& scale) noexcept
{
	Matrix4x4 matrix = Identity;
	matrix.m_Data[0][0] *= scale.x;
	matrix.m_Data[0][1] *= scale.x;
	matrix.m_Data[0][2] *= scale.x;
	matrix.m_Data[0][3] = 0.f;

	matrix.m_Data[1][0] *= scale.y;
	matrix.m_Data[1][1] *= scale.y;
	matrix.m_Data[1][2] *= scale.y;
	matrix.m_Data[1][3] = 0.f;

	matrix.m_Data[2][0] *= scale.z;
	matrix.m_Data[2][1] *= scale.z;
	matrix.m_Data[2][2] *= scale.z;
	matrix.m_Data[2][3] = 0.f;

	matrix.m_Data[3][0] = translate.x;
	matrix.m_Data[3][1] = translate.y;
	matrix.m_Data[3][2] = translate.z;
	matrix.m_Data[3][3] = 1.f;
	return matrix;
}

inline constexpr Matrix4x4 Matrix4x4::FromTransform(const Vector3f& translate, const Matrix3x3& rotate, const float scale) noexcept
{
	Matrix4x4 matrix(rotate);
	matrix.m_Data[0][0] *= scale;
	matrix.m_Data[0][1] *= scale;
	matrix.m_Data[0][2] *= scale;
	matrix.m_Data[0][3] = 0.f;

	matrix.m_Data[1][0] *= scale;
	matrix.m_Data[1][1] *= scale;
	matrix.m_Data[1][2] *= scale;
	matrix.m_Data[1][3] = 0.f;

	matrix.m_Data[2][0] *= scale;
	matrix.m_Data[2][1] *= scale;
	matrix.m_Data[2][2] *= scale;
	matrix.m_Data[2][3] = 0.f;

	matrix.m_Data[3][0] = translate.x;
	matrix.m_Data[3][1] = translate.y;
	matrix.m_Data[3][2] = translate.z;
	matrix.m_Data[3][3] = 1.f;
	return matrix;
}

inline constexpr Matrix4x4 Matrix4x4::FromTransform(const Vector3f& translate, const Matrix3x3& rotate, const Vector3f& scale) noexcept
{
	Matrix4x4 matrix(rotate);
	matrix.m_Data[0][0] *= scale.x;
	matrix.m_Data[0][1] *= scale.x;
	matrix.m_Data[0][2] *= scale.x;
	matrix.m_Data[0][3] = 0.f;

	matrix.m_Data[1][0] *= scale.y;
	matrix.m_Data[1][1] *= scale.y;
	matrix.m_Data[1][2] *= scale.y;
	matrix.m_Data[1][3] = 0.f;

	matrix.m_Data[2][0] *= scale.z;
	matrix.m_Data[2][1] *= scale.z;
	matrix.m_Data[2][2] *= scale.z;
	matrix.m_Data[2][3] = 0.f;

	matrix.m_Data[3][0] = translate.x;
	matrix.m_Data[3][1] = translate.y;
	matrix.m_Data[3][2] = translate.z;
	matrix.m_Data[3][3] = 1.f;
	return matrix;
}

inline constexpr Matrix4x4 Matrix4x4::FromTransform(const Vector3f& translate, const Quaternion& rotate, const float scale) noexcept
{
	Matrix4x4 matrix = Matrix4x4::FromRotate(rotate);
	matrix.m_Data[0][0] *= scale;
	matrix.m_Data[0][1] *= scale;
	matrix.m_Data[0][2] *= scale;

	matrix.m_Data[1][0] *= scale;
	matrix.m_Data[1][1] *= scale;
	matrix.m_Data[1][2] *= scale;

	matrix.m_Data[2][0] *= scale;
	matrix.m_Data[2][1] *= scale;
	matrix.m_Data[2][2] *= scale;

	matrix.m_Data[3][0] = translate.x;
	matrix.m_Data[3][1] = translate.y;
	matrix.m_Data[3][2] = translate.z;
	return matrix;
}

inline constexpr Matrix4x4 Matrix4x4::FromTransform(const Vector3f& translate, const Quaternion& rotate, const Vector3f& scale) noexcept
{
	Matrix4x4 matrix = Matrix4x4::FromRotate(rotate);
	matrix.m_Data[0][0] *= scale.x;
	matrix.m_Data[0][1] *= scale.x;
	matrix.m_Data[0][2] *= scale.x;

	matrix.m_Data[1][0] *= scale.y;
	matrix.m_Data[1][1] *= scale.y;
	matrix.m_Data[1][2] *= scale.y;

	matrix.m_Data[2][0] *= scale.z;
	matrix.m_Data[2][1] *= scale.z;
	matrix.m_Data[2][2] *= scale.z;

	matrix.m_Data[3][0] = translate.x;
	matrix.m_Data[3][1] = translate.y;
	matrix.m_Data[3][2] = translate.z;
	return matrix;
}

inline constexpr Matrix4x4 Matrix4x4::FromTranslate(const Vector3f& value) noexcept
{
	return Matrix4x4{
		1.f, 0.f, 0.f, 0.f,
		0.f, 1.f, 0.f, 0.f,
		0.f, 0.f, 1.f, 0.f,
		value.x, value.y, value.z, 1.f };
}

inline constexpr Matrix4x4 Matrix4x4::FromRotate(const Quaternion& value) noexcept
{
	return Matrix4x4(
		1.f - (2.f * value.y * value.y) - (2.f * value.z * value.z),
		(2.f * value.x * value.y) + (2.f * value.z * value.w),
		(2.f * value.x * value.z) - (2.f * value.y * value.w),
		0.f,

		(2.f * value.x * value.y) - (2.f * value.z * value.w),
		1.f - (2.f * value.x * value.x) - (2.f * value.z * value.z),
		(2.f * value.y * value.z) + (2.f * value.x * value.w),
		0.f,

		(2.f * value.x * value.z) + (2.f * value.y * value.w),
		(2.f * value.y * value.z) - (2.f * value.x * value.w),
		1.f - (2.f * value.x * value.x) - (2.f * value.y * value.y),
		0.f,

		0.f,
		0.f,
		0.f,
		1.f);
}

inline constexpr Matrix4x4 Matrix4x4::FromScale(const Vector3f& value) noexcept
{
	return Matrix4x4{
		value.x, 0.f, 0.f, 0.f,
		0.f, value.y, 0.f, 0.f,
		0.f, 0.f, value.z, 0.f,
		0.f, 0.f, 0.f, 1.f };
}

inline constexpr Matrix4x4 Matrix4x4::FromScale(const float value) noexcept
{
	return Matrix4x4{
		value, 0.f, 0.f, 0.f,
		0.f, value, 0.f, 0.f,
		0.f, 0.f, value, 0.f,
		0.f, 0.f, 0.f, 1.f };
}

inline constexpr Vector2f operator*(const Vector2f& lhs, const Matrix4x4& rhs) noexcept
{
	return Vector2f(
		rhs.m_Data[0][0] * lhs.x + rhs.m_Data[1][0] * lhs.y + rhs.m_Data[3][0],
		rhs.m_Data[0][1] * lhs.x + rhs.m_Data[1][1] * lhs.y + rhs.m_Data[3][1]);
}

inline constexpr Vector3f operator*(const Vector3f& lhs, const Matrix4x4& rhs) noexcept
{
	return Vector3f(
		rhs.m_Data[0][0] * lhs.x + rhs.m_Data[1][0] * lhs.y + rhs.m_Data[2][0] * lhs.z + rhs.m_Data[3][0],
		rhs.m_Data[0][1] * lhs.x + rhs.m_Data[1][1] * lhs.y + rhs.m_Data[2][1] * lhs.z + rhs.m_Data[3][1],
		rhs.m_Data[0][2] * lhs.x + rhs.m_Data[1][2] * lhs.y + rhs.m_Data[2][2] * lhs.z + rhs.m_Data[3][2]);
}

inline constexpr Vector4f operator*(const Vector4f& lhs, const Matrix4x4& rhs) noexcept
{
	return Vector4f(
		rhs.m_Data[0][0] * lhs.x + rhs.m_Data[1][0] * lhs.y + rhs.m_Data[2][0] * lhs.z + rhs.m_Data[3][0] * lhs.w,
		rhs.m_Data[0][1] * lhs.x + rhs.m_Data[1][1] * lhs.y + rhs.m_Data[2][1] * lhs.z + rhs.m_Data[3][1] * lhs.w,
		rhs.m_Data[0][2] * lhs.x + rhs.m_Data[1][2] * lhs.y + rhs.m_Data[2][2] * lhs.z + rhs.m_Data[3][2] * lhs.w,
		rhs.m_Data[0][3] * lhs.x + rhs.m_Data[1][3] * lhs.y + rhs.m_Data[2][3] * lhs.z + rhs.m_Data[3][3] * lhs.w);
}