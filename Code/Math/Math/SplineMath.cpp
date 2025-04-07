#include "Math/SplineMath.h"

#include "Math/Vector.h"

Vector2f math::BezierQuadratic(const Vector2f& p1, const Vector2f& c, const Vector2f& p2, float t)
{
	const Vector2f ip0 = math::Lerp(p1, c, t);
	const Vector2f ip1 = math::Lerp(c, p2, t);
	return math::Lerp(ip0, ip1, t);
}

Vector2f math::BezierCubic(const Vector2f& p1, const Vector2f& c1, const Vector2f& c2, const Vector2f& p2, float t)
{
	const Vector2f ip0 = math::Lerp(p1, c1, t);
	const Vector2f ip1 = math::Lerp(c1, c2, t);
	const Vector2f ip2 = math::Lerp(c2, p2, t);
	const Vector2f ic2 = math::Lerp(ip0, ip1, t);
	const Vector2f ip4 = math::Lerp(ip1, ip2, t);
	return math::Lerp(ic2, ip4, t);
}

// https://github.com/chen0040/cpp-spline/blob/master/spline/src/main/cpp/CatmullRom.cpp
Vector2f math::CatmullRom(const Vector2f& c1, const Vector2f& p1, const Vector2f& p2, const Vector2f& c2, const float t)
{
	const float t2 = t * t;
	const float t3 = t2 * t;
	const Vector2f a = p1 * 3.f - c1 - p2 * 3.f + c2;
	const Vector2f b = c1 * 2.f - p1 * 5.f + p2 * 4.f - c2;
	const Vector2f c = p2 - c1;
	const Vector2f d = p1 * 2.f;
	return (a * t3 + b * t2 + c * t + d) * 0.5f;
}

// https://github.com/keshavnandan/OpenGL/blob/master/hermite_curve/hermite.cpp
Vector2f math::Hermite(const Vector2f& p1, const Vector2f& v1, const Vector2f& p2, const Vector2f& v2, const float t)
{
	const float h0 =  2.f*t*t*t - 3.f*t*t + 1.f;
	const float h1 = -2.f*t*t*t + 3.f*t*t;
	const float h2 =      t*t*t - 2.f*t*t + t;
	const float h3 =      t*t*t -     t*t;
	return p1*h0 + p2*h1 + v1*h2 + v2*h3;
}