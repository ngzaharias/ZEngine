#include "EnginePCH.h"
#include "Engine/LinesComponent.h"

#include "Engine/CameraHelpers.h"
#include "Engine/Screen.h"
#include "Math/AABB.h"
#include "Math/Circle.h"
#include "Math/Matrix.h"
#include "Math/OBB.h"
#include "Math/Sphere.h"

void eng::LinesComponent::AddAABB(const Vector3f& translate, const AABB3f& extents, const Vector4f& colour)
{
	Vector3f corners[8];
	corners[0] = translate + Vector3f(extents.m_Min.x, extents.m_Min.y, extents.m_Min.z);
	corners[1] = translate + Vector3f(extents.m_Min.x, extents.m_Min.y, extents.m_Max.z);
	corners[2] = translate + Vector3f(extents.m_Max.x, extents.m_Min.y, extents.m_Max.z);
	corners[3] = translate + Vector3f(extents.m_Max.x, extents.m_Min.y, extents.m_Min.z);

	corners[4] = translate + Vector3f(extents.m_Min.x, extents.m_Max.y, extents.m_Min.z);
	corners[5] = translate + Vector3f(extents.m_Min.x, extents.m_Max.y, extents.m_Max.z);
	corners[6] = translate + Vector3f(extents.m_Max.x, extents.m_Max.y, extents.m_Max.z);
	corners[7] = translate + Vector3f(extents.m_Max.x, extents.m_Max.y, extents.m_Max.z);

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

void eng::LinesComponent::AddAABB(const Vector3f& translate, const float extents, const Vector4f& colour)
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

void eng::LinesComponent::AddCircle(const Vector3f& translate, const Circle2f& circle, const Vector4f& colour)
{
	Vector3f corners[8];
	corners[0] = translate + circle.m_Position.X0Y() + Vector3f(-circle.m_Radius, 0.f, 0.f);
	corners[1] = translate + circle.m_Position.X0Y() + Vector3f(+circle.m_Radius, 0.f, 0.f);
	corners[2] = translate + circle.m_Position.X0Y() + Vector3f(0.f, -circle.m_Radius, 0.f);
	corners[3] = translate + circle.m_Position.X0Y() + Vector3f(0.f, +circle.m_Radius, 0.f);
	corners[4] = translate + circle.m_Position.X0Y() + Vector3f(0.f, 0.f, -circle.m_Radius);
	corners[5] = translate + circle.m_Position.X0Y() + Vector3f(0.f, 0.f, +circle.m_Radius);

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

void eng::LinesComponent::AddFrustrum(const Vector3f& translate, const Rotator& rotate, const eng::camera::Projection& projection, const Vector4f& colour)
{
	std::visit([&](auto projection) { AddFrustrum(translate, rotate, projection, colour); }, projection);
}

void eng::LinesComponent::AddFrustrum(const Vector3f& translate, const Rotator& rotate, const eng::camera::Orthographic& projection, const Vector4f& colour)
{
	// #todo: pass in width & height
	const float aspect = Screen::width / Screen::height;
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

void eng::LinesComponent::AddFrustrum(const Vector3f& translate, const Rotator& rotate, const eng::camera::Perspective& projection, const Vector4f& colour)
{
	// #todo: pass in width & height
	const float ratio = Screen::width / Screen::height;
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

void eng::LinesComponent::AddLine(const Vector3f& pointA, const Vector3f& pointB, const Vector4f& colour)
{
	m_Vertices.Append({ pointA, colour });
	m_Vertices.Append({ pointB, colour });
}

void eng::LinesComponent::AddOBB(const Vector3f& translate, const OBB3f& obb, const Vector4f& colour)
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

void eng::LinesComponent::AddSphere(const Vector3f& translate, const Sphere3f& sphere, const Vector4f& colour)
{
	Vector3f corners[8];
	corners[0] = translate + sphere.m_Position + Vector3f(-sphere.m_Radius, 0.f, 0.f);
	corners[1] = translate + sphere.m_Position + Vector3f(+sphere.m_Radius, 0.f, 0.f);
	corners[2] = translate + sphere.m_Position + Vector3f(0.f, -sphere.m_Radius, 0.f);
	corners[3] = translate + sphere.m_Position + Vector3f(0.f, +sphere.m_Radius, 0.f);
	corners[4] = translate + sphere.m_Position + Vector3f(0.f, 0.f, -sphere.m_Radius);
	corners[5] = translate + sphere.m_Position + Vector3f(0.f, 0.f, +sphere.m_Radius);

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