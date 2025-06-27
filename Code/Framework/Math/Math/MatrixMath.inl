#pragma once

#include "Math/Math.h"

inline constexpr bool math::IsNearly(const Matrix3x3& a, const Matrix3x3& b, const float threshold /*= 0.001f*/) noexcept
{
	return Absolute(a.m_Data[0][0] - b.m_Data[0][0]) < threshold
		&& Absolute(a.m_Data[0][1] - b.m_Data[0][1]) < threshold
		&& Absolute(a.m_Data[0][2] - b.m_Data[0][2]) < threshold

		&& Absolute(a.m_Data[1][0] - b.m_Data[1][0]) < threshold
		&& Absolute(a.m_Data[1][1] - b.m_Data[1][1]) < threshold
		&& Absolute(a.m_Data[1][2] - b.m_Data[1][2]) < threshold

		&& Absolute(a.m_Data[2][0] - b.m_Data[2][0]) < threshold
		&& Absolute(a.m_Data[2][1] - b.m_Data[2][1]) < threshold
		&& Absolute(a.m_Data[2][2] - b.m_Data[2][2]) < threshold;
}

inline constexpr bool math::IsNearly(const Matrix4x4& a, const Matrix4x4& b, const float threshold /*= 0.001f*/) noexcept
{
	return Absolute(a.m_Data[0][0] - b.m_Data[0][0]) < threshold
		&& Absolute(a.m_Data[0][1] - b.m_Data[0][1]) < threshold
		&& Absolute(a.m_Data[0][2] - b.m_Data[0][2]) < threshold
		&& Absolute(a.m_Data[0][3] - b.m_Data[0][3]) < threshold

		&& Absolute(a.m_Data[1][0] - b.m_Data[1][0]) < threshold
		&& Absolute(a.m_Data[1][1] - b.m_Data[1][1]) < threshold
		&& Absolute(a.m_Data[1][2] - b.m_Data[1][2]) < threshold
		&& Absolute(a.m_Data[1][3] - b.m_Data[1][3]) < threshold

		&& Absolute(a.m_Data[2][0] - b.m_Data[2][0]) < threshold
		&& Absolute(a.m_Data[2][1] - b.m_Data[2][1]) < threshold
		&& Absolute(a.m_Data[2][2] - b.m_Data[2][2]) < threshold
		&& Absolute(a.m_Data[2][3] - b.m_Data[2][3]) < threshold

		&& Absolute(a.m_Data[3][0] - b.m_Data[3][0]) < threshold
		&& Absolute(a.m_Data[3][1] - b.m_Data[3][1]) < threshold
		&& Absolute(a.m_Data[3][2] - b.m_Data[3][2]) < threshold
		&& Absolute(a.m_Data[3][3] - b.m_Data[3][3]) < threshold;
}