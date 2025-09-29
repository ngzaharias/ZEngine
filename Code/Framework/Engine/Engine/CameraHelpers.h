#pragma once

#include "Math/Ray.h"
#include "Math/Vector.h"

class Matrix4x4;

namespace eng
{
	class Window;
	struct TransformComponent;
}

namespace eng::camera
{
	struct ProjectionComponent;
}

namespace eng::camera
{
	struct Cinematic;
	struct Orthographic;
	struct Perspective;
	struct UserInterface;

	using Projection = Variant<
		//Cinematic, 
		Orthographic,
		Perspective,
		UserInterface>;
}

namespace eng::camera
{
	Matrix4x4 GetProjection(const Projection& value, const Vector2u& windowSize);
	Matrix4x4 GetProjection(const Cinematic& value, const Vector2u& windowSize);
	Matrix4x4 GetProjection(const Orthographic& value, const Vector2u& windowSize);
	Matrix4x4 GetProjection(const Perspective& value, const Vector2u& windowSize);
	Matrix4x4 GetProjection(const UserInterface& value, const Vector2u& windowSize);

	/// \brief Converts a screen position to a world direction using a projection and transform.
	Vector3f ScreenToDirection(
		const Projection& projection, 
		const Matrix4x4& transform, 
		const Vector3f& pixelPos);
	/// \brief Converts a screen position to a world direction using a camera and transform.
	Vector3f ScreenToDirection(
		const eng::camera::ProjectionComponent& camera,
		const eng::TransformComponent& transform,
		const Vector3f& pixelPos);

	/// \brief Converts a screen position to a world ray using a projection, transform and window.
	Ray3f ScreenToRay(
		const Projection& projection,
		const Matrix4x4& transform,
		const Vector2u& windowSize,
		const Vector2f& pixelPos);
	/// \brief Converts a screen position to a world ray using a projection, transform and window.
	Ray3f ScreenToRay(
		const eng::camera::ProjectionComponent& camera,
		const eng::TransformComponent& transform,
		const eng::Window& window,
		const Vector2f& pixelPos);

	/// \brief Converts a screen position to a world position using a projection, transform and size.
	Vector3f ScreenToWorld(
		const Projection& projection,
		const Matrix4x4& transform,
		const Vector2u& windowSize,
		const Vector2f& pixelPos);
	/// \brief Converts a screen position to a world position using a projection, transform and size.
	Vector3f ScreenToWorld(
		const Projection& projection,
		const Matrix4x4& transform,
		const Vector2u& windowSize,
		const Vector2f& pixelPos,
		const float depth);
	/// \brief Converts a screen position to a world position using the camera and window.
	Vector3f ScreenToWorld(
		const eng::camera::ProjectionComponent& camera,
		const eng::TransformComponent& transform,
		const eng::Window& window,
		const Vector2f& pixelPos);
	/// \brief Converts a screen position to a world position using the camera and window.
	Vector3f ScreenToWorld(
		const eng::camera::ProjectionComponent& camera,
		const eng::TransformComponent& transform,
		const eng::Window& window,
		const Vector2f& pixelPos,
		const float depth);
}