#pragma once

#include <cmath>

#include <Core/Math.h>
#include <Core/Vector.h>

namespace math
{
	/// \brief Returns the unsigned angle between two vectors in radians.
	inline float Angle(const Vector2f& a, const Vector2f& b);
	/// \brief Returns the unsigned angle between two vectors in radians.
	inline float Angle(const Vector3f& a, const Vector3f& b);

	/// \brief Returns the signed angle from vector a to vector b in radians.
	inline float AngleSigned(const Vector2f& a, const Vector2f& b);
	/// \brief Returns the signed angle from vector a to vector b in radians.
	inline float AngleSigned(const Vector3f& a, const Vector3f& b, const Vector3f& normal);

	/// \brief Returns a vector with each component clamped between min and max so that they doesn't exceed either.
	inline constexpr Vector2f Clamp(const Vector2f& value, const Vector2f& min, const Vector2f& max) noexcept;
	/// \brief Returns a vector with each component clamped between min and max so that they doesn't exceed either.
	inline constexpr Vector3f Clamp(const Vector3f& value, const Vector3f& min, const Vector3f& max) noexcept;

	/// \brief
	inline constexpr Vector3f Cross(const Vector3f& a, const Vector3f& b) noexcept;

	/// \brief Returns the distance between two vectors.
	inline float Distance(const Vector2f& a, const Vector2f& b) noexcept;
	/// \brief Returns the distance between two vectors.
	inline float Distance(const Vector3f& a, const Vector3f& b) noexcept;
	/// \brief
	inline float DistanceXY(const Vector3f& a, const Vector3f& b) noexcept;
	/// \brief
	inline float DistanceXZ(const Vector3f& a, const Vector3f& b) noexcept;

	/// \brief Returns the squared distance between two vectors.
	inline constexpr float DistanceSqr(const Vector2f& a, const Vector2f& b) noexcept;
	/// \brief Returns the squared distance between two vectors.
	inline constexpr float DistanceSqr(const Vector3f& a, const Vector3f& b) noexcept;
	/// \brief
	inline constexpr float DistanceXYSqr(const Vector3f& a, const Vector3f& b) noexcept;
	/// \brief
	inline constexpr float DistanceXZSqr(const Vector3f& a, const Vector3f& b) noexcept;

	/// \brief Divides the two vectors component-wise.
	inline constexpr Vector2f Divide(const Vector2f& a, const Vector2f& b) noexcept;
	/// \brief Divides the two vectors component-wise.
	inline constexpr Vector3f Divide(const Vector3f& a, const Vector3f& b) noexcept;

	/// \brief Returns the dot product of two vectors.
	inline constexpr float Dot(const Vector2f& a, const Vector2f& b) noexcept;
	/// \brief Returns the dot product of two vectors.
	inline constexpr float Dot(const Vector3f& a, const Vector3f& b) noexcept;

	/// \brief Returns a vector made up of the bigger components from each vector.
	inline constexpr Vector2f Max(const Vector2f& a, const Vector2f& b) noexcept;
	/// \brief Returns a vector made up of the bigger components from each vector.
	inline constexpr Vector3f Max(const Vector3f& a, const Vector3f& b) noexcept;

	/// \brief Returns a vector made up of the smaller components from each vector.
	inline constexpr Vector2f Min(const Vector2f& a, const Vector2f& b) noexcept;
	/// \brief Returns a vector made up of the smaller components from each vector.
	inline constexpr Vector3f Min(const Vector3f& a, const Vector3f& b) noexcept;

	/// \brief Multiplies the two vectors component-wise.
	inline constexpr Vector2f Multiply(const Vector2f& a, const Vector2f& b) noexcept;
	/// \brief Multiplies the two vectors component-wise.
	inline constexpr Vector3f Multiply(const Vector3f& a, const Vector3f& b) noexcept;

	/// \brief Returns a vector perpendicular (clockwise) to the original vector.
	inline constexpr Vector2f Perpendicular(const Vector2f& vector) noexcept;

	/// \brief 
	inline constexpr Vector3f ProjectOntoLine(const Vector3f& point, const Vector3f& lineA, const Vector3f& lineB);
	/// \brief 
	inline constexpr Vector3f ProjectOntoLineClamped(const Vector3f& point, const Vector3f& lineA, const Vector3f& lineB);

	/// \brief 
	inline constexpr Vector3f ProjectOntoLineXY(const Vector3f& point, const Vector3f& lineA, const Vector3f& lineB);
	/// \brief 
	inline constexpr Vector3f ProjectOntoLineXZ(const Vector3f& point, const Vector3f& lineA, const Vector3f& lineB);
	/// \brief 
	inline constexpr Vector3f ProjectOntoLineXYClamped(const Vector3f& point, const Vector3f& lineA, const Vector3f& lineB);
	/// \brief 
	inline constexpr Vector3f ProjectOntoLineXZClamped(const Vector3f& point, const Vector3f& lineA, const Vector3f& lineB);

	/// \brief 
	inline constexpr Vector3f ProjectOntoRay(const Vector3f& point, const Vector3f& rayOrigin, const Vector3f& rayDir);
	/// \brief 
	inline constexpr Vector3f ProjectOntoRayXY(const Vector3f& point, const Vector3f& rayOrigin, const Vector3f& rayDir);
	/// \brief 
	inline constexpr Vector3f ProjectOntoRayXZ(const Vector3f& point, const Vector3f& rayOrigin, const Vector3f& rayDir);

	/// \brief Returns a vector reflected off a normal.
	inline constexpr Vector2f Reflect(const Vector2f& vector, const Vector2f& normal) noexcept;

	/// \brief Convert a world position to a grid position.
	/// Input in the range of >= 0.f and < 1.f will all map to the same value.
	inline constexpr Vector2i ToGridPos(const Vector2f& worldPos, const float cellSize = 1.f)
	{
		return Vector2i(
			math::Floor<int32>(worldPos.x / cellSize),
			math::Floor<int32>(worldPos.y / cellSize));
	}

	/// \brief Convert a world position to a grid position.
	/// Input in the range of >= 0.f and < 1.f will all map to the same value.
	inline constexpr Vector3i ToGridPos(const Vector3f& worldPos, const float cellSize = 1.f)
	{
		return Vector3i(
			math::Floor<int32>(worldPos.x / cellSize),
			math::Floor<int32>(worldPos.y / cellSize),
			math::Floor<int32>(worldPos.z / cellSize));
	}

	/// \brief Convert a grid position to a world position.
	/// The world position will be centered on the grid position.
	inline constexpr Vector2f ToWorldPos(const Vector2i& gridPos, const float cellSize = 1.f)
	{
		const float half = cellSize * 0.5f;
		return Vector2f(
			gridPos.x * cellSize + half,
			gridPos.y * cellSize + half);
	}

	/// \brief Convert a grid position to a world position.
	/// The world position will be centered on the grid position.
	inline constexpr Vector3f ToWorldPos(const Vector3i& gridPos, const float cellSize = 1.f)
	{
		const float half = cellSize * 0.5f;
		return Vector3f(
			gridPos.x * cellSize + half,
			gridPos.y * cellSize + half,
			gridPos.z * cellSize + half);
	}
}

#include "VectorMath.inl"