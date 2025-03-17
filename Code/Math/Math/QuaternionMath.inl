#pragma once

inline constexpr bool math::IsNearly(const Quaternion& a, const Quaternion& b, const float threshold /*= 0.001f*/) noexcept
{
	return (a - b).LengthSqr() < threshold;
}