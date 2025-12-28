#include "EnginePCH.h"
#include "Engine/LinesComponent.h"

#include "Engine/CameraHelpers.h"
#include "Math/AABB.h"
#include "Math/Circle.h"
#include "Math/Matrix.h"
#include "Math/OBB.h"
#include "Math/Sphere.h"

namespace
{
	constexpr Vector3f s_Icosphere[] = {
			Vector3f(0.0f, 1.000f, 0.0f),
			Vector3f(0.353f, 0.866f, -0.353f),
			Vector3f(0.353f, 0.866f, -0.353f),
			Vector3f(0.612f, 0.500f, -0.612f),
			Vector3f(0.612f, 0.500f, -0.612f),
			Vector3f(0.707f, 0.0f, -0.707f),
			Vector3f(0.707f, 0.0f, -0.707f),
			Vector3f(0.612f, -0.499f, -0.612f),
			Vector3f(0.612f, -0.499f, -0.612f),
			Vector3f(0.353f, -0.866f, -0.353f),
			Vector3f(0.0f, 0.866f, -0.499f),
			Vector3f(0.353f, 0.866f, -0.353f),
			Vector3f(0.0f, -0.499f, -0.866f),
			Vector3f(0.612f, -0.499f, -0.612f),
			Vector3f(0.499f, 0.866f, 0.0f),
			Vector3f(0.866f, 0.500f, 0.0f),
			Vector3f(0.866f, 0.500f, 0.0f),
			Vector3f(0.999f, 0.0f, 0.0f),
			Vector3f(0.999f, 0.0f, 0.0f),
			Vector3f(0.866f, -0.499f, 0.0f),
			Vector3f(0.866f, -0.499f, 0.0f),
			Vector3f(0.499f, -0.866f, 0.0f),
			Vector3f(0.499f, -0.866f, 0.0f),
			Vector3f(0.353f, -0.866f, -0.353f),
			Vector3f(0.612f, -0.499f, -0.612f),
			Vector3f(0.866f, -0.499f, 0.0f),
			Vector3f(0.999f, 0.0f, 0.0f),
			Vector3f(0.707f, 0.0f, -0.707f),
			Vector3f(0.612f, 0.500f, -0.612f),
			Vector3f(0.866f, 0.500f, 0.0f),
			Vector3f(0.499f, 0.866f, 0.0f),
			Vector3f(0.353f, 0.866f, -0.353f),
			Vector3f(0.353f, 0.866f, 0.353f),
			Vector3f(0.612f, 0.5f, 0.612f),
			Vector3f(0.612f, 0.5f, 0.612f),
			Vector3f(0.707f, 0.0f, 0.707f),
			Vector3f(0.707f, 0.0f, 0.707f),
			Vector3f(0.612f, -0.5f, 0.612f),
			Vector3f(0.612f, -0.5f, 0.612f),
			Vector3f(0.353f, -0.866f, 0.353f),
			Vector3f(0.353f, -0.866f, 0.353f),
			Vector3f(0.0f, -0.999f, 0.0f),
			Vector3f(0.353f, -0.866f, 0.353f),
			Vector3f(0.499f, -0.866f, 0.0f),
			Vector3f(0.866f, -0.499f, 0.0f),
			Vector3f(0.612f, -0.5f, 0.612f),
			Vector3f(0.707f, 0.0f, 0.707f),
			Vector3f(0.999f, 0.0f, 0.0f),
			Vector3f(0.866f, 0.500f, 0.0f),
			Vector3f(0.612f, 0.5f, 0.612f),
			Vector3f(0.353f, 0.866f, 0.353f),
			Vector3f(0.499f, 0.866f, 0.0f),
			Vector3f(0.0f, 0.866f, 0.499f),
			Vector3f(0.0f, 0.499f, 0.866f),
			Vector3f(0.0f, 0.499f, 0.866f),
			Vector3f(0.0f, 0.0f, 0.999f),
			Vector3f(0.0f, 0.0f, 0.999f),
			Vector3f(0.0f, -0.5f, 0.866f),
			Vector3f(0.0f, -0.5f, 0.866f),
			Vector3f(0.0f, -0.866f, 0.499f),
			Vector3f(0.0f, 0.0f, 0.999f),
			Vector3f(0.707f, 0.0f, 0.707f),
			Vector3f(0.612f, 0.5f, 0.612f),
			Vector3f(0.0f, 0.499f, 0.866f),
			Vector3f(0.0f, 0.866f, 0.499f),
			Vector3f(0.353f, 0.866f, 0.353f),
			Vector3f(0.353f, -0.866f, 0.353f),
			Vector3f(0.0f, -0.866f, 0.499f),
			Vector3f(0.0f, -0.5f, 0.866f),
			Vector3f(0.612f, -0.5f, 0.612f),
			Vector3f(-0.353f, 0.866f, 0.353f),
			Vector3f(-0.612f, 0.5f, 0.612f),
			Vector3f(-0.612f, 0.5f, 0.612f),
			Vector3f(-0.707f, 0.0f, 0.707f),
			Vector3f(-0.707f, 0.0f, 0.707f),
			Vector3f(-0.612f, -0.5f, 0.612f),
			Vector3f(-0.612f, -0.5f, 0.612f),
			Vector3f(-0.353f, -0.866f, 0.353f),
			Vector3f(-0.353f, 0.866f, 0.353f),
			Vector3f(0.0f, 0.866f, 0.499f),
			Vector3f(0.0f, -0.866f, 0.499f),
			Vector3f(-0.353f, -0.866f, 0.353f),
			Vector3f(-0.612f, -0.5f, 0.612f),
			Vector3f(0.0f, -0.5f, 0.866f),
			Vector3f(0.0f, 0.0f, 0.999f),
			Vector3f(-0.707f, 0.0f, 0.707f),
			Vector3f(-0.612f, 0.5f, 0.612f),
			Vector3f(0.0f, 0.499f, 0.866f),
			Vector3f(-0.499f, 0.866f, 0.0f),
			Vector3f(-0.866f, 0.500f, 0.0f),
			Vector3f(-0.866f, 0.500f, 0.0f),
			Vector3f(-0.999f, 0.0f, 0.0f),
			Vector3f(-0.999f, 0.0f, 0.0f),
			Vector3f(-0.866f, -0.499f, 0.0f),
			Vector3f(-0.866f, -0.499f, 0.0f),
			Vector3f(-0.499f, -0.866f, 0.0f),
			Vector3f(-0.353f, -0.866f, 0.353f),
			Vector3f(-0.499f, -0.866f, 0.0f),
			Vector3f(-0.866f, -0.499f, 0.0f),
			Vector3f(-0.612f, -0.5f, 0.612f),
			Vector3f(-0.707f, 0.0f, 0.707f),
			Vector3f(-0.999f, 0.0f, 0.0f),
			Vector3f(-0.866f, 0.500f, 0.0f),
			Vector3f(-0.612f, 0.5f, 0.612f),
			Vector3f(-0.353f, 0.866f, 0.353f),
			Vector3f(-0.499f, 0.866f, 0.0f),
			Vector3f(-0.353f, 0.866f, -0.353f),
			Vector3f(-0.612f, 0.500f, -0.612f),
			Vector3f(-0.612f, 0.500f, -0.612f),
			Vector3f(-0.707f, 0.0f, -0.707f),
			Vector3f(-0.707f, 0.0f, -0.707f),
			Vector3f(-0.612f, -0.499f, -0.612f),
			Vector3f(-0.612f, -0.499f, -0.612f),
			Vector3f(-0.353f, -0.866f, -0.353f),
			Vector3f(-0.499f, -0.866f, 0.0f),
			Vector3f(-0.353f, -0.866f, -0.353f),
			Vector3f(-0.612f, -0.499f, -0.612f),
			Vector3f(-0.866f, -0.499f, 0.0f),
			Vector3f(-0.999f, 0.0f, 0.0f),
			Vector3f(-0.707f, 0.0f, -0.707f),
			Vector3f(-0.612f, 0.500f, -0.612f),
			Vector3f(-0.866f, 0.500f, 0.0f),
			Vector3f(-0.499f, 0.866f, 0.0f),
			Vector3f(-0.353f, 0.866f, -0.353f),
			Vector3f(0.0f, 0.500f, -0.866f),
			Vector3f(0.0f, 0.0f, -0.999f),
			Vector3f(-0.353f, -0.866f, -0.353f),
			Vector3f(0.0f, -0.866f, -0.499f),
			Vector3f(-0.707f, 0.0f, -0.707f),
			Vector3f(0.0f, 0.0f, -0.999f),
			Vector3f(0.0f, 0.500f, -0.866f),
			Vector3f(-0.612f, 0.500f, -0.612f),
			Vector3f(0.0f, 1.000f, 0.0f),
			Vector3f(0.0f, 0.866f, -0.499f),
			Vector3f(0.0f, 0.866f, -0.499f),
			Vector3f(0.0f, 0.500f, -0.866f),
			Vector3f(0.0f, 0.0f, -0.999f),
			Vector3f(0.0f, -0.499f, -0.866f),
			Vector3f(0.0f, -0.499f, -0.866f),
			Vector3f(0.0f, -0.866f, -0.499f),
			Vector3f(0.0f, -0.866f, -0.499f),
			Vector3f(0.0f, -0.999f, 0.0f),
			Vector3f(0.353f, -0.866f, -0.353f),
			Vector3f(0.0f, -0.999f, 0.0f),
			Vector3f(0.612f, 0.500f, -0.612f),
			Vector3f(0.0f, 0.500f, -0.866f),
			Vector3f(0.353f, -0.866f, -0.353f),
			Vector3f(0.0f, -0.866f, -0.499f),
			Vector3f(0.707f, 0.0f, -0.707f),
			Vector3f(0.0f, 0.0f, -0.999f),
			Vector3f(0.0f, 1.000f, 0.0f),
			Vector3f(0.499f, 0.866f, 0.0f),
			Vector3f(0.499f, -0.866f, 0.0f),
			Vector3f(0.0f, -0.999f, 0.0f),
			Vector3f(0.0f, 1.000f, 0.0f),
			Vector3f(0.353f, 0.866f, 0.353f),
			Vector3f(0.0f, 1.000f, 0.0f),
			Vector3f(0.0f, 0.866f, 0.499f),
			Vector3f(0.0f, -0.866f, 0.499f),
			Vector3f(0.0f, -0.999f, 0.0f),
			Vector3f(0.0f, 1.000f, 0.0f),
			Vector3f(-0.353f, 0.866f, 0.353f),
			Vector3f(-0.353f, -0.866f, 0.353f),
			Vector3f(0.0f, -0.999f, 0.0f),
			Vector3f(0.0f, 1.000f, 0.0f),
			Vector3f(-0.499f, 0.866f, 0.0f),
			Vector3f(-0.499f, -0.866f, 0.0f),
			Vector3f(0.0f, -0.999f, 0.0f),
			Vector3f(0.0f, 1.000f, 0.0f),
			Vector3f(-0.353f, 0.866f, -0.353f),
			Vector3f(-0.353f, -0.866f, -0.353f),
			Vector3f(0.0f, -0.999f, 0.0f),
			Vector3f(0.0f, -0.499f, -0.866f),
			Vector3f(-0.612f, -0.499f, -0.612f),
			Vector3f(-0.353f, 0.866f, -0.353f),
			Vector3f(0.0f, 0.866f, -0.499f) };
}

void eng::LinesSingleton::AddAABB(const AABB3f& extents, const Colour& colour)
{
	AddAABB(Vector3f::Zero, extents, colour);
}

void eng::LinesSingleton::AddAABB(const Vector3f& translate, const float extents, const Colour& colour)
{
	Vector3f corners[8];
	corners[0] = translate + Vector3f(-extents, -extents, -extents);
	corners[1] = translate + Vector3f(-extents, -extents, +extents);
	corners[2] = translate + Vector3f(+extents, -extents, +extents);
	corners[3] = translate + Vector3f(+extents, -extents, -extents);

	corners[4] = translate + Vector3f(-extents, +extents, -extents);
	corners[5] = translate + Vector3f(-extents, +extents, +extents);
	corners[6] = translate + Vector3f(+extents, +extents, +extents);
	corners[7] = translate + Vector3f(+extents, +extents, -extents);

	AddLine(corners[0], corners[1], colour);
	AddLine(corners[1], corners[2], colour);
	AddLine(corners[2], corners[3], colour);
	AddLine(corners[3], corners[0], colour);

	AddLine(corners[4], corners[5], colour);
	AddLine(corners[5], corners[6], colour);
	AddLine(corners[6], corners[7], colour);
	AddLine(corners[7], corners[4], colour);

	AddLine(corners[0], corners[4], colour);
	AddLine(corners[1], corners[5], colour);
	AddLine(corners[2], corners[6], colour);
	AddLine(corners[3], corners[7], colour);
}

void eng::LinesSingleton::AddAABB(const Vector3f& translate, const Vector3f& extents, const Colour& colour)
{
	Vector3f corners[8];
	corners[0] = translate + Vector3f(-extents.x, -extents.y, -extents.z);
	corners[1] = translate + Vector3f(-extents.x, -extents.y, +extents.z);
	corners[2] = translate + Vector3f(+extents.x, -extents.y, +extents.z);
	corners[3] = translate + Vector3f(+extents.x, -extents.y, -extents.z);

	corners[4] = translate + Vector3f(-extents.x, +extents.y, -extents.z);
	corners[5] = translate + Vector3f(-extents.x, +extents.y, +extents.z);
	corners[6] = translate + Vector3f(+extents.x, +extents.y, +extents.z);
	corners[7] = translate + Vector3f(+extents.x, +extents.y, -extents.z);

	AddLine(corners[0], corners[1], colour);
	AddLine(corners[1], corners[2], colour);
	AddLine(corners[2], corners[3], colour);
	AddLine(corners[3], corners[0], colour);

	AddLine(corners[4], corners[5], colour);
	AddLine(corners[5], corners[6], colour);
	AddLine(corners[6], corners[7], colour);
	AddLine(corners[7], corners[4], colour);

	AddLine(corners[0], corners[4], colour);
	AddLine(corners[1], corners[5], colour);
	AddLine(corners[2], corners[6], colour);
	AddLine(corners[3], corners[7], colour);
}

void eng::LinesSingleton::AddAABB(const Vector3f& translate, const AABB3f& extents, const Colour& colour)
{
	Vector3f corners[8];
	corners[0] = translate + Vector3f(extents.m_Min.x, extents.m_Min.y, extents.m_Min.z);
	corners[1] = translate + Vector3f(extents.m_Min.x, extents.m_Min.y, extents.m_Max.z);
	corners[2] = translate + Vector3f(extents.m_Max.x, extents.m_Min.y, extents.m_Max.z);
	corners[3] = translate + Vector3f(extents.m_Max.x, extents.m_Min.y, extents.m_Min.z);

	corners[4] = translate + Vector3f(extents.m_Min.x, extents.m_Max.y, extents.m_Min.z);
	corners[5] = translate + Vector3f(extents.m_Min.x, extents.m_Max.y, extents.m_Max.z);
	corners[6] = translate + Vector3f(extents.m_Max.x, extents.m_Max.y, extents.m_Max.z);
	corners[7] = translate + Vector3f(extents.m_Max.x, extents.m_Max.y, extents.m_Min.z);

	AddLine(corners[0], corners[1], colour);
	AddLine(corners[1], corners[2], colour);
	AddLine(corners[2], corners[3], colour);
	AddLine(corners[3], corners[0], colour);

	AddLine(corners[4], corners[5], colour);
	AddLine(corners[5], corners[6], colour);
	AddLine(corners[6], corners[7], colour);
	AddLine(corners[7], corners[4], colour);

	AddLine(corners[0], corners[4], colour);
	AddLine(corners[1], corners[5], colour);
	AddLine(corners[2], corners[6], colour);
	AddLine(corners[3], corners[7], colour);
}

void eng::LinesSingleton::AddCircle(const Vector3f& translate, const Circle2f& circle, const float steps, const Colour& colour)
{
	const float step = math::ToRadians(360.f / steps);

	Vector3f pointA;
	Vector3f pointB = translate + Vector3f::AxisX * circle.m_Radius;
	for (float angle = step; angle <= PI_TWO; angle += step)
	{
		pointA = pointB;
		pointB.x = translate.x + std::cos(angle) * circle.m_Radius;
		pointB.y = translate.y + std::sin(angle) * circle.m_Radius;
		AddLine(pointA, pointB, colour);
	}

	pointA = translate + Vector3f::AxisX * circle.m_Radius;
	AddLine(pointA, pointB, colour);
}

void eng::LinesSingleton::AddFrustrum(const Vector3f& translate, const Rotator& rotate, const eng::Projection& projection, const Vector2u& size, const Colour& colour)
{
	std::visit([&](auto projection) { AddFrustrum(translate, rotate, projection, size, colour); }, projection);
}

void eng::LinesSingleton::AddFrustrum(const Vector3f& translate, const Rotator& rotate, const eng::Orthographic& projection, const Vector2u& size, const Colour& colour)
{
	// #todo: pass in width & height
	const float aspect = (float)size.x / (float)size.y;
	const float widthH = projection.m_Size * aspect * 0.5f;
	const float heightH = projection.m_Size * 0.5f;

	Vector3f corners[8];
	corners[0] = Vector3f(-widthH, +heightH, projection.m_ClippingNear);
	corners[1] = Vector3f(+widthH, +heightH, projection.m_ClippingNear);
	corners[2] = Vector3f(-widthH, -heightH, projection.m_ClippingNear);
	corners[3] = Vector3f(+widthH, -heightH, projection.m_ClippingNear);

	corners[4] = Vector3f(-widthH, +heightH, projection.m_ClippingFar);
	corners[5] = Vector3f(+widthH, +heightH, projection.m_ClippingFar);
	corners[6] = Vector3f(-widthH, -heightH, projection.m_ClippingFar);
	corners[7] = Vector3f(+widthH, -heightH, projection.m_ClippingFar);

	Matrix4x4 transform = Matrix4x4::FromRotate(rotate);
	transform.SetTranslate(translate);

	for (uint32 i = 0; i < 8; ++i)
		corners[i] = corners[i] * transform;

	AddLine(corners[0], corners[1], colour);
	AddLine(corners[1], corners[3], colour);
	AddLine(corners[3], corners[2], colour);
	AddLine(corners[2], corners[0], colour);

	AddLine(corners[4], corners[5], colour);
	AddLine(corners[5], corners[7], colour);
	AddLine(corners[7], corners[6], colour);
	AddLine(corners[6], corners[4], colour);

	AddLine(corners[0], corners[4], colour);
	AddLine(corners[1], corners[5], colour);
	AddLine(corners[2], corners[6], colour);
	AddLine(corners[3], corners[7], colour);
}

void eng::LinesSingleton::AddFrustrum(const Vector3f& translate, const Rotator& rotate, const eng::Perspective& projection, const Vector2u& size, const Colour& colour)
{
	// #todo: pass in width & height
	const float ratio = (float)size.x / (float)size.y;
	const float fovy = math::ToRadians(projection.m_FieldOfView);

	const float nearHeight = 2.f * tan(fovy * 0.5f) * projection.m_ClippingNear;
	const float nearWidth = nearHeight * ratio;

	const float farHeight = 2.f * tan(fovy * 0.5f) * projection.m_ClippingFar;
	const float farWidth = farHeight * ratio;

	Vector3f corners[8];
	corners[0] = Vector3f(-nearWidth * 0.5f, -nearHeight * 0.5f, projection.m_ClippingNear);
	corners[1] = Vector3f(+nearWidth * 0.5f, -nearHeight * 0.5f, projection.m_ClippingNear);
	corners[2] = Vector3f(-nearWidth * 0.5f, +nearHeight * 0.5f, projection.m_ClippingNear);
	corners[3] = Vector3f(+nearWidth * 0.5f, +nearHeight * 0.5f, projection.m_ClippingNear);

	corners[4] = Vector3f(-farWidth * 0.5f, -farHeight * 0.5f, projection.m_ClippingFar);
	corners[5] = Vector3f(+farWidth * 0.5f, -farHeight * 0.5f, projection.m_ClippingFar);
	corners[6] = Vector3f(-farWidth * 0.5f, +farHeight * 0.5f, projection.m_ClippingFar);
	corners[7] = Vector3f(+farWidth * 0.5f, +farHeight * 0.5f, projection.m_ClippingFar);

	Matrix4x4 transform = Matrix4x4::FromRotate(rotate);
	transform.SetTranslate(translate);

	for (uint32 i = 0; i < 8; ++i)
		corners[i] = corners[i] * transform;

	AddLine(corners[0], corners[1], colour);
	AddLine(corners[1], corners[3], colour);
	AddLine(corners[3], corners[2], colour);
	AddLine(corners[2], corners[0], colour);

	AddLine(corners[4], corners[5], colour);
	AddLine(corners[5], corners[7], colour);
	AddLine(corners[7], corners[6], colour);
	AddLine(corners[6], corners[4], colour);

	AddLine(corners[0], corners[4], colour);
	AddLine(corners[1], corners[5], colour);
	AddLine(corners[2], corners[6], colour);
	AddLine(corners[3], corners[7], colour);
}

void eng::LinesSingleton::AddFrustrum(const Vector3f& translate, const Rotator& rotate, const eng::camera::UserInterface& projection, const Vector2u& size, const Colour& colour)
{
}

void eng::LinesSingleton::AddHexagon(const Vector3f& translate, const float radius, const Colour& colour)
{
	constexpr Vector3f s_DirectionE = Vector2f(+1.f, 0.f).X0Y();
	constexpr Vector3f s_DirectionSE = Vector2f(+0.5f, -0.866025f).X0Y();
	constexpr Vector3f s_DirectionSW = Vector2f(-0.5f, -0.866025f).X0Y();
	constexpr Vector3f s_DirectionW = Vector2f(-1.f, 0.f).X0Y();
	constexpr Vector3f s_DirectionNW = Vector2f(-0.5f, +0.866025f).X0Y();
	constexpr Vector3f s_DirectionNE = Vector2f(+0.5f, +0.866025f).X0Y();

	AddLine(translate + s_DirectionE * radius, translate + s_DirectionSE * radius, colour);
	AddLine(translate + s_DirectionSE * radius, translate + s_DirectionSW * radius, colour);
	AddLine(translate + s_DirectionSW * radius, translate + s_DirectionW * radius, colour);
	AddLine(translate + s_DirectionW * radius, translate + s_DirectionNW * radius, colour);
	AddLine(translate + s_DirectionNW * radius, translate + s_DirectionNE * radius, colour);
	AddLine(translate + s_DirectionNE * radius, translate + s_DirectionE * radius, colour);
}

void eng::LinesSingleton::AddIcosphere(const Vector3f& translate, const Sphere3f& sphere, const Colour& colour)
{
	const Matrix4x4 transform = Matrix4x4::FromTransform(
		translate + sphere.m_Position,
		Quaternion::Identity,
		sphere.m_Radius);

	const uint64 count = sizeof(s_Icosphere) / sizeof(Vector3f);
	for (uint64 i = 0, j = 1; j < count; ++i, ++j)
	{
		const Vector3f pointA = s_Icosphere[i] * transform;
		const Vector3f pointB = s_Icosphere[j] * transform;
		AddLine(pointA, pointB, colour);
	}
}

void eng::LinesSingleton::AddLine(const Vector3f& pointA, const Vector3f& pointB, const Colour& colour)
{
	m_Vertices.Append({ pointA, colour });
	m_Vertices.Append({ pointB, colour });
}

void eng::LinesSingleton::AddOBB(const Vector3f& translate, const OBB3f& obb, const Colour& colour)
{
	AddLine(translate + obb.m_Points[0], translate + obb.m_Points[1], colour);
	AddLine(translate + obb.m_Points[1], translate + obb.m_Points[2], colour);
	AddLine(translate + obb.m_Points[2], translate + obb.m_Points[3], colour);
	AddLine(translate + obb.m_Points[3], translate + obb.m_Points[0], colour);

	AddLine(translate + obb.m_Points[4], translate + obb.m_Points[5], colour);
	AddLine(translate + obb.m_Points[5], translate + obb.m_Points[6], colour);
	AddLine(translate + obb.m_Points[6], translate + obb.m_Points[7], colour);
	AddLine(translate + obb.m_Points[7], translate + obb.m_Points[4], colour);

	AddLine(translate + obb.m_Points[0], translate + obb.m_Points[4], colour);
	AddLine(translate + obb.m_Points[1], translate + obb.m_Points[5], colour);
	AddLine(translate + obb.m_Points[2], translate + obb.m_Points[6], colour);
	AddLine(translate + obb.m_Points[3], translate + obb.m_Points[7], colour);
}

void eng::LinesSingleton::AddSphere(const Sphere3f& sphere, const Colour& colour)
{
	AddSphere(Vector3f::Zero, sphere, colour);
}

void eng::LinesSingleton::AddSphere(const Vector3f& translate, const float radius, const Colour& colour)
{
	Vector3f corners[8];
	corners[0] = translate + Vector3f(-radius, 0.f, 0.f);
	corners[1] = translate + Vector3f(+radius, 0.f, 0.f);
	corners[2] = translate + Vector3f(0.f, -radius, 0.f);
	corners[3] = translate + Vector3f(0.f, +radius, 0.f);
	corners[4] = translate + Vector3f(0.f, 0.f, -radius);
	corners[5] = translate + Vector3f(0.f, 0.f, +radius);

	AddLine(corners[0], corners[2], colour);
	AddLine(corners[2], corners[1], colour);
	AddLine(corners[1], corners[3], colour);
	AddLine(corners[3], corners[0], colour);

	AddLine(corners[0], corners[4], colour);
	AddLine(corners[4], corners[1], colour);
	AddLine(corners[1], corners[5], colour);
	AddLine(corners[5], corners[0], colour);

	AddLine(corners[2], corners[4], colour);
	AddLine(corners[4], corners[3], colour);
	AddLine(corners[3], corners[5], colour);
	AddLine(corners[5], corners[2], colour);
}

void eng::LinesSingleton::AddSphere(const Vector3f& translate, const Sphere3f& sphere, const Colour& colour)
{
	AddSphere(translate + sphere.m_Position, sphere.m_Radius, colour);
}