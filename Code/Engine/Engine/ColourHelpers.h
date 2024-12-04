#pragma once

#include "Math/Vector.h"

namespace ecs
{
	struct Entity;
}

namespace colour
{
	Vector3f LerpRGB(const Vector3f& a, const Vector3f& b, const float t);

	Vector3f ToHSV(const Vector3f& rgb);
	Vector3f ToRGB(const Vector3f& hsv);

	const Vector4f& From(const void* value);
	const Vector4f& From(const int32 value);
	const Vector4f& From(const uint32 value);
	const Vector4f& From(const ecs::Entity& value);

	const Vector4f& Generate();
}
