#include "GameClientPCH.h"
#include "GameClient/DragMovementSystem.h"

#include <Core/CollisionMath.h>
#include <Core/OBB.h>
#include <Core/Ray.h>
#include <Core/Triangle.h>

#include <ECS/EntityWorld.h>
#include <ECS/NameComponent.h>
#include <ECS/QueryTypes.h>
#include <ECS/WorldView.h>

#include <Engine/CameraComponent.h>
#include <Engine/CameraHelpers.h>
#include <Engine/LinesComponent.h>
#include <Engine/InputComponent.h>
#include <Engine/PhysicsSceneComponent.h>
#include <Engine/Screen.h>
#include <Engine/TransformComponent.h>

#include <PhysX/PxRigidActor.h>
#include <PhysX/PxScene.h>

#include "GameClient/DragComponents.h"

namespace
{
	constexpr Vector4f s_ColourW = Vector4f(1.f, 1.f, 1.f, 1.f);
}

void drag::MovementSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	auto& linesComponent = world.GetSingleton<eng::LinesComponent>();

	for (const ecs::Entity& dragEntity : world.Query<ecs::query::Include<const drag::SelectionComponent>>())
	{
		const auto& dragComponent = world.GetComponent<const drag::SelectionComponent>(dragEntity);
		const auto& inputComponent = world.GetComponent<const eng::InputComponent>(dragComponent.m_InputEntity);

		const auto& cameraComponent = world.GetComponent<const eng::CameraComponent>(dragComponent.m_CameraEntity);
		const auto& cameraTransform = world.GetComponent<const eng::TransformComponent>(dragComponent.m_CameraEntity);

		const Quaternion cameraRotate = Quaternion::FromRotator(cameraTransform.m_Rotate);
		const Vector3f& cameraTranslate = cameraTransform.m_Translate;
		const Matrix4x4 cameraView = cameraTransform.ToTransform();

		const Vector3f mousePosition = camera::ScreenToWorld(inputComponent.m_MousePosition, cameraComponent.m_Projection, cameraView);
		const Vector3f mouseForward = (mousePosition - cameraTranslate).Normalized();

		// debug
		{
			const Vector3f planeTranslate = dragComponent.m_TranslatePlane.m_Point;
			const Vector3f planeForward = dragComponent.m_TranslatePlane.m_Normal;
			const Vector3f planeRight = math::Cross(planeForward, Vector3f::AxisY);
			const Vector3f planeUp = math::Cross(planeForward, planeRight);

			const float pitch = math::ToDegrees(asinf(-planeForward.y));
			const float yaw = math::ToDegrees(atan2f(planeForward.x, planeForward.z));
			const Rotator rotator = Rotator(pitch, yaw, 0.f);

			const OBB obb = OBB::FromExtents(rotator, Vector3f(500.f, 500.f, 0.f));
			linesComponent.AddOBB(planeTranslate, obb, s_ColourW);
		}

		{
			Ray3f ray;
			ray.m_Position = mousePosition;
			ray.m_Direction = mouseForward;

			Vector3f intersectPos;
			if (math::Intersection(ray, dragComponent.m_TranslatePlane, intersectPos))
			{
				auto& selectedTransform = world.GetComponent<eng::TransformComponent>(dragComponent.m_SelectedEntity);
				selectedTransform.m_Translate = intersectPos + dragComponent.m_TranslateOffset;
			}
		}
	}

	for (const ecs::Entity& inputEntity : world.Query<ecs::query::Include<const eng::InputComponent>>())
	{
		for (const ecs::Entity& cameraEntity : world.Query<ecs::query::Include<const eng::CameraComponent>>())
		{
			const auto& camera = world.GetComponent<const eng::CameraComponent>(cameraEntity);
			const auto& transform = world.GetComponent<const eng::TransformComponent>(cameraEntity);
			const auto& input = world.GetComponent<const eng::InputComponent>(inputEntity);

			Vector3f position = camera::ScreenToWorld(
				input.m_MousePosition, 
				camera.m_Projection, 
				transform.ToTransform());

			{
				Ray3f ray;
				ray.m_Position = position;
				ray.m_Direction = (position - transform.m_Translate).Normalized();

				Plane3f plane;
				plane.m_Point = Vector3f::Zero;
				plane.m_Normal = Vector3f::AxisZ;

				math::Intersection(ray, plane, position);
			}

			Triangle3f triangleA;
			triangleA.m_PointA = position + Vector3f(-70.f, -110.f, 0.f);
			triangleA.m_PointB = position + Vector3f(+30.f, +90.f, 0.f);
			triangleA.m_PointC = position + Vector3f(+90.f, -80.f, 0.f);

			Triangle3f triangleB;
			triangleB.m_PointA = Vector3f(200.f, 400.f, 0.f) + Vector3f(-1000.f, -1000.f, 0.f);
			triangleB.m_PointB = Vector3f(200.f, 400.f, 0.f) + Vector3f(   00.f, +1000.f, 0.f);
			triangleB.m_PointC = Vector3f(200.f, 400.f, 0.f) + Vector3f(+1000.f, -1000.f, 0.f);

			Triangle2f a;
			a.m_PointA = triangleA.m_PointA.XY();
			a.m_PointB = triangleA.m_PointB.XY();
			a.m_PointC = triangleA.m_PointC.XY();

			Triangle2f b;
			b.m_PointA = triangleB.m_PointA.XY();
			b.m_PointB = triangleB.m_PointB.XY();
			b.m_PointC = triangleB.m_PointC.XY();

			const bool isColliding = math::IsOverlapping(a, b);
			const Vector4f colour = isColliding ? Vector4f(1.f, 0.f, 0.f, 1.f) : Vector4f(1.f);
			linesComponent.AddLine(triangleA.m_PointA, triangleA.m_PointB, colour);
			linesComponent.AddLine(triangleA.m_PointB, triangleA.m_PointC, colour);
			linesComponent.AddLine(triangleA.m_PointC, triangleA.m_PointA, colour);

			linesComponent.AddLine(triangleB.m_PointA, triangleB.m_PointB, colour);
			linesComponent.AddLine(triangleB.m_PointB, triangleB.m_PointC, colour);
			linesComponent.AddLine(triangleB.m_PointC, triangleB.m_PointA, colour);
		}
	}
}