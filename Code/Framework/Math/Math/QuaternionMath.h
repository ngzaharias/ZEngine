#pragma once

#include "Math/Quaternion.h"

namespace math
{
	/// \brief Check if the distance between two quaternions is less than the threshold.
	constexpr bool IsNearly(const Quaternion& a, const Quaternion& b, const float threshold = 0.001f) noexcept;
}

#include "QuaternionMath.inl"