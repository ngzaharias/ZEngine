#pragma once

#include "Core/Array.h"
#include "ECS/Component.h"
#include "Engine/CameraTypes.h"
#include "Math/Vector.h"

class AABB3f;
class Circle2f;
class OBB3f;
class Rotator;
class Sphere3f;

namespace eng::camera
{
	struct Orthographic;
	struct Perspective;
	using Projection = Variant<
		Orthographic,
		Perspective>;
}

namespace eng
{
	struct LineVertex
	{
		Vector3f m_Point = { };
		Vector4f m_Colour = { };
	};

	struct LinesComponent final : public ecs::SingletonComponent<LinesComponent>
	{
		/// \brief Adds an axis-aligned bounding box.
		void AddAABB(const Vector3f& translate, const float extents, const Vector4f& colour);
		/// \brief Adds an axis-aligned bounding box.
		void AddAABB(const Vector3f& translate, const Vector3f& extents, const Vector4f& colour);
		/// \brief Adds an axis-aligned bounding box.
		void AddAABB(const Vector3f& translate, const AABB3f& abb, const Vector4f& colour);

		/// \brief Adds a circle.
		void AddCircle(const Vector3f& translate, const Circle2f& circle, const Vector4f& colour);

		/// \brief Adds a frustum.
		void AddFrustrum(const Vector3f& translate, const Rotator& rotate, const eng::camera::Projection& projection, const Vector4f& colour);
		/// \brief Adds a frustum.
		void AddFrustrum(const Vector3f& translate, const Rotator& rotate, const eng::camera::Orthographic& projection, const Vector4f& colour);
		/// \brief Adds a frustum.
		void AddFrustrum(const Vector3f& translate, const Rotator& rotate, const eng::camera::Perspective& projection, const Vector4f& colour);

		/// \brief Adds an icosphere.
		void AddIcosphere(const Vector3f& translate, const Sphere3f& sphere, const Vector4f& colour);

		/// \brief Adds a flat top hexagon.
		void AddHexagon(const Vector3f& translate, const float radius, const Vector4f& colour);

		/// \brief Adds a line.
		void AddLine(const Vector3f& pointA, const Vector3f& pointB, const Vector4f& colour);

		/// \brief Adds a orientated bounding box.
		void AddOBB(const Vector3f& translate, const OBB3f& obb, const Vector4f& colour);

		/// \brief Adds a sphere.
		void AddSphere(const Vector3f& translate, const Sphere3f& sphere, const Vector4f& colour);

		Array<LineVertex> m_Vertices = { };
	};
}