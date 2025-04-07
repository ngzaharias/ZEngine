#pragma once

#include <cmath>

#include "Math/Line.h"
#include "Math/Math.h"
#include "Math/Ray.h"
#include "Math/Segment.h"
#include "Math/Vector.h"

namespace math
{
	/// \brief Returns a vector made up of the largest components of the two vectors.
	inline constexpr Vector2f Abs(const Vector2f& value) noexcept;
	/// \brief Returns a vector made up of the largest components of the two vectors.
	inline constexpr Vector3f Abs(const Vector3f& value) noexcept;

	/// \brief Calculates the unsigned angle between two vectors in radians.
	inline float Angle(const Vector2f& a, const Vector2f& b) noexcept;
	/// \brief Calculates the unsigned angle between two vectors in radians.
	inline float Angle(const Vector3f& a, const Vector3f& b) noexcept;

	/// \brief Calculates the signed angle from direction a to direction b in radians.
	inline float AngleSigned(const Vector2f& a, const Vector2f& b) noexcept;
	/// \brief Calculates the signed angle from direction a to direction b in radians.
	inline float AngleSigned(const Vector3f& a, const Vector3f& b, const Vector3f& normal) noexcept;

	/// \brief Returns the vector with each component clamped between min and max so that they doesn't exceed either.
	inline constexpr Vector2f Clamp(const Vector2f& value, const Vector2f& min, const Vector2f& max) noexcept;
	/// \brief Returns the vector with each component clamped between min and max so that they doesn't exceed either.
	inline constexpr Vector3f Clamp(const Vector3f& value, const Vector3f& min, const Vector3f& max) noexcept;

	/// \brief Returns the vector with its length clamped to max.
	/// If the length of the vector is 0 and value is less than 0 then it makes it a NaN vector.
	inline Vector2f ClampLength(const Vector2f& value, const float max) noexcept;
	/// \brief Returns the vector with its length clamped to max.
	/// If the length of the vector is 0 and value is less than 0 then it makes it a NaN vector.
	inline Vector3f ClampLength(const Vector3f& value, const float max) noexcept;

	/// \brief Cross Product of two vectors.
	inline constexpr float Cross(const Vector2f& a, const Vector2f& b) noexcept;
	/// \brief Cross Product of two vectors.
	inline constexpr Vector3f Cross(const Vector3f& a, const Vector3f& b) noexcept;

	/// \brief Calculates the distance between two vectors.
	inline float Distance(const Vector2f& a, const Vector2f& b) noexcept;
	/// \brief Calculates the distance between two vectors.
	inline float Distance(const Vector3f& a, const Vector3f& b) noexcept;
	/// \brief Calculates the distance between two vectors on the XY plane.
	inline float DistanceXY(const Vector3f& a, const Vector3f& b) noexcept;
	/// \brief Calculates the distance between two vectors on the XZ plane.
	inline float DistanceXZ(const Vector3f& a, const Vector3f& b) noexcept;

	/// \brief Calculates the squared distance between two vectors.
	inline constexpr float DistanceSqr(const Vector2f& a, const Vector2f& b) noexcept;
	/// \brief Calculates the squared distance between two vectors.
	inline constexpr float DistanceSqr(const Vector3f& a, const Vector3f& b) noexcept;
	/// \brief Calculates the squared distance between two vectors on the XY plane.
	inline constexpr float DistanceXYSqr(const Vector3f& a, const Vector3f& b) noexcept;
	/// \brief Calculates the squared distance between two vectors on the XZ plane.
	inline constexpr float DistanceXZSqr(const Vector3f& a, const Vector3f& b) noexcept;

	/// \brief Divides the two vectors component-wise.
	inline constexpr Vector2f Divide(const Vector2f& a, const Vector2f& b) noexcept;
	inline constexpr Vector2i Divide(const Vector2i& a, const Vector2i& b) noexcept;
	/// \brief Divides the two vectors component-wise.
	inline constexpr Vector3f Divide(const Vector3f& a, const Vector3f& b) noexcept;
	inline constexpr Vector3i Divide(const Vector3i& a, const Vector3i& b) noexcept;

	/// \brief Dot Product of two vectors.
	inline constexpr float Dot(const Vector2f& a, const Vector2f& b) noexcept;
	/// \brief Dot Product of two vectors.
	inline constexpr float Dot(const Vector3f& a, const Vector3f& b) noexcept;

	/// \brief Check if the distance between two vectors is less than the threshold.
	constexpr bool IsNearly(const Vector2f& a, const Vector2f& b, const float threshold = 0.001f) noexcept;
	/// \brief Check if the distance between two vectors is less than the threshold.
	constexpr bool IsNearly(const Vector3f& a, const Vector3f& b, const float threshold = 0.001f) noexcept;
	/// \brief Check if the distance between two vectors is less than the threshold.
	constexpr bool IsNearly(const Vector4f& a, const Vector4f& b, const float threshold = 0.001f) noexcept;

	/// \brief Returns a vector made up of the largest components of the two vectors.
	inline constexpr Vector2f Max(const Vector2f& a, const Vector2f& b) noexcept;
	/// \brief Returns a vector made up of the largest components of the two vectors.
	inline constexpr Vector3f Max(const Vector3f& a, const Vector3f& b) noexcept;

	/// \brief Returns a vector made up of the smallest components of the two vectors.
	inline constexpr Vector2f Min(const Vector2f& a, const Vector2f& b) noexcept;
	/// \brief Returns a vector made up of the smallest components of the two vectors.
	inline constexpr Vector3f Min(const Vector3f& a, const Vector3f& b) noexcept;

	/// \brief Multiplies the two vectors component-wise.
	inline constexpr Vector2f Multiply(const Vector2f& a, const Vector2f& b) noexcept;
	inline constexpr Vector2i Multiply(const Vector2i& a, const Vector2i& b) noexcept;
	/// \brief Multiplies the two vectors component-wise.
	inline constexpr Vector3f Multiply(const Vector3f& a, const Vector3f& b) noexcept;
	inline constexpr Vector3i Multiply(const Vector3i& a, const Vector3i& b) noexcept;

	/// \brief Returns the vector perpendicular (clockwise) to the original vector.
	inline constexpr Vector2f Perpendicular(const Vector2f& vector) noexcept;

	/// \brief Projects the point onto the nearest position along a line.
	inline constexpr Vector2f Project(const Vector2f& point, const Line2f& line);
	/// \brief Projects the point onto the nearest position along a ray.
	inline constexpr Vector2f Project(const Vector2f& point, const Ray2f& ray);
	/// \brief Projects the point onto the nearest position along a segment.
	inline constexpr Vector2f Project(const Vector2f& point, const Segment2f& segment);

	/// \brief Projects the point onto the nearest position along a line.
	inline constexpr Vector3f Project(const Vector3f& point, const Line3f& line);
	/// \brief Projects the point onto the nearest position along a ray.
	inline constexpr Vector3f Project(const Vector3f& point, const Ray3f& ray);
	/// \brief Projects the point onto the nearest position along a segment.
	inline constexpr Vector3f Project(const Vector3f& point, const Segment3f& segment);

	/// \brief Projects the point onto the nearest position along a line on the XY plane.
	inline constexpr Vector3f ProjectXY(const Vector3f& point, const Line3f& line);
	/// \brief Projects the point onto the nearest position along a ray on the XY plane.
	inline constexpr Vector3f ProjectXY(const Vector3f& point, const Ray3f& ray);
	/// \brief Projects the point onto the nearest position along a segment on the XY plane.
	inline constexpr Vector3f ProjectXY(const Vector3f& point, const Segment3f& segment);

	/// \brief Projects the point onto the nearest position along a line on the XZ plane.
	inline constexpr Vector3f ProjectXZ(const Vector3f& point, const Line3f& line);
	/// \brief Projects the point onto the nearest position along a ray on the XZ plane.
	inline constexpr Vector3f ProjectXZ(const Vector3f& point, const Ray3f& ray);
	/// \brief Projects the point onto the nearest position along a segment on the XZ plane.
	inline constexpr Vector3f ProjectXZ(const Vector3f& point, const Segment3f& segment);

	/// \brief Reflects the vector off the plane defined by a normal.
	inline constexpr Vector2f Reflect(const Vector2f& vector, const Vector2f& normal) noexcept;

	/// \brief Calculate the sign of three points of a triangle.
	inline constexpr float Sign(const Vector2f& a, const Vector2f& b, const Vector2f& c) noexcept;

	inline constexpr int32 To1Dimension(const Vector2i& value, const int32 stride);
	inline constexpr Vector2i To2Dimension(const int32 value, const int32 stride);

	/// \brief Convert a world position to a grid position.
	/// Input in the range of >= 0.f and < 1.f will all map to the same value.
	inline constexpr Vector2i ToGridPos(const Vector2f& worldPos, const float cellSize = 1.f);
	/// \brief Convert a world position to a grid position.
	/// Input in the range of >= 0.f and < 1.f will all map to the same value.
	inline constexpr Vector2i ToGridPos(const Vector2f& worldPos, const Vector2f& cellSize = Vector2f::One);
	/// \brief Convert a world position to a grid position.
	/// Input in the range of >= 0.f and < 1.f will all map to the same value.
	inline constexpr Vector3i ToGridPos(const Vector3f& worldPos, const float cellSize = 1.f);

	/// \brief Convert a world position to a grid position.
	/// Input in the range of >= 0.f and < 1.f will all map to the same value.
	inline constexpr Vector3i ToGridPos(const Vector3f& worldPos, const Vector3f& cellSize = Vector3f::One);

	/// \brief Convert a direction to a circular radian.
	inline float ToHeading(const Vector2f& direction);

	/// \brief Convert a grid position to a world position.
	/// The world position will be centered on the grid position.
	inline constexpr Vector2f ToWorldPos(const Vector2i& gridPos, const float cellSize = 1.f);
	/// \brief Convert a grid position to a world position.
	/// The world position will be centered on the grid position.
	inline constexpr Vector2f ToWorldPos(const Vector2i& gridPos, const Vector2f& cellSize = Vector2f::One);
	/// \brief Convert a grid position to a world position.
	/// The world position will be centered on the grid position.
	inline constexpr Vector3f ToWorldPos(const Vector3i& gridPos, const float cellSize = 1.f);
	/// \brief Convert a grid position to a world position.
	/// The world position will be centered on the grid position.
	inline constexpr Vector3f ToWorldPos(const Vector3i& gridPos, const Vector3f& cellSize = Vector3f::One);
}

#include "VectorMath.inl"