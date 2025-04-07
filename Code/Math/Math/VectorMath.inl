#pragma once

#ifdef VECTOR_ASSERTS_ENABLED
#include <cassert>
#define VECTOR_ASSERT(condition, ...) assert(condition)
#else
#define VECTOR_ASSERT(...)
#endif

inline constexpr Vector2f math::Abs(const Vector2f& value) noexcept
{
	return Vector2f(std::abs(value.x), std::abs(value.y));
}

inline constexpr Vector3f math::Abs(const Vector3f& value) noexcept
{
	return Vector3f(std::abs(value.x), std::abs(value.y), std::abs(value.z));
}

inline float math::Angle(const Vector2f& a, const Vector2f& b) noexcept
{
	const float dot = math::Dot(a, b);
	return std::acos(math::Clamp(dot, -1.f, 1.f));
}

inline float math::Angle(const Vector3f& a, const Vector3f& b) noexcept
{
	const float dot = math::Dot(a, b);
	return std::acos(math::Clamp(dot, -1.f, 1.f));
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

inline constexpr Vector2i math::Divide(const Vector2i& a, const Vector2i& b) noexcept
{
	return Vector2i(a.x / b.x, a.y / b.y);
}

inline constexpr Vector3f math::Divide(const Vector3f& a, const Vector3f& b) noexcept
{
	return Vector3f(a.x / b.x, a.y / b.y, a.z / b.z);
}

inline constexpr Vector3i math::Divide(const Vector3i& a, const Vector3i& b) noexcept
{
	return Vector3i(a.x / b.x, a.y / b.y, a.z / b.z);
}

inline constexpr float math::Dot(const Vector2f& a, const Vector2f& b) noexcept
{
	return a.x * b.x + a.y * b.y;
}

inline constexpr float math::Dot(const Vector3f& a, const Vector3f& b) noexcept
{
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

inline constexpr bool math::IsNearly(const Vector2f& a, const Vector2f& b, const float threshold /*= 0.001f*/) noexcept
{
	return (a - b).LengthSqr() < threshold;
}

inline constexpr bool math::IsNearly(const Vector3f& a, const Vector3f& b, const float threshold /*= 0.001f*/) noexcept
{
	return (a - b).LengthSqr() < threshold;
}

inline constexpr bool math::IsNearly(const Vector4f& a, const Vector4f& b, const float threshold /*= 0.001f*/) noexcept
{
	return (a - b).LengthSqr() < threshold;
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

inline constexpr Vector2i math::Multiply(const Vector2i& a, const Vector2i& b) noexcept
{
	return Vector2i(a.x * b.x, a.y * b.y);
}

inline constexpr Vector3f math::Multiply(const Vector3f& a, const Vector3f& b) noexcept
{
	return Vector3f(a.x * b.x, a.y * b.y, a.z * b.z);
}

inline constexpr Vector3i math::Multiply(const Vector3i& a, const Vector3i& b) noexcept
{
	return Vector3i(a.x * b.x, a.y * b.y, a.z * b.z);
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

inline constexpr float math::Sign(const Vector2f& a, const Vector2f& b, const Vector2f& c) noexcept
{
	return (a.x - c.x) * (b.y - c.y) - (b.x - c.x) * (a.y - c.y);
}

inline constexpr int32 math::To1Dimension(const Vector2i& value, const int32 stride)
{
	return value.x + value.y * stride;
}

inline constexpr Vector2i math::To2Dimension(const int32 value, const int32 stride)
{
	return Vector2i(value % stride, value / stride);
}

inline constexpr Vector2i math::ToGridPos(const Vector2f& worldPos, const float cellSize /*= 1.f*/)
{
	return Vector2i(
		math::Floor<int32>(worldPos.x / cellSize),
		math::Floor<int32>(worldPos.y / cellSize));
}

inline constexpr Vector2i math::ToGridPos(const Vector2f& worldPos, const Vector2f& cellSize /*= Vector2f::One*/)
{
	return Vector2i(
		math::Floor<int32>(worldPos.x / cellSize.x),
		math::Floor<int32>(worldPos.y / cellSize.y));
}

inline constexpr Vector3i math::ToGridPos(const Vector3f& worldPos, const float cellSize /*= 1.f*/)
{
	return Vector3i(
		math::Floor<int32>(worldPos.x / cellSize),
		math::Floor<int32>(worldPos.y / cellSize),
		math::Floor<int32>(worldPos.z / cellSize));
}

inline constexpr Vector3i math::ToGridPos(const Vector3f& worldPos, const Vector3f& cellSize /*= Vector3f::One*/)
{
	return Vector3i(
		math::Floor<int32>(worldPos.x / cellSize.x),
		math::Floor<int32>(worldPos.y / cellSize.y),
		math::Floor<int32>(worldPos.z / cellSize.z));
}

inline float math::ToHeading(const Vector2f& direction)
{
	const float radians = std::atan2(direction.y, direction.x);
	return math::ToCircular(radians);
}

inline constexpr Vector2f math::ToWorldPos(const Vector2i& gridPos, const float cellSize /*= 1.f*/)
{
	const float half = cellSize * 0.5f;
	return Vector2f(
		gridPos.x * cellSize + half,
		gridPos.y * cellSize + half);
}

inline constexpr Vector2f math::ToWorldPos(const Vector2i& gridPos, const Vector2f& cellSize /*= Vector2f::One*/)
{
	return Vector2f(
		gridPos.x * cellSize.x + cellSize.x * 0.5f,
		gridPos.y * cellSize.y + cellSize.y * 0.5f);
}

inline constexpr Vector3f math::ToWorldPos(const Vector3i& gridPos, const float cellSize /*= 1.f*/)
{
	const float half = cellSize * 0.5f;
	return Vector3f(
		gridPos.x * cellSize + half,
		gridPos.y * cellSize + half,
		gridPos.z * cellSize + half);
}

inline constexpr Vector3f math::ToWorldPos(const Vector3i& gridPos, const Vector3f& cellSize /*= Vector3f::One*/)
{
	return Vector3f(
		gridPos.x * cellSize.x + cellSize.x * 0.5f,
		gridPos.y * cellSize.y + cellSize.y * 0.5f,
		gridPos.z * cellSize.z + cellSize.z * 0.5f);
}