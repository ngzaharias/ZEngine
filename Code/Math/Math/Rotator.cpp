#include "Math/Rotator.h"

#include "Math/Math.h"
#include "Math/Matrix.h"
#include "Math/Quaternion.h"

#include <cmath>

// https://stackoverflow.com/questions/1996957/conversion-euler-to-matrix-and-matrix-to-euler
Rotator Rotator::FromMatrix(const Matrix3x3& value)
{
	Rotator rotator;
	rotator.m_Pitch = std::asinf(-value[2][1]);
	if (std::cosf(rotator.m_Pitch) > 0.0001f)
	{
		rotator.m_Yaw = std::atan2f(value[2][0], value[2][2]);
		rotator.m_Roll = std::atan2f(value[0][1], value[1][1]);
	}
	else
	{
		rotator.m_Yaw = 0.0f;
		rotator.m_Roll = std::atan2f(-value[1][0], value[0][0]);
	}

	rotator.m_Pitch = math::ToDegrees(rotator.m_Pitch);
	rotator.m_Yaw   = math::ToDegrees(rotator.m_Yaw);
	rotator.m_Roll  = math::ToDegrees(rotator.m_Roll);
	return rotator;
}

Rotator Rotator::FromMatrix(const Matrix4x4& value)
{
	Rotator rotator;
	rotator.m_Pitch = std::asinf(-value[2][1]);
	if (std::cosf(rotator.m_Pitch) > 0.0001f)
	{
		rotator.m_Yaw = std::atan2f(value[2][0], value[2][2]);
		rotator.m_Roll = std::atan2f(value[0][1], value[1][1]);
	}
	else
	{
		rotator.m_Yaw = 0.0f;
		rotator.m_Roll = std::atan2f(-value[1][0], value[0][0]);
	}

	rotator.m_Pitch = math::ToDegrees(rotator.m_Pitch);
	rotator.m_Yaw   = math::ToDegrees(rotator.m_Yaw);
	rotator.m_Roll  = math::ToDegrees(rotator.m_Roll);
	return rotator;
}

// https://en.wikipedia.org/wiki/Conversion_between_quaternions_and_Euler_angles
Rotator Rotator::FromQuaternion(const Quaternion& value)
{
	Rotator rotator;
	rotator.m_Pitch = std::atan2(2.f * (value.w * value.x + value.y * value.z), 1.f - 2.f * (value.x * value.x + value.y * value.y));
	rotator.m_Yaw   = std::asin( 2.f * (value.w * value.y - value.z * value.x));
	rotator.m_Roll  = std::atan2(2.f * (value.w * value.z + value.x * value.y), 1.f - 2.f * (value.y * value.y + value.z * value.z));

	rotator.m_Pitch = math::ToDegrees(rotator.m_Pitch);
	rotator.m_Yaw   = math::ToDegrees(rotator.m_Yaw);
	rotator.m_Roll  = math::ToDegrees(rotator.m_Roll);
	return rotator;
}