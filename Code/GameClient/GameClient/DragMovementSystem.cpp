#include "GameClientPCH.h"
#include "GameClient/DragMovementSystem.h"

#include "ECS/EntityWorld.h"
#include "ECS/NameComponent.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/CameraComponent.h"
#include "Engine/CameraHelpers.h"
#include "Engine/LinesComponent.h"
#include "Engine/InputComponent.h"
#include "Engine/PhysicsSceneComponent.h"
#include "Engine/Screen.h"
#include "Engine/TransformComponent.h"
#include "GameClient/DragComponents.h"
#include "Math/CollisionMath.h"
#include "Math/OBB.h"
#include "Math/Ray.h"
#include "Math/Triangle.h"

#include <PhysX/PxRigidActor.h>
#include <PhysX/PxScene.h>

namespace
{
	constexpr Vector4f s_ColourW = Vector4f(1.f, 1.f, 1.f, 1.f);
}

void drag::MovementSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	auto& linesComponent = world.WriteSingleton<eng::LinesComponent>();

	for (const ecs::Entity& dragEntity : world.Query<ecs::query::Include<const drag::SelectionComponent>>())
	{
		const auto& dragComponent = world.ReadComponent<drag::SelectionComponent>(dragEntity);
		const auto& inputComponent = world.ReadComponent<eng::InputComponent>(dragComponent.m_InputEntity);

		const auto& cameraComponent = world.ReadComponent<eng::camera::ProjectionComponent>(dragComponent.m_CameraEntity);
		const auto& cameraTransform = world.ReadComponent<eng::TransformComponent>(dragComponent.m_CameraEntity);

		const Quaternion cameraRotate = Quaternion::FromRotator(cameraTransform.m_Rotate);
		const Vector3f& cameraTranslate = cameraTransform.m_Translate;
		const Matrix4x4 cameraView = cameraTransform.ToTransform();

		const Vector3f mousePosition = eng::camera::ScreenToWorld(inputComponent.m_MousePosition, cameraComponent.m_Projection, cameraView);
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

			const OBB3f obb = OBB3f::FromExtents(rotator, Vector3f(500.f, 500.f, 0.f));
			linesComponent.AddOBB(planeTranslate, obb, s_ColourW);
		}

		{
			Ray3f ray;
			ray.m_Position = mousePosition;
			ray.m_Direction = mouseForward;

			Vector3f intersectPos;
			if (math::Intersection(ray, dragComponent.m_TranslatePlane, intersectPos))
			{
				auto& selectedTransform = world.WriteComponent<eng::TransformComponent>(dragComponent.m_SelectedEntity);
				selectedTransform.m_Translate = intersectPos + dragComponent.m_TranslateOffset;
			}
		}
	}
}