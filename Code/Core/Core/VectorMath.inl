#pragma once

#ifdef VECTOR_ASSERTS_ENABLED
#include <cassert>
#define VECTOR_ASSERT(condition, ...) assert(condition)
#else
#define VECTOR_ASSERT(...)
#endif

inline float math::Angle(const Vector2f& a, const Vector2f& b) noexcept
{
	return std::acos(math::Dot(a, b));
}

inline float math::Angle(const Vector3f& a, const Vector3f& b) noexcept
{
	return std::acos(math::Dot(a, b));
}

inline float math::AngleSigned(const Vector2f& a, const Vector2f& b) noexcept
{
	const float angle = math::Angle(a, b);
	if (a.x * b.y - a.y * b.x >= 0.f)
		return angle;
	return -angle;
}

inline float math::AngleSigned(const Vector3f& a, const Vector3f& b, const Vector3f& normal) noexcept
{
	const float angle = math::Angle(a, b);
	if (math::Dot(normal, math::Cross(a, b)) >= 0.f)
		return angle;
	return -angle;
}

inline constexpr Vector2f math::Clamp(const Vector2f& value, const Vector2f& min, const Vector2f& max) noexcept
{
	return Vector2f(
		(value.x < min.x) ? min.x : (value.x > max.x) ? max.x : value.x,
		(value.y < min.y) ? min.y : (value.y > max.y) ? max.y : value.y);
}

inline constexpr Vector3f math::Clamp(const Vector3f& value, const Vector3f& min, const Vector3f& max) noexcept
{
	return Vector3f(
		(value.x < min.x) ? min.x : (value.x > max.x) ? max.x : value.x,
		(value.y < min.y) ? min.y : (value.y > max.y) ? max.y : value.y,
		(value.z < min.z) ? min.z : (value.z > max.z) ? max.z : value.z);
}

inline Vector2f math::ClampLength(const Vector2f& value, const float max) noexcept
{
	Vector2f result = value;
	const float length = result.Length();
	if (length > max)
		result *= (max / length);

	VECTOR_ASSERT(!std::isnan(result.x), "math::ClampLength resulted in NaN!");
	return result;
}

inline Vector3f math::ClampLength(const Vector3f& value, const float max) noexcept
{
	Vector3f result = value;
	const float length = result.Length();
	if (length > max)
		result *= (max / length);

	VECTOR_ASSERT(!std::isnan(result.x), "math::ClampLength resulted in NaN!");
	return result;
}

inline constexpr float math::Cross(const Vector2f& a, const Vector2f& b) noexcept
{
	return a.x * b.y - a.y * b.x;
}

inline constexpr Vector3f math::Cross(const Vector3f& a, const Vector3f& b) noexcept
{
	return Vector3f(
		(a.y * b.z) - (a.z * b.y),
		(a.z * b.x) - (a.x * b.z),
		(a.x * b.y) - (a.y * b.x));
}

inline float math::Distance(const Vector2f& a, const Vector2f& b) noexcept
{
	return (b - a).Length();
}

inline float math::Distance(const Vector3f& a, const Vector3f& b) noexcept
{
	return (b - a).Length();
}

inline float math::DistanceXY(const Vector3f& a, const Vector3f& b) noexcept
{
	return (b - a).LengthXY();
}

inline float math::DistanceXZ(const Vector3f& a, const Vector3f& b) noexcept
{
	return (b - a).LengthXZ();
}

inline constexpr float math::DistanceSqr(const Vector2f& a, const Vector2f& b) noexcept
{
	return (b - a).LengthSqr();
}

inline constexpr float math::DistanceSqr(const Vector3f& a, const Vector3f& b) noexcept
{
	return (b - a).LengthSqr();
}

inline constexpr float math::DistanceXYSqr(const Vector3f& a, const Vector3f& b) noexcept
{
	return (b - a).LengthXYSqr();
}

inline constexpr float math::DistanceXZSqr(const Vector3f& a, const Vector3f& b) noexcept
{
	return (b - a).LengthXZSqr();
}

inline constexpr Vector2f math::Divide(const Vector2f& a, const Vector2f& b) noexcept
{
	return Vector2f(a.x / b.x, a.y / b.y);
}

inline constexpr Vector3f math::Divide(const Vector3f& a, const Vector3f& b) noexcept
{
	return Vector3f(a.x / b.x, a.y / b.y, a.z / b.z);
}

inline constexpr float math::Dot(const Vector2f& a, const Vector2f& b) noexcept
{
	return a.x * b.x + a.y * b.y;
}

inline constexpr float math::Dot(const Vector3f& a, const Vector3f& b) noexcept
{
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

inline constexpr Vector2f math::Max(const Vector2f& a, const Vector2f& b) noexcept
{
	return Vector2f(
		(a.x > b.x) ? a.x : b.x,
		(a.y > b.y) ? a.y : b.y);
}

inline constexpr Vector3f math::Max(const Vector3f& a, const Vector3f& b) noexcept
{
	return Vector3f(
		(a.x > b.x) ? a.x : b.x,
		(a.y > b.y) ? a.y : b.y,
		(a.z > b.z) ? a.z : b.z);
}

inline constexpr Vector2f math::Min(const Vector2f& a, const Vector2f& b) noexcept
{
	return Vector2f(
		(a.x < b.x) ? a.x : b.x,
		(a.y < b.y) ? a.y : b.y);
}

inline constexpr Vector3f math::Min(const Vector3f& a, const Vector3f& b) noexcept
{
	return Vector3f(
		(a.x < b.x) ? a.x : b.x,
		(a.y < b.y) ? a.y : b.y,
		(a.z < b.z) ? a.z : b.z);
}

inline constexpr Vector2f math::Multiply(const Vector2f& a, const Vector2f& b) noexcept
{
	return Vector2f(a.x * b.x, a.y * b.y);
}

inline constexpr Vector3f math::Multiply(const Vector3f& a, const Vector3f& b) noexcept
{
	return Vector3f(a.x * b.x, a.y * b.y, a.z * b.z);
}

inline constexpr Vector2f math::Perpendicular(const Vector2f& vector) noexcept
{
	return Vector2f(-vector.y, vector.x);
}

inline constexpr Vector2f math::Project(const Vector2f& point, const Line2f& line)
{
	const Vector2f lineAB = line.m_PointB - line.m_PointA;
	const Vector2f lineAP = point - line.m_PointA;
	const float p = math::Dot(lineAP, lineAB) / lineAB.LengthSqr();
	return line.m_PointA + lineAB * p;
}

inline constexpr Vector2f math::Project(const Vector2f& point, const Ray2f& ray)
{
	const Vector2f lineAP = point - ray.m_Position;
	const float p = math::Dot(lineAP, ray.m_Direction);

	if (p < 0.f)
		return ray.m_Position;
	return ray.m_Position + ray.m_Direction * p;
}

inline constexpr Vector2f math::Project(const Vector2f& point, const Segment2f& segment)
{
	const Vector2f lineAB = segment.m_PointB - segment.m_PointA;
	const Vector2f lineAP = point - segment.m_PointA;
	const float p = math::Dot(lineAP, lineAB) / lineAB.LengthSqr();

	if (p < 0.f)
	{
		return segment.m_PointA;
	}
	else if (p > 1.f)
	{
		return segment.m_PointB;
	}
	else
	{
		return segment.m_PointA + lineAB * p;
	}
}

// https://gamedev.stackexchange.com/questions/72528/how-can-i-project-a-3d-point-onto-a-3d-line
inline constexpr Vector3f math::Project(const Vector3f& point, const Line3f& line)
{
	const Vector3f lineAB = line.m_PointB - line.m_PointA;
	const Vector3f lineAP = point - line.m_PointA;
	const float p = math::Dot(lineAP, lineAB) / lineAB.LengthSqr();
	return line.m_PointA + lineAB * p;
}

inline constexpr Vector3f math::Project(const Vector3f& point, const Ray3f& ray)
{
	const Vector3f lineAP = point - ray.m_Position;
	const float p = math::Dot(lineAP, ray.m_Direction);

	if (p < 0.f)
		return ray.m_Position;
	return ray.m_Position + ray.m_Direction * p;
}

// https://gamedev.stackexchange.com/questions/72528/how-can-i-project-a-3d-point-onto-a-3d-line
inline constexpr Vector3f math::Project(const Vector3f& point, const Segment3f& segment)
{
	const Vector3f lineAB = segment.m_PointB - segment.m_PointA;
	const Vector3f lineAP = point - segment.m_PointA;
	const float p = math::Dot(lineAP, lineAB) / lineAB.LengthSqr();

	if (p < 0.f)
	{
		return segment.m_PointA;
	}
	else if (p > 1.f)
	{
		return segment.m_PointB;
	}
	else
	{
		return segment.m_PointA + lineAB * p;
	}
}

inline constexpr Vector3f math::ProjectXY(const Vector3f& point, const Line3f& line)
{
	const Vector3f lineAB = line.m_PointB - line.m_PointA;
	const Vector2f lineXY = lineAB.XY();
	const Vector2f lineAP = point.XY() - line.m_PointA.XY();
	const float p = math::Dot(lineAP, lineXY) / lineAB.LengthSqr();
	return line.m_PointA + lineAB * p;
}

inline constexpr Vector3f math::ProjectXY(const Vector3f& point, const Ray3f& ray)
{
	const Vector2f lineAP = point.XY() - ray.m_Position.XY();
	const float p = math::Dot(lineAP, ray.m_Direction.XY());
	return ray.m_Position + ray.m_Direction * p;
}

inline constexpr Vector3f math::ProjectXY(const Vector3f& point, const Segment3f& segment)
{
	const Vector3f lineAB = segment.m_PointB - segment.m_PointA;
	const Vector2f lineXY = lineAB.XY();
	const Vector2f lineAP = point.XY() - segment.m_PointA.XY();
	const float p = math::Dot(lineAP, lineXY) / lineAB.LengthSqr();

	if (p < 0.f)
	{
		return segment.m_PointA;
	}
	else if (p > 1.f)
	{
		return segment.m_PointB;
	}
	else
	{
		return segment.m_PointA + lineAB * p;
	}
}

inline constexpr Vector3f math::ProjectXZ(const Vector3f& point, const Line3f& line)
{
	const Vector3f lineAB = line.m_PointB - line.m_PointA;
	const Vector2f lineXZ = lineAB.XZ();
	const Vector2f lineAP = point.XZ() - line.m_PointA.XZ();
	const float p = math::Dot(lineAP, lineXZ) / lineAB.LengthSqr();
	return line.m_PointA + lineAB * p;
}

inline constexpr Vector3f math::ProjectXZ(const Vector3f& point, const Ray3f& ray)
{
	const Vector2f lineAP = point.XZ() - ray.m_Position.XZ();
	const float p = math::Dot(lineAP, ray.m_Direction.XZ());
	return ray.m_Position + ray.m_Direction * p;
}

inline constexpr Vector3f math::ProjectXZ(const Vector3f& point, const Segment3f& segment)
{
	const Vector3f lineAB = segment.m_PointB - segment.m_PointA;
	const Vector2f lineXZ = lineAB.XZ();
	const Vector2f lineAP = point.XZ() - segment.m_PointA.XZ();
	const float p = math::Dot(lineAP, lineXZ) / lineAB.LengthSqr();

	if (p < 0.f)
	{
		return segment.m_PointA;
	}
	else if (p > 1.f)
	{
		return segment.m_PointB;
	}
	else
	{
		return segment.m_PointA + lineAB * p;
	}
}

inline constexpr Vector2f math::Reflect(const Vector2f& vector, const Vector2f& normal) noexcept
{
	// -2 * (V dot N) * N + V
	const float dot2 = -2.0f * math::Dot(vector, normal);
	return math::Multiply(Vector2f(dot2), normal) + vector;
}

inline constexpr Vector2i math::ToGridPos(const Vector2f& worldPos, const float cellSize /*= 1.f*/)
{
	return Vector2i(
		math::Floor<int32>(worldPos.x / cellSize),
		math::Floor<int32>(worldPos.y / cellSize));
}

inline constexpr Vector3i math::ToGridPos(const Vector3f& worldPos, const float cellSize /*= 1.f*/)
{
	return Vector3i(
		math::Floor<int32>(worldPos.x / cellSize),
		math::Floor<int32>(worldPos.y / cellSize),
		math::Floor<int32>(worldPos.z / cellSize));
}

inline constexpr Vector2f math::ToWorldPos(const Vector2i& gridPos, const float cellSize /*= 1.f*/)
{
	const float half = cellSize * 0.5f;
	return Vector2f(
		gridPos.x * cellSize + half,
		gridPos.y * cellSize + half);
}

inline constexpr Vector3f math::ToWorldPos(const Vector3i& gridPos, const float cellSize /*= 1.f*/)
{
	const float half = cellSize * 0.5f;
	return Vector3f(
		gridPos.x * cellSize + half,
		gridPos.y * cellSize + half,
		gridPos.z * cellSize + half);
}