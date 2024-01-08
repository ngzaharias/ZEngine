#pragma once

inline float math::Angle(const Vector2f& a, const Vector2f& b)
{
	return std::acos(math::Dot(a, b));
}

inline float math::Angle(const Vector3f& a, const Vector3f& b)
{
	return std::acos(math::Dot(a, b));
}

inline float math::AngleSigned(const Vector2f& a, const Vector2f& b)
{
	const float angle = math::Angle(a, b);
	if (a.x * b.y - a.y * b.x >= 0.f)
		return angle;
	return -angle;
}

inline float math::AngleSigned(const Vector3f& a, const Vector3f& b, const Vector3f& normal)
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
	return Vector2f(vector.y, -vector.x);
}

// https://gamedev.stackexchange.com/questions/72528/how-can-i-project-a-3d-point-onto-a-3d-line
inline constexpr Vector3f math::ProjectOntoLine(const Vector3f& point, const Vector3f& lineA, const Vector3f& lineB)
{
	const Vector3f lineAB = lineB - lineA;
	const Vector3f lineAP = point - lineA;
	const float p = math::Dot(lineAP, lineAB) / lineAB.LengthSqr();
	return lineA + lineAB * p;
}

inline constexpr Vector3f math::ProjectOntoLineClamped(const Vector3f& point, const Vector3f& lineA, const Vector3f& lineB)
{
	if (lineA == lineB)
		return lineA;

	const Vector3f lineAB = lineB - lineA;
	const Vector3f lineAP = point - lineA;
	const float p = math::Dot(lineAP, lineAB) / lineAB.LengthSqr();

	if (p < 0.f)
	{
		return lineA;
	}
	else if (p > 1.f)
	{
		return lineB;
	}
	else
	{
		return lineA + lineAB * p;
	}
}

inline constexpr Vector3f math::ProjectOntoLineXY(const Vector3f& point, const Vector3f& lineA, const Vector3f& lineB)
{
	const Vector3f lineAB = (lineB - lineA);
	const Vector2f lineXZ = lineAB.XY();
	const Vector2f lineAP = point.XY() - lineA.XY();
	const float p = math::Dot(lineAP, lineXZ) / lineAB.LengthSqr();
	return lineA + lineAB * p;
}

inline constexpr Vector3f math::ProjectOntoLineXZ(const Vector3f& point, const Vector3f& lineA, const Vector3f& lineB)
{
	const Vector3f lineAB = (lineB - lineA);
	const Vector2f lineXZ = lineAB.XZ();
	const Vector2f lineAP = point.XZ() - lineA.XZ();
	const float p = math::Dot(lineAP, lineXZ) / lineAB.LengthSqr();
	return lineA + lineAB * p;
}

inline constexpr Vector3f math::ProjectOntoLineXYClamped(const Vector3f& point, const Vector3f& lineA, const Vector3f& lineB)
{
	const Vector3f lineAB = (lineB - lineA);
	const Vector2f lineXZ = lineAB.XY();
	const Vector2f lineAP = point.XY() - lineA.XY();
	const float p = math::Dot(lineAP, lineXZ) / lineAB.LengthSqr();

	if (p < 0.f)
	{
		return lineA;
	}
	else if (p > 1.f)
	{
		return lineB;
	}
	else
	{
		return lineA + lineAB * p;
	}
}

inline constexpr Vector3f math::ProjectOntoLineXZClamped(const Vector3f& point, const Vector3f& lineA, const Vector3f& lineB)
{
	const Vector3f lineAB = (lineB - lineA);
	const Vector2f lineXZ = lineAB.XZ();
	const Vector2f lineAP = point.XZ() - lineA.XZ();
	const float p = math::Dot(lineAP, lineXZ) / lineAB.LengthSqr();

	if (p < 0.f)
	{
		return lineA;
	}
	else if (p > 1.f)
	{
		return lineB;
	}
	else
	{
		return lineA + lineAB * p;
	}
}

inline constexpr Vector3f math::ProjectOntoRay(const Vector3f& point, const Vector3f& rayOrigin, const Vector3f& rayDir)
{
	const Vector3f lineAP = point - rayOrigin;
	const float p = math::Dot(lineAP, rayDir);
	return rayOrigin + rayDir * p;
}

inline constexpr Vector3f math::ProjectOntoRayXY(const Vector3f& point, const Vector3f& rayOrigin, const Vector3f& rayDir)
{
	const Vector2f lineAP = point.XY() - rayOrigin.XY();
	const float p = math::Dot(lineAP, rayDir.XY());
	return rayOrigin + rayDir * p;
}

inline constexpr Vector3f math::ProjectOntoRayXZ(const Vector3f& point, const Vector3f& rayOrigin, const Vector3f& rayDir)
{
	const Vector2f lineAP = point.XZ() - rayOrigin.XZ();
	const float p = math::Dot(lineAP, rayDir.XZ());
	return rayOrigin + rayDir * p;
}

inline constexpr Vector2f math::Reflect(const Vector2f& vector, const Vector2f& normal) noexcept
{
	// -2 * (V dot N) * N + V
	const float dot2 = -2.0f * math::Dot(vector, normal);
	return math::Multiply(Vector2f(dot2), normal) + vector;
}