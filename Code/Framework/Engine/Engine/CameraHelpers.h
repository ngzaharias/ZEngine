#pragma once

#include "Engine/CameraTypes.h"
#include "Math/Matrix.h"

namespace eng::camera
{
	Matrix4x4 GetProjection(const Vector2u& windowSize, const Projection& value);

	Matrix4x4 GetProjection(const Vector2u& windowSize, const Cinematic& value);
	Matrix4x4 GetProjection(const Vector2u& windowSize, const Orthographic& value);
	Matrix4x4 GetProjection(const Vector2u& windowSize, const Perspective& value);
	Matrix4x4 GetProjection(const Vector2u& windowSize, const UserInterface& value);

	/// \brief Converts a screen position to a world position using a projection and transform matrix.
	Vector3f ScreenToWorld(const Vector2f& pixelPos, const Projection& projection, const Matrix4x4& transform, const Vector2u& windowSize);

	/// \brief Converts a screen position to a world position using a projection and transform matrix.
	Vector3f ScreenToWorld(const Vector2f& pixelPos, const float depth, const Projection& projection, const Matrix4x4& transform, const Vector2u& windowSize);

	/// \brief Converts a world position to a screen position using a projection and transform matrix.
	Vector2f WorldToScreen(const Vector3f& worldPos, const Projection& projection, const Matrix4x4& transform, const Vector2u& windowSize);
}