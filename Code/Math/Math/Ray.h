#pragma once

#include "Math/Vector.h"

class Segment2f;
class Segment3f;

/// \brief A ray in 2D space denoted by a origin and direction using floating-point coordinates.
class Ray2f
{
public:
	constexpr Ray2f() noexcept : m_Direction(), m_Position() { }
	constexpr Ray2f(const Vector2f& position, const Vector2f& direction) noexcept : m_Position(position), m_Direction(direction) { }

	[[nodiscard]] static Ray2f FromPoints(const Vector2f& pointA, const Vector2f& pointB);
	[[nodiscard]] static Ray2f FromSegment(const Segment2f& segment);

public:
	Vector2f m_Position;
	Vector2f m_Direction;
};


/// \brief A ray in 3D space denoted by a origin and direction using floating-point coordinates.
class Ray3f
{
public:
	constexpr Ray3f() noexcept : m_Direction(), m_Position() { }
	constexpr Ray3f(const Vector3f& position, const Vector3f& direction) noexcept : m_Position(position), m_Direction(direction) { }

	[[nodiscard]] static Ray3f FromPoints(const Vector3f& pointA, const Vector3f& pointB);
	[[nodiscard]] static Ray3f FromSegment(const Segment3f& segment);

public:
	Vector3f m_Position;
	Vector3f m_Direction;
};
