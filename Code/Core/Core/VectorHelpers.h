#pragma once

#include <cmath>

#include <Core/Vector.h>

/// \brief Check if the distance between two vectors is less than the epsilon.
inline bool IsNearly(const Vector2f& a, const Vector2f& b, const float epsilon = 0.001f) noexcept
{
	return (a - b).LengthSqr() < epsilon;
}

/// \brief Check if the distance between two vectors is less than the epsilon.
inline bool IsNearly(const Vector3f& a, const Vector3f& b, const float epsilon = 0.001f) noexcept
{
	return (a - b).LengthSqr() < (epsilon * epsilon);
}

namespace math
{
	/// \brief Get the angle between two vectors in radians.
	inline float Angle(const Vector2f& a, const Vector2f& b)
	{
		return std::acos(Vector2f::Dot(a, b));
	}

	/// \brief Get the angle between two vectors in radians.
	inline float Angle(const Vector3f& a, const Vector3f& b)
	{
		return std::acos(Vector3f::Dot(a, b));
	}

	/// \brief Convert a world position to a grid position.
	/// Input in the range of >= 0.f and < 1.f will all map to the same value.
	inline constexpr Vector2i ToGridPos(const Vector2f& worldPos, const float cellSize = 1.f)
	{
		return Vector2i(
			static_cast<int32>(std::floorf(worldPos.x / cellSize)),
			static_cast<int32>(std::floorf(worldPos.y / cellSize)));
	}

	/// \brief Convert a world position to a grid position.
	/// Input in the range of >= 0.f and < 1.f will all map to the same value.
	inline constexpr Vector3i ToGridPos(const Vector3f& worldPos, const float cellSize = 1.f)
	{
		return Vector3i(
			static_cast<int32>(std::floorf(worldPos.x / cellSize)),
			static_cast<int32>(std::floorf(worldPos.y / cellSize)),
			static_cast<int32>(std::floorf(worldPos.z / cellSize)));
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

	// https://gamedev.stackexchange.com/questions/72528/how-can-i-project-a-3d-point-onto-a-3d-line
	/// \brief 
	inline constexpr Vector3f ProjectOntoLine(const Vector3f& point, const Vector3f& lineA, const Vector3f& lineB);

	/// \brief 
	inline constexpr Vector3f ProjectOntoLineClamped(const Vector3f& point, const Vector3f& lineA, const Vector3f& lineB);

	/// \brief 
	inline constexpr Vector3f ProjectOntoLineXZ(const Vector3f& point, const Vector3f& lineA, const Vector3f& lineB);

	/// \brief 
	inline constexpr Vector3f ProjectOntoLineXZClamped(const Vector3f& point, const Vector3f& lineA, const Vector3f& lineB);

	/// \brief 
	inline constexpr Vector3f ProjectOntoRay(const Vector3f& point, const Vector3f& rayOrigin, const Vector3f& rayDir);

	/// \brief 
	inline constexpr Vector3f ProjectOntoRayXZ(const Vector3f& point, const Vector3f& rayOrigin, const Vector3f& rayDir);
}

#include "VectorHelpers.inl"