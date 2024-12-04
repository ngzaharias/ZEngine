#pragma once

#include "Math/Math.h"
#include "Math/Matrix.h"
#include "Math/Rotator.h"
#include "Math/Vector.h"
#include "Math/VectorMath.h"

#include <cmath>

// https://www.geeks3d.com/20141201/how-to-rotate-a-vertex-by-a-quaternion-in-glsl/?fbclid=IwAR3k347hpiyblLTohmZTZnk_tbawsaXbiUewbaSSujV6WzBqhylS7fNIQFI

constexpr Quaternion::Quaternion()  noexcept
	: x(), y(), z(), w()
{
}

constexpr Quaternion::Quaternion(const float x, const float y, const float z, const float w)  noexcept
	: x(x), y(y), z(z), w(w)
{
}

constexpr Quaternion::Quaternion(const Vector3f& value, const float w) noexcept
	: x(value.x), y(value.y), z(value.z), w(w)
{
}

constexpr Quaternion::Quaternion(const Vector4f& value) noexcept
	: x(value.x), y(value.y), z(value.z), w(value.w)
{
}

inline constexpr Quaternion Quaternion::operator*(const Quaternion& rhs) const noexcept
{
	return Quaternion(
		((w * rhs.x + x * rhs.w) + y * rhs.z) - z * rhs.y,
		((w * rhs.y + y * rhs.w) + z * rhs.x) - x * rhs.z,
		((w * rhs.z + z * rhs.w) + x * rhs.y) - y * rhs.x,
		((w * rhs.w - x * rhs.x) - y * rhs.y) - z * rhs.z);
}

inline float Quaternion::Length() const noexcept
{
	return math::Sqrt(x * x + y * y + z * z + w * w);
}

inline void Quaternion::Inverse() noexcept
{
	x = -x;	y = -y;	z = -z;
}

inline void Quaternion::Normalize() noexcept
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

inline void Quaternion::NormalizeUnsafe() noexcept
{
	*this *= 1.f / Length();
}

inline constexpr Quaternion Quaternion::Inversed() const noexcept
{
	return Quaternion(-x, -y, -z, w);
}

inline Quaternion Quaternion::Normalized() const noexcept
{
	Quaternion value(*this);
	value.Normalize();
	return value;
}

inline Quaternion Quaternion::NormalizedUnsafe() const noexcept
{
	Quaternion value(*this);
	value.NormalizeUnsafe();
	return value;
}

// https://gist.github.com/shihyu/c5abf3ebff2f5f1cfd32a90968f04a3b
inline void Quaternion::ToAxisAngle(Vector3f& out_Axis, float& out_Angle) const noexcept
{
	out_Angle = acosf(w);
	float angleSin = sinf(out_Angle);
	if (fabsf(angleSin) > KINDA_SMALL_FLOAT)
	{
		angleSin = 1.0f / angleSin;

		out_Axis.x = x * angleSin;
		out_Axis.y = y * angleSin;
		out_Axis.z = z * angleSin;

		out_Angle *= 2.0f;
		if (out_Angle > PI_ONE)
			out_Angle = PI_TWO - out_Angle;
	}
	else
	{
		out_Angle = 0.0f;
		out_Axis.x = 1.0f;
		out_Axis.y = 0.0f;
		out_Axis.z = 0.0f;
	}
}

// https://gist.github.com/shihyu/c5abf3ebff2f5f1cfd32a90968f04a3b
inline float Quaternion::Angle(const Quaternion& a, const Quaternion& b) noexcept
{
	const float angle = acosf((b * a.Inversed()).w) * 2.0f;
	if (angle > PI_ONE)
		return PI_TWO - angle;
	return angle;
}

inline constexpr float Quaternion::Dot(const Quaternion& a, const Quaternion& b) noexcept
{
	return a.x * b.x * a.y * b.y + a.z * b.z + a.w * b.w;
}

inline constexpr Quaternion Quaternion::Lerp(const Quaternion& from, const Quaternion& to, const float t) noexcept
{
	return (from * (1.0f - t)) + (to * t);
}

// https://gist.github.com/shihyu/c5abf3ebff2f5f1cfd32a90968f04a3b
inline Quaternion Quaternion::Slerp(const Quaternion& from, const Quaternion& to, const float t) noexcept
{
	Quaternion result = to;

	float cosTheta = Quaternion::Dot(from, to);
	if (cosTheta < 0.0f)
	{
		cosTheta = -cosTheta;
		result = -result;
	}

	const float theta = acosf(cosTheta);
	const float sinTheta = 1.0f / sinf(theta);
	result = (from * sinf(theta * (1.0f - t))) +
		(result * sinf(t * theta)) * sinTheta;
	result.Normalize();
	return result;
}

inline Quaternion Quaternion::FromAxisAngle(const Vector3f& axis, const float radians) noexcept
{
	const float half = radians * 0.5f;
	return Quaternion(axis * sinf(half), cosf(half));
}

// https://www.euclideanspace.com/maths/geometry/rotations/conversions/matrixToQuaternion/
inline Quaternion Quaternion::FromMatrix(const Matrix3x3& value) noexcept
{
	Quaternion quaternion;

	const float trace = value.m_Data[0][0] + value.m_Data[1][1] + value.m_Data[2][2];
	if (trace > 0)
	{
		const float s = std::sqrtf(trace + 1.f) * 2.f;
		quaternion.x = (value[1][2] - value[2][1]) / s;
		quaternion.y = (value[2][0] - value[0][2]) / s;
		quaternion.z = (value[0][1] - value[1][0]) / s;
		quaternion.w = 0.25f * s;
	}
	else if ((value[0][0] > value[1][1]) & (value[0][0] > value[2][2]))
	{
		const float s = std::sqrtf(1.f + value[0][0] - value[1][1] - value[2][2]) * 2.f;
		quaternion.x = 0.25f * s;
		quaternion.y = (value[0][1] + value[1][0]) / s;
		quaternion.z = (value[0][2] + value[2][0]) / s;
		quaternion.w = (value[1][2] - value[2][1]) / s;
	}
	else if (value[1][1] > value[2][2])
	{
		const float s = std::sqrtf(1.f + value[1][1] - value[0][0] - value[2][2]) * 2.f;
		quaternion.x = (value[0][1] + value[1][0]) / s;
		quaternion.y = 0.25f * s;
		quaternion.z = (value[1][2] + value[2][1]) / s;
		quaternion.w = (value[2][0] - value[0][2]) / s;
	}
	else
	{
		const float s = std::sqrtf(1.f + value[2][2] - value[0][0] - value[1][1]) * 2.f;
		quaternion.x = (value[0][2] + value[2][0]) / s;
		quaternion.y = (value[1][2] + value[2][1]) / s;
		quaternion.z = 0.25f * s;
		quaternion.w = (value[0][1] - value[1][0]) / s;
	}
	return quaternion;
}

// https://www.euclideanspace.com/maths/geometry/rotations/conversions/matrixToQuaternion/
inline Quaternion Quaternion::FromMatrix(const Matrix4x4& value) noexcept
{
	Quaternion quaternion;

	const float trace = value.m_Data[0][0] + value.m_Data[1][1] + value.m_Data[2][2];
	if (trace > 0)
	{
		const float s = std::sqrtf(trace + 1.f) * 2.f;
		quaternion.x = (value[1][2] - value[2][1]) / s;
		quaternion.y = (value[2][0] - value[0][2]) / s;
		quaternion.z = (value[0][1] - value[1][0]) / s;
		quaternion.w = 0.25f * s;
	}
	else if ((value[0][0] > value[1][1]) & (value[0][0] > value[2][2]))
	{
		const float s = std::sqrtf(1.f + value[0][0] - value[1][1] - value[2][2]) * 2.f;
		quaternion.x = 0.25f * s;
		quaternion.y = (value[0][1] + value[1][0]) / s;
		quaternion.z = (value[0][2] + value[2][0]) / s;
		quaternion.w = (value[1][2] - value[2][1]) / s;
	}
	else if (value[1][1] > value[2][2])
	{
		const float s = std::sqrtf(1.f + value[1][1] - value[0][0] - value[2][2]) * 2.f;
		quaternion.x = (value[0][1] + value[1][0]) / s;
		quaternion.y = 0.25f * s;
		quaternion.z = (value[1][2] + value[2][1]) / s;
		quaternion.w = (value[2][0] - value[0][2]) / s;
	}
	else
	{
		const float s = std::sqrtf(1.f + value[2][2] - value[0][0] - value[1][1]) * 2.f;
		quaternion.x = (value[0][2] + value[2][0]) / s;
		quaternion.y = (value[1][2] + value[2][1]) / s;
		quaternion.z = 0.25f * s;
		quaternion.w = (value[0][1] - value[1][0]) / s;
	}
	return quaternion;
}

inline Quaternion Quaternion::FromRotator(const Rotator& value) noexcept
{
	Quaternion quaternion = Quaternion::Identity;
	quaternion = quaternion * Quaternion::FromAxisAngle(Vector3f::AxisX, math::ToRadians(value.m_Pitch));
	quaternion = Quaternion::FromAxisAngle(Vector3f::AxisY, math::ToRadians(value.m_Yaw)) * quaternion;
	quaternion = Quaternion::FromAxisAngle(Vector3f::AxisZ, math::ToRadians(value.m_Roll)) * quaternion;
	return quaternion;
}

// https://gamedev.stackexchange.com/questions/28395/rotating-vector3-by-a-quaternion
inline constexpr Vector2f operator*(const Vector2f& lhs, const Quaternion& rhs) noexcept
{
	const Vector3f lhsv(lhs, 0.f);
	const Vector3f rhsv(rhs.x, rhs.y, rhs.z);
	const Vector3f result = rhsv * (2.0f * math::Dot(rhsv, lhsv))
		+ lhsv * (rhs.w * rhs.w - math::Dot(rhsv, rhsv))
		+ math::Cross(rhsv, lhsv) * (2.0f * rhs.w);
	return result.XY();
}

// https://gamedev.stackexchange.com/questions/28395/rotating-vector3-by-a-quaternion
inline constexpr Vector3f operator*(const Vector3f& lhs, const Quaternion& rhs) noexcept
{
	const Vector3f rhsv(rhs.x, rhs.y, rhs.z);
	return rhsv * (2.0f * math::Dot(rhsv, lhs))
		+ lhs * (rhs.w * rhs.w - math::Dot(rhsv, rhsv))
		+ math::Cross(rhsv, lhs) * (2.0f * rhs.w);
}

inline constexpr Vector2f& operator*=(Vector2f& lhs, const Quaternion& rhs) noexcept
{
	return lhs = lhs * rhs;
}

inline constexpr Vector3f& operator*=(Vector3f& lhs, const Quaternion& rhs) noexcept
{
	return lhs = lhs * rhs;
}