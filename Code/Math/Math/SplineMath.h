#pragma once

class Vector2f;

namespace math
{
	/// \brief Quadratic bezier interpolation from p1 -> p2 based on t where the curve passes through p1 and p2.
	/// The direction and curvature can be adjusted by the control point c.
	Vector2f BezierQuadratic(const Vector2f& p1, const Vector2f& c, const Vector2f& p2, float t);

	/// \brief Cubic bezier interpolation from p1 -> p2 based on t where the curve passes through p1 and p2.
	/// The direction and curvature can be adjusted by the control points c1 and c2.
	Vector2f BezierCubic(const Vector2f& p1, const Vector2f& c1, const Vector2f& c2, const Vector2f& p2, float t);

	/// \brief CatmullRom interpolation from p1 -> p2 based on t where the curve passes through all points.
	/// The direction and curvature can be adjusted by the control points c1 and c2.
	Vector2f CatmullRom(const Vector2f& c1, const Vector2f& p1, const Vector2f& p2, const Vector2f& c2, const float t);

	/// \brief Hermite interpolation from p1 -> p2 based on t where the curve passes through p1 and p2.
	/// The direction and curvature can be adjusted by the velocity points v1 and v2.
	Vector2f Hermite(const Vector2f& p1, const Vector2f& v1, const Vector2f& p2, const Vector2f& v2, const float t);
}