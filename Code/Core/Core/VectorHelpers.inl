#pragma once

// https://gamedev.stackexchange.com/questions/72528/how-can-i-project-a-3d-point-onto-a-3d-line
/// \brief 
constexpr Vector3f math::ProjectOntoLine(const Vector3f& point, const Vector3f& lineA, const Vector3f& lineB)
{
	const Vector3f lineAB = lineB - lineA;
	const Vector3f lineAP = point - lineA;
	const float p = Vector3f::Dot(lineAP, lineAB) / lineAB.LengthSqr();
	return lineA + lineAB * p;
}

/// \brief 
constexpr Vector3f math::ProjectOntoLineClamped(const Vector3f& point, const Vector3f& lineA, const Vector3f& lineB)
{
	if (lineA == lineB)
		return lineA;

	const Vector3f lineAB = lineB - lineA;
	const Vector3f lineAP = point - lineA;
	const float p = Vector3f::Dot(lineAP, lineAB) / lineAB.LengthSqr();

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

/// \brief 
constexpr Vector3f math::ProjectOntoLineXZ(const Vector3f& point, const Vector3f& lineA, const Vector3f& lineB)
{
	const Vector3f lineAB = (lineB - lineA);
	const Vector2f lineXZ = lineAB.XZ();
	const Vector2f lineAP = point.XZ() - lineA.XZ();
	const float p = Vector2f::Dot(lineAP, lineXZ) / lineAB.LengthSqr();
	return lineA + lineAB * p;
}

/// \brief 
constexpr Vector3f math::ProjectOntoLineXZClamped(const Vector3f& point, const Vector3f& lineA, const Vector3f& lineB)
{
	const Vector3f lineAB = (lineB - lineA);
	const Vector2f lineXZ = lineAB.XZ();
	const Vector2f lineAP = point.XZ() - lineA.XZ();
	const float p = Vector2f::Dot(lineAP, lineXZ) / lineAB.LengthSqr();

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

/// \brief 
constexpr Vector3f math::ProjectOntoRay(const Vector3f& point, const Vector3f& rayOrigin, const Vector3f& rayDir)
{
	const Vector3f lineAP = point - rayOrigin;
	const float p = Vector3f::Dot(lineAP, rayDir);
	return rayOrigin + rayDir * p;
}

/// \brief 
constexpr Vector3f math::ProjectOntoRayXZ(const Vector3f& point, const Vector3f& rayOrigin, const Vector3f& rayDir)
{
	const Vector2f lineAP = point.XZ() - rayOrigin.XZ();
	const float p = Vector2f::Dot(lineAP, rayDir.XZ());
	return rayOrigin + rayDir * p;
}