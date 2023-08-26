#include "Core/OBB.h"

#include "Core/AABB.h"
#include "Core/Matrix.h"
#include "Core/Quaternion.h"
#include "Core/Rotator.h"

OBB OBB::FromExtents(const Rotator& rotate, const Vector3f& extents) noexcept
{
	const Matrix3x3 matrix = Matrix3x3::FromRotate(rotate);

	OBB obb;
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

OBB OBB::FromExtents(const Vector3f& centre, const Rotator& rotate, const Vector3f& extents) noexcept
{
	const Matrix3x3 matrix = Matrix3x3::FromRotate(rotate);

	OBB obb;
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
