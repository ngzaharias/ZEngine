#include "GameClientPCH.h"
#include "GameClient/DragMovementSystem.h"

#include "Core/Colour.h"
#include "ECS/EntityWorld.h"
#include "ECS/NameComponent.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/CameraComponent.h"
#include "Engine/CameraHelpers.h"
#include "Engine/LinesComponent.h"
#include "Engine/InputManager.h"
#include "Engine/PhysicsSceneComponent.h"
#include "Engine/TransformComponent.h"
#include "Engine/Window.h"
#include "Engine/WindowManager.h"
#include "GameClient/DragComponents.h"
#include "Math/CollisionMath.h"
#include "Math/OBB.h"
#include "Math/Ray.h"
#include "Math/Triangle.h"

#include <PhysX/PxRigidActor.h>
#include <PhysX/PxScene.h>

void drag::MovementSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	const auto& windowManager = world.ReadResource<eng::WindowManager>();
	const eng::Window* window = windowManager.GetWindow(0);
	if (!window)
		return;

	const auto& input = world.ReadResource<eng::InputManager>();
	for (const ecs::Entity& dragEntity : world.Query<ecs::query::Include<const drag::SelectionComponent>>())
	{
		const auto& dragComponent = world.ReadComponent<drag::SelectionComponent>(dragEntity);
		const auto& cameraComponent = world.ReadComponent<eng::camera::ProjectionComponent>(dragComponent.m_CameraEntity);
		const auto& cameraTransform = world.ReadComponent<eng::TransformComponent>(dragComponent.m_CameraEntity);

		const Ray3f ray = eng::camera::ScreenToRay(
			cameraComponent, 
			cameraTransform,
			*window,
			input.m_MousePosition);

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

			auto& linesComponent = world.WriteSingleton<eng::LinesSingleton>();
			linesComponent.AddOBB(planeTranslate, obb, Colour::White);
		}

		{
			Vector3f intersectPos;
			if (math::Intersection(ray, dragComponent.m_TranslatePlane, intersectPos))
			{
				auto& selectedTransform = world.WriteComponent<eng::TransformComponent>(dragComponent.m_SelectedEntity);
				selectedTransform.m_Translate = intersectPos + dragComponent.m_TranslateOffset;
			}
		}
	}
}