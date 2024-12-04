#include "Math/OBB.h"

#include "Math/AABB.h"
#include "Math/Matrix.h"
#include "Math/Quaternion.h"
#include "Math/Rotator.h"

OBB2f OBB2f::FromExtents(const Rotator& rotate, const Vector2f& extents) noexcept
{
	const Matrix3x3 matrix = Matrix3x3::FromRotate(rotate);

	OBB2f obb;
	obb.m_Points[0] = Vector2f(-extents.x, -extents.y) * matrix;
	obb.m_Points[1] = Vector2f(-extents.x, -extents.y) * matrix;
	obb.m_Points[2] = Vector2f(+extents.x, -extents.y) * matrix;
	obb.m_Points[3] = Vector2f(+extents.x, -extents.y) * matrix;
	return obb;
}

OBB2f OBB2f::FromExtents(const Vector2f& centre, const Rotator& rotate, const Vector2f& extents) noexcept
{
	const Matrix3x3 matrix = Matrix3x3::FromRotate(rotate);

	OBB2f obb;
	obb.m_Points[0] = centre + Vector2f(-extents.x, -extents.y) * matrix;
	obb.m_Points[1] = centre + Vector2f(-extents.x, -extents.y) * matrix;
	obb.m_Points[2] = centre + Vector2f(+extents.x, -extents.y) * matrix;
	obb.m_Points[3] = centre + Vector2f(+extents.x, -extents.y) * matrix;
	return obb;
}

OBB3f OBB3f::FromExtents(const Rotator& rotate, const Vector3f& extents) noexcept
{
	const Matrix3x3 matrix = Matrix3x3::FromRotate(rotate);

	OBB3f obb;
	obb.m_Points[0] = Vector3f(-extents.x, -extents.y, -extents.z) * matrix;
	obb.m_Points[1] = Vector3f(-extents.x, -extents.y, +extents.z) * matrix;
	obb.m_Points[2] = Vector3f(+extents.x, -extents.y, +extents.z) * matrix;
	obb.m_Points[3] = Vector3f(+extents.x, -extents.y, -extents.z) * matrix;
	obb.m_Points[4] = Vector3f(-extents.x, +extents.y, -extents.z) * matrix;
	obb.m_Points[5] = Vector3f(-extents.x, +extents.y, +extents.z) * matrix;
	obb.m_Points[6] = Vector3f(+extents.x, +extents.y, +extents.z) * matrix;
	obb.m_Points[7] = Vector3f(+extents.x, +extents.y, -extents.z) * matrix;
	return obb;
}

OBB3f OBB3f::FromExtents(const Vector3f& centre, const Rotator& rotate, const Vector3f& extents) noexcept
{
	const Matrix3x3 matrix = Matrix3x3::FromRotate(rotate);

	OBB3f obb;
	obb.m_Points[0] = centre + Vector3f(-extents.x, -extents.y, -extents.z) * matrix;
	obb.m_Points[1] = centre + Vector3f(-extents.x, -extents.y, +extents.z) * matrix;
	obb.m_Points[2] = centre + Vector3f(+extents.x, -extents.y, +extents.z) * matrix;
	obb.m_Points[3] = centre + Vector3f(+extents.x, -extents.y, -extents.z) * matrix;
	obb.m_Points[4] = centre + Vector3f(-extents.x, +extents.y, -extents.z) * matrix;
	obb.m_Points[5] = centre + Vector3f(-extents.x, +extents.y, +extents.z) * matrix;
	obb.m_Points[6] = centre + Vector3f(+extents.x, +extents.y, +extents.z) * matrix;
	obb.m_Points[7] = centre + Vector3f(+extents.x, +extents.y, -extents.z) * matrix;
	return obb;
}
