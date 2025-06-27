#pragma once

#include "Math/Matrix.h"

namespace math
{
	/// \brief Check if the distance between two matrices is less than the threshold.
	constexpr bool IsNearly(const Matrix3x3& a, const Matrix3x3& b, const float threshold = 0.001f) noexcept;
	/// \brief Check if the distance between two matrices is less than the threshold.
	constexpr bool IsNearly(const Matrix4x4& a, const Matrix4x4& b, const float threshold = 0.001f) noexcept;
}

#include "MatrixMath.inl"