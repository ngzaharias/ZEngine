#pragma once

#include "Engine/CameraTypes.h"
#include "Math/Ray.h"

class Matrix4x4;
class Ray3f;
class Vector2f;
class Vector2u;
class Vector3f;

namespace eng
{
	class Window;
	struct CameraComponent;
	struct TransformComponent;
}

namespace eng
{
	Matrix4x4 GetProjection(const eng::Projection& value, const Vector2u& windowSize);
	Matrix4x4 GetProjection(const eng::Cinematic& value, const Vector2u& windowSize);
	Matrix4x4 GetProjection(const eng::Orthographic& value, const Vector2u& windowSize);
	Matrix4x4 GetProjection(const eng::Perspective& value, const Vector2u& windowSize);
	Matrix4x4 GetProjection(const eng::UserInterface& value, const Vector2u& windowSize);

	/// \brief Converts a screen position to a world direction using a projection and transform.
	Vector3f ScreenToDirection(
		const eng::Projection& projection,
		const Matrix4x4& transform, 
		const Vector3f& pixelPos);
	/// \brief Converts a screen position to a world direction using a camera and transform.
	Vector3f ScreenToDirection(
		const eng::CameraComponent& camera,
		const eng::TransformComponent& transform,
		const Vector3f& pixelPos);

	/// \brief Converts a screen position to a world ray using a projection, transform and window.
	Ray3f ScreenToRay(
		const eng::Projection& projection,
		const Matrix4x4& transform,
		const Vector2u& windowSize,
		const Vector2f& pixelPos);
	/// \brief Converts a screen position to a world ray using a projection, transform and window.
	Ray3f ScreenToRay(
		const eng::CameraComponent& camera,
		const eng::TransformComponent& transform,
		const eng::Window& window,
		const Vector2f& pixelPos);

	/// \brief Converts a screen position to a world position using a projection, transform and size.
	Vector3f ScreenToWorld(
		const eng::Projection& projection,
		const Matrix4x4& transform,
		const Vector2u& windowSize,
		const Vector2f& pixelPos);
	/// \brief Converts a screen position to a world position using a projection, transform and size.
	Vector3f ScreenToWorld(
		const eng::Projection& projection,
		const Matrix4x4& transform,
		const Vector2u& windowSize,
		const Vector2f& pixelPos,
		const float depth);
	/// \brief Converts a screen position to a world position using the camera and window.
	Vector3f ScreenToWorld(
		const eng::CameraComponent& camera,
		const eng::TransformComponent& transform,
		const eng::Window& window,
		const Vector2f& pixelPos);
	/// \brief Converts a screen position to a world position using the camera and window.
	Vector3f ScreenToWorld(
		const eng::CameraComponent& camera,
		const eng::TransformComponent& transform,
		const eng::Window& window,
		const Vector2f& pixelPos,
		const float depth);
}