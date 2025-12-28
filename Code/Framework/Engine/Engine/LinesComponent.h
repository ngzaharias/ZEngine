#pragma once

#include "Core/Array.h"
#include "Core/Colour.h"
#include "ECS/Singleton.h"
#include "Engine/CameraTypes.h"
#include "Math/Vector.h"

class AABB3f;
class Circle2f;
class OBB3f;
class Rotator;
class Sphere3f;
class Vector2u;

namespace eng::camera
{
	struct Orthographic;
	struct Perspective;
	struct UserInterface;
	using Projection = Variant<
		Orthographic,
		Perspective,
		UserInterface>;
}

namespace eng
{
	struct LineVertex
	{
		Vector3f m_Point = { };
		Colour m_Colour = { };
	};

	struct LinesSingleton final : public ecs::Singleton<LinesSingleton>
	{
		/// \brief Adds an axis-aligned bounding box.
		void AddAABB(const AABB3f& extents, const Colour& colour);
		/// \brief Adds an axis-aligned bounding box.
		void AddAABB(const Vector3f& translate, const float extents, const Colour& colour);
		/// \brief Adds an axis-aligned bounding box.
		void AddAABB(const Vector3f& translate, const Vector3f& extents, const Colour& colour);
		/// \brief Adds an axis-aligned bounding box.
		void AddAABB(const Vector3f& translate, const AABB3f& extents, const Colour& colour);

		/// \brief Adds a circle.
		void AddCircle(const Vector3f& translate, const Circle2f& circle, const float steps, const Colour& colour);

		/// \brief Adds a frustum.
		void AddFrustrum(const Vector3f& translate, const Rotator& rotate, const eng::Projection& projection, const Vector2u& size, const Colour& colour);
		/// \brief Adds a frustum.
		void AddFrustrum(const Vector3f& translate, const Rotator& rotate, const eng::Orthographic& projection, const Vector2u& size, const Colour& colour);
		/// \brief Adds a frustum.
		void AddFrustrum(const Vector3f& translate, const Rotator& rotate, const eng::Perspective& projection, const Vector2u& size, const Colour& colour);
		/// \brief Adds a frustum.
		void AddFrustrum(const Vector3f& translate, const Rotator& rotate, const eng::camera::UserInterface& projection, const Vector2u& size, const Colour& colour);

		/// \brief Adds an icosphere.
		void AddIcosphere(const Vector3f& translate, const Sphere3f& sphere, const Colour& colour);

		/// \brief Adds a flat top hexagon.
		void AddHexagon(const Vector3f& translate, const float radius, const Colour& colour);

		/// \brief Adds a line.
		void AddLine(const Vector3f& pointA, const Vector3f& pointB, const Colour& colour);

		/// \brief Adds a orientated bounding box.
		void AddOBB(const Vector3f& translate, const OBB3f& obb, const Colour& colour);

		/// \brief Adds a sphere.
		void AddSphere(const Sphere3f& sphere, const Colour& colour);
		/// \brief Adds a sphere.
		void AddSphere(const Vector3f& translate, const float radius, const Colour& colour);
		/// \brief Adds a sphere.
		void AddSphere(const Vector3f& translate, const Sphere3f& sphere, const Colour& colour);

		Array<LineVertex> m_Vertices = { };
	};
}