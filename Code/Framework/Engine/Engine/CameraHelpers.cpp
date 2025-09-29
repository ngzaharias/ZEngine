#include "EnginePCH.h"
#include "Engine/CameraHelpers.h"

#include "Core/Assert.h"
#include "Engine/CameraComponent.h"
#include "Engine/TransformComponent.h"
#include "Engine/Window.h"
#include "Math/Math.h"
#include "Math/Matrix.h"

#include <algorithm>

// https://learnopengl.com/Getting-started/Transformations
// http://www.opengl-tutorial.org/beginners-tutorials/tutorial-3-matrices/#the-projection-matrix
// https://www.scratchapixel.com/lessons/3d-basic-rendering/perspective-and-orthographic-projection-matrix/opengl-perspective-projection-matrix
// http://learnwebgl.brown37.net/08_projections/projections_ortho.html
// http://learnwebgl.brown37.net/08_projections/projections_perspective.html

Matrix4x4 eng::camera::GetProjection(const Projection& value, const Vector2u& windowSize)
{
	Matrix4x4 projection = Matrix4x4::Identity;
	std::visit([&](auto data) { projection = GetProjection(data, windowSize); }, value);
	return projection;
}

Matrix4x4 eng::camera::GetProjection(const Cinematic& settings, const Vector2u& windowSize)
{
	Z_PANIC(false, "");
	return Matrix4x4::Identity;
}

Matrix4x4 eng::camera::GetProjection(const Orthographic& settings, const Vector2u& windowSize)
{
	const float aspect = static_cast<float>(windowSize.x) / static_cast<float>(windowSize.y);
	const float near = math::Max(settings.m_ClippingNear, 0.1f);
	const float far = math::Max(settings.m_ClippingFar, near + 0.1f);

#ifdef Z_OPENGL
	const float width = settings.m_Size * aspect;
	const float height = settings.m_Size;

	Matrix4x4 projection = Matrix4x4::Identity;

	// scale the viewing volume
	projection[0][0] = 2.f / width;
	projection[1][1] = 2.f / height;

	// scale the z buffer
	projection[2][2] = 1.f / (far - near);
	projection[3][2] = near / (near - far);

	return projection;
#endif
}

Matrix4x4 eng::camera::GetProjection(const Perspective& settings, const Vector2u& windowSize)
{
	const float aspect = static_cast<float>(windowSize.x) / static_cast<float>(windowSize.y);
	const float near = math::Max(settings.m_ClippingNear, 0.1f);
	const float far = math::Max(settings.m_ClippingFar, near + 0.1f);

#if defined Z_OPENGL
	const bool isHorizontal = settings.m_FoVAxis == EAxis::Horizontal;
	const float fov = math::ToRadians(settings.m_FieldOfView);
	const float t = 1.f / std::tanf(fov * 0.5f);
	const float xScale = isHorizontal ? t / aspect : t;
	const float yScale = isHorizontal ? t : t * aspect;

	Matrix4x4 projection = Matrix4x4::Identity;

	// scale the viewing volume
	projection[0][0] = xScale;
	projection[1][1] = yScale;

	// scale the z buffer from 0 at near plane to 1 at far plane
	projection[2][2] = far / (far - near);
	projection[2][3] = 1.f;
	projection[3][2] = -near * far / (far - near);
	projection[3][3] = 0.f;

	return projection;
#endif
}

Matrix4x4 eng::camera::GetProjection(const UserInterface& settings, const Vector2u& windowSize)
{
	const float right = (float)windowSize.x;
	const float bottom = (float)windowSize.y;
	const float near = math::Max(settings.m_ClippingNear, 0.1f);
	const float far = math::Max(settings.m_ClippingFar, near + 0.1f);

#ifdef Z_OPENGL
	Matrix4x4 projection = Matrix4x4::Identity;

	// set the viewing volume to be the size of the window
	// with 0,0 as the top left of the window
	projection[0][0] = 2.0f / (right);
	projection[1][1] = 2.0f / (-bottom);
	projection[3][0] = -1.f;
	projection[3][1] = 1.f;

	// scale the z buffer
	projection[2][2] = 1.f / (far - near);
	projection[3][2] = near / (near - far);

	return projection;
#endif
}

Vector3f eng::camera::ScreenToDirection(
	const Projection& projection,
	const Matrix4x4& transform, 
	const Vector3f& screenPos)
{
	if (std::holds_alternative<eng::camera::Orthographic>(projection))
	{
		const Quaternion cameraRotate = Quaternion::FromMatrix(transform);
		return Vector3f::AxisZ * cameraRotate;
	}

	const Vector3f translate = Matrix4x4::ToTranslate(transform);
	return (screenPos - translate).Normalized();
}

Vector3f eng::camera::ScreenToDirection(
	const eng::camera::ProjectionComponent& camera, 
	const eng::TransformComponent& transform, 
	const Vector3f& screenPos)
{
	if (std::holds_alternative<eng::camera::Orthographic>(camera.m_Projection))
	{
		const Quaternion cameraRotate = Quaternion::FromRotator(transform.m_Rotate);
		return Vector3f::AxisZ * cameraRotate;
	}

	return (screenPos - transform.m_Translate).Normalized();
}

Ray3f eng::camera::ScreenToRay(
	const Projection& projection,
	const Matrix4x4& transform,
	const Vector2u& windowSize,
	const Vector2f& pixelPos)
{
	Ray3f ray;
	ray.m_Position = eng::camera::ScreenToWorld(projection, transform, windowSize, pixelPos);
	ray.m_Direction = eng::camera::ScreenToDirection(projection, transform, ray.m_Position);
	return ray;
}

Ray3f eng::camera::ScreenToRay(
	const eng::camera::ProjectionComponent& camera,
	const eng::TransformComponent& transform,
	const eng::Window& window,
	const Vector2f& pixelPos)
{
	const Matrix4x4 cameraView = transform.ToTransform();
	const Vector2u& windowSize = window.GetSize();

	Ray3f ray;
	ray.m_Position = eng::camera::ScreenToWorld(camera.m_Projection, cameraView, windowSize, pixelPos);
	ray.m_Direction = eng::camera::ScreenToDirection(camera, transform, ray.m_Position);
	return ray;
}

Vector3f eng::camera::ScreenToWorld(
	const Projection& projection, 
	const Matrix4x4& transform, 
	const Vector2u& windowSize, 
	const Vector2f& pixelPos)
{
	return ScreenToWorld(projection, transform, windowSize, pixelPos, 0.f);
}

Vector3f eng::camera::ScreenToWorld(
	const Projection& projection, 
	const Matrix4x4& transform, 
	const Vector2u& windowSize, 
	const Vector2f& pixelPos, 
	const float depth)
{
	const Matrix4x4 inverseProj = GetProjection(projection, windowSize).Inversed();

	// pixel -> screen
	Vector3f screenPos;
	screenPos.x = +math::Remap(screenPos.x, 0.f, (float)windowSize.x, -1.f, 1.f);
	screenPos.y = -math::Remap(screenPos.y, 0.f, (float)windowSize.y, -1.f, 1.f);
	screenPos.z = depth;

	// screen -> homogeneous
	Vector4f homogeneousPos = Vector4f(screenPos, 1.f);
	homogeneousPos = homogeneousPos * inverseProj;

	// homogeneous -> local
	Vector3f localPos;
	localPos.x = homogeneousPos.x / homogeneousPos.w;
	localPos.y = homogeneousPos.y / homogeneousPos.w;
	localPos.z = homogeneousPos.z / homogeneousPos.w;

	// local -> world
	return localPos * transform;
}

Vector3f eng::camera::ScreenToWorld(
	const eng::camera::ProjectionComponent& camera,
	const eng::TransformComponent& transform,
	const eng::Window& window,
	const Vector2f& pixelPos)
{
	return ScreenToWorld(camera.m_Projection, transform.ToTransform(), window.GetSize(), pixelPos, 0.f);
}

Vector3f eng::camera::ScreenToWorld(
	const eng::camera::ProjectionComponent& camera,
	const eng::TransformComponent& transform,
	const eng::Window& window,
	const Vector2f& pixelPos,
	const float depth)
{
	return ScreenToWorld(camera.m_Projection, transform.ToTransform(), window.GetSize(), pixelPos, depth);
}