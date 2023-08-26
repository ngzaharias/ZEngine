#pragma once

#include <Core/Array.h>
#include <Core/Vector.h>

#include <ECS/Component.h>

#include <Engine/CameraTypes.h>

class AABB;
class OBB;
class Rotator;

namespace eng
{
	struct LineVertex
	{
		Vector3f m_Point = { };
		Vector4f m_Colour = { };
	};

	struct LinesComponent final : public ecs::SingletonComponent<LinesComponent>
	{
		void AddAABB(const Vector3f& translate, const AABB& abb, const Vector4f& colour);
		void AddAABB(const Vector3f& translate, const float extents, const Vector4f& colour);

		void AddFrustrum(const Vector3f& translate, const Rotator& rotate, const camera::Projection& projection, const Vector4f& colour);
		void AddFrustrum(const Vector3f& translate, const Rotator& rotate, const camera::Orthographic& projection, const Vector4f& colour);
		void AddFrustrum(const Vector3f& translate, const Rotator& rotate, const camera::Perspective& projection, const Vector4f& colour);

		void AddLine(const Vector3f& pointA, const Vector3f& pointB, const Vector4f& colour);

		void AddOBB(const Vector3f& translate, const OBB& obb, const Vector4f& colour);

		void AddSphere(const Vector3f& translate, const float radius, const Vector4f& colour);

		Array<LineVertex> m_Vertices = { };
	};
}