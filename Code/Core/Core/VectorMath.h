#pragma once

#include <cmath>

#include <Core/Line.h>
#include <Core/Math.h>
#include <Core/Ray.h>
#include <Core/Vector.h>

namespace math
{
	/// \brief Calculates the unsigned angle between two vectors in radians.
	inline float Angle(const Vector2f& a, const Vector2f& b) noexcept;
	/// \brief Calculates the unsigned angle between two vectors in radians.
	inline float Angle(const Vector3f& a, const Vector3f& b) noexcept;

	/// \brief Calculates the signed angle from vector a to vector b in radians.
	inline float AngleSigned(const Vector2f& a, const Vector2f& b) noexcept;
	/// \brief Calculates the signed angle from vector a to vector b in radians.
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
	/// \brief Divides the two vectors component-wise.
	inline constexpr Vector3f Divide(const Vector3f& a, const Vector3f& b) noexcept;

	/// \brief Dot Product of two vectors.
	inline constexpr float Dot(const Vector2f& a, const Vector2f& b) noexcept;
	/// \brief Dot Product of two vectors.
	inline constexpr float Dot(const Vector3f& a, const Vector3f& b) noexcept;

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
	/// \brief Multiplies the two vectors component-wise.
	inline constexpr Vector3f Multiply(const Vector3f& a, const Vector3f& b) noexcept;

	/// \brief Returns the vector perpendicular (clockwise) to the original vector.
	inline constexpr Vector2f Perpendicular(const Vector2f& vector) noexcept;

	/// \brief Projects the point onto the nearest position along a line.
	inline constexpr Vector3f Project(const Vector3f& point, const Line& line);
	/// \brief Projects the point onto the nearest position along a ray.
	inline constexpr Vector3f Project(const Vector3f& point, const Ray& ray);
	/// \brief Projects the point onto the nearest position along a line on the XY plane.
	inline constexpr Vector3f ProjectXY(const Vector3f& point, const Line& line);
	/// \brief Projects the point onto the nearest position along a ray on the XY plane.
	inline constexpr Vector3f ProjectXY(const Vector3f& point, const Ray& ray);
	/// \brief Projects the point onto the nearest position along a line on the XZ plane.
	inline constexpr Vector3f ProjectXZ(const Vector3f& point, const Line& line);
	/// \brief Projects the point onto the nearest position along a ray on the XZ plane.
	inline constexpr Vector3f ProjectXZ(const Vector3f& point, const Ray& ray);

	/// \brief Reflects the vector off the plane defined by a normal.
	inline constexpr Vector2f Reflect(const Vector2f& vector, const Vector2f& normal) noexcept;

	/// \brief Convert a world position to a grid position.
	/// Input in the range of >= 0.f and < 1.f will all map to the same value.
	inline constexpr Vector2i ToGridPos(const Vector2f& worldPos, const float cellSize = 1.f);
	/// \brief Convert a world position to a grid position.
	/// Input in the range of >= 0.f and < 1.f will all map to the same value.
	inline constexpr Vector3i ToGridPos(const Vector3f& worldPos, const float cellSize = 1.f);

	/// \brief Convert a grid position to a world position.
	/// The world position will be centered on the grid position.
	inline constexpr Vector2f ToWorldPos(const Vector2i& gridPos, const float cellSize = 1.f);
	/// \brief Convert a grid position to a world position.
	/// The world position will be centered on the grid position.
	inline constexpr Vector3f ToWorldPos(const Vector3i& gridPos, const float cellSize = 1.f);
}

#include "VectorMath.inl"