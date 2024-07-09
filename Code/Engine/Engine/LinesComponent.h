#pragma once

#include "Core/Array.h"
#include "Core/Vector.h"
#include "ECS/Component.h"
#include "Engine/CameraTypes.h"

class AABB3f;
class Circle2f;
class OBB3f;
class Rotator;
class Sphere3f;

namespace eng
{
	struct LineVertex
	{
		Vector3f m_Point = { };
		Vector4f m_Colour = { };
	};

	struct LinesComponent final : public ecs::SingletonComponent<LinesComponent>
	{
		void AddAABB(const Vector3f& translate, const AABB3f& abb, const Vector4f& colour);
		void AddAABB(const Vector3f& translate, const float extents, const Vector4f& colour);

		void AddCircle(const Vector3f& translate, const Circle2f& circle, const Vector4f& colour);

		void AddFrustrum(const Vector3f& translate, const Rotator& rotate, const camera::Projection& projection, const Vector4f& colour);
		void AddFrustrum(const Vector3f& translate, const Rotator& rotate, const camera::Orthographic& projection, const Vector4f& colour);
		void AddFrustrum(const Vector3f& translate, const Rotator& rotate, const camera::Perspective& projection, const Vector4f& colour);

		void AddLine(const Vector3f& pointA, const Vector3f& pointB, const Vector4f& colour);

		void AddOBB(const Vector3f& translate, const OBB3f& obb, const Vector4f& colour);

		void AddSphere(const Vector3f& translate, const Sphere3f& sphere, const Vector4f& colour);

		Array<LineVertex> m_Vertices = { };
	};
}