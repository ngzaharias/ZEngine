#include "GameClientPCH.h"
#include "GameClient/DragSelectionSystem.h"

#include "Core/Colour.h"
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
#include "Math/Ray.h"

#include <PhysX/PxRigidActor.h>
#include <PhysX/PxScene.h>

void drag::SelectionSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	const auto& sceneComponent = world.ReadSingleton<eng::PhysicsSceneComponent>();
	auto& linesComponent = world.WriteSingleton<eng::LinesComponent>();

	for (const ecs::Entity& cameraEntity : world.Query<ecs::query::Include<const eng::camera::ProjectionComponent, const eng::TransformComponent>>())
	{
		const auto& cameraComponent = world.ReadComponent<eng::camera::ProjectionComponent>(cameraEntity);
		const auto& cameraTransform = world.ReadComponent<eng::TransformComponent>(cameraEntity);

		const Quaternion cameraRotate = Quaternion::FromRotator(cameraTransform.m_Rotate);
		const Vector3f& cameraTranslate = cameraTransform.m_Translate;
		const Vector3f cameraForward = Vector3f::AxisZ * cameraRotate;

		const Matrix4x4 cameraView = cameraTransform.ToTransform();
		for (const ecs::Entity& inputEntity : world.Query<ecs::query::Include<const eng::InputComponent>>())
		{
			const auto& inputComponent = world.ReadComponent<eng::InputComponent>(inputEntity);

			// mouse
			const float distance = 100000.f;
			const Vector3f mousePosition = eng::camera::ScreenToWorld(inputComponent.m_MousePosition, cameraComponent.m_Projection, cameraView);
			const Vector3f mouseForward = (mousePosition - cameraTranslate).Normalized();

			const physx::PxVec3 position = { 
				mousePosition.x,
				mousePosition.y,
				mousePosition.z };
			const physx::PxVec3 direction = { 
				mouseForward.x,
				mouseForward.y,
				mouseForward.z };

			physx::PxRaycastBuffer hitcall;
			sceneComponent.m_PhysicsScene->raycast(position, direction, distance, hitcall);
			if (hitcall.hasAnyHits())
			{
				const physx::PxRaycastHit& closestHit = hitcall.block;
				const Vector3f hitPosition = Vector3f(closestHit.position.x, closestHit.position.y, closestHit.position.z);

				const ecs::Entity selectedEntity = reinterpret_cast<uint64>(closestHit.actor->userData);
				const bool hasSelectable = world.HasComponent<drag::IsSelectableComponent>(selectedEntity);
				const bool hasTransform = world.HasComponent<eng::TransformComponent>(selectedEntity);
				if (hasSelectable && hasTransform)
				{
					constexpr float s_Extents = 100.1f;

					const auto& transformComponent = world.ReadComponent<eng::TransformComponent>(selectedEntity);
					linesComponent.AddAABB(transformComponent.m_Translate, s_Extents, Colour::Magenta);

					if (inputComponent.IsKeyPressed(input::EMouse::Left))
					{
						const ecs::Entity dragEntity = world.CreateEntity();
						auto& nameComponent = world.AddComponent<ecs::NameComponent>(dragEntity);
						nameComponent.m_Name = "Drag Entity";

						auto& dragComponent = world.AddComponent<drag::SelectionComponent>(dragEntity);
						dragComponent.m_CameraEntity = cameraEntity;
						dragComponent.m_InputEntity = inputEntity;
						dragComponent.m_SelectedEntity = selectedEntity;
						dragComponent.m_TranslatePlane = Plane3f(-cameraForward, hitPosition);
						dragComponent.m_TranslateOffset = transformComponent.m_Translate - hitPosition;
					}
				}
			}
		}
	}

	for (const ecs::Entity& dragEntity : world.Query<ecs::query::Include<const drag::SelectionComponent>>())
	{
		const auto& dragComponent = world.ReadComponent<drag::SelectionComponent>(dragEntity);
		const auto& inputComponent = world.ReadComponent<eng::InputComponent>(dragComponent.m_InputEntity);

		if (!inputComponent.IsKeyHeld(input::EMouse::Left))
			world.DestroyEntity(dragEntity);
	}
}