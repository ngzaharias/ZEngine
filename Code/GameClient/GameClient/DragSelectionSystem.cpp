#include "GameClientPCH.h"
#include "GameClient/DragSelectionSystem.h"

#include <Core/CollisionMath.h>
#include <Core/Ray.h>

#include <ECS/EntityWorld.h>
#include <ECS/QueryTypes.h>
#include <ECS/WorldView.h>

#include <Engine/CameraComponent.h>
#include <Engine/CameraHelpers.h>
#include <Engine/LinesComponent.h>
#include <Engine/InputComponent.h>
#include <Engine/NameComponent.h>
#include <Engine/PhysicsSceneComponent.h>
#include <Engine/Screen.h>
#include <Engine/TransformComponent.h>

#include <PhysX/PxRigidActor.h>
#include <PhysX/PxScene.h>

#include "GameClient/DragComponents.h"

namespace
{
	constexpr Vector4f s_ColourM = Vector4f(1.f, 0.f, 1.f, 1.f);
	constexpr Vector4f s_ColourW = Vector4f(1.f, 1.f, 1.f, 1.f);
}

void drag::SelectionSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	const auto& sceneComponent = world.GetSingleton<const eng::PhysicsSceneComponent>();
	auto& linesComponent = world.GetSingleton<eng::LinesComponent>();

	for (const ecs::Entity& cameraEntity : world.Query<ecs::query::Include<const eng::CameraComponent, const eng::TransformComponent>>())
	{
		const auto& cameraComponent = world.GetComponent<const eng::CameraComponent>(cameraEntity);
		const auto& cameraTransform = world.GetComponent<const eng::TransformComponent>(cameraEntity);

		const Quaternion cameraRotate = Quaternion::FromRotator(cameraTransform.m_Rotate);
		const Vector3f& cameraTranslate = cameraTransform.m_Translate;
		const Vector3f cameraRight = Vector3f::AxisX * cameraRotate;
		const Vector3f cameraUpward = Vector3f::AxisY * cameraRotate;
		const Vector3f cameraForward = Vector3f::AxisZ * cameraRotate;

		const Vector2u screenSize = Vector2u(static_cast<uint32>(Screen::width), static_cast<uint32>(Screen::height));
		const Matrix4x4 cameraProj = camera::GetProjection(screenSize, cameraComponent.m_Projection);
		const Matrix4x4 cameraView = Matrix4x4::FromTransform(
			cameraTranslate,
			cameraRotate,
			cameraTransform.m_Scale);

		for (const ecs::Entity& inputEntity : world.Query<ecs::query::Include<const eng::InputComponent>>())
		{
			const auto& inputComponent = world.GetComponent<const eng::InputComponent>(inputEntity);
			const Vector3f cameraPoint = cameraTranslate + cameraForward * 30.f;

			// camera
			linesComponent.AddLine(
				cameraPoint - cameraRight * 0.1f,
				cameraPoint + cameraRight * 0.1f,
				s_ColourW);
			linesComponent.AddLine(
				cameraPoint - cameraUpward * 0.1f,
				cameraPoint + cameraUpward * 0.1f,
				s_ColourW);

			// mouse
			const float distance = 100000.f;
			const Vector3f mousePosition = camera::ScreenToWorld(inputComponent.m_MousePosition, cameraComponent.m_Projection, cameraView);
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
				if (world.HasComponent<eng::TransformComponent>(selectedEntity))
				{
					constexpr float s_Extents = 100.1f;

					const auto& transformComponent = world.GetComponent<const eng::TransformComponent>(selectedEntity);
					linesComponent.AddAABB(transformComponent.m_Translate, s_Extents, s_ColourM);

					if (inputComponent.IsKeyPressed(input::EMouse::Left))
					{
						const ecs::Entity dragEntity = world.CreateEntity();
						auto& nameComponent = world.AddComponent<eng::NameComponent>(dragEntity);
						nameComponent.m_Name = "Drag Entity";

						auto& dragComponent = world.AddComponent<drag::SelectionComponent>(dragEntity);
						dragComponent.m_CameraEntity = cameraEntity;
						dragComponent.m_InputEntity = inputEntity;
						dragComponent.m_SelectedEntity = selectedEntity;
						dragComponent.m_TranslatePlane = Plane(-cameraForward, hitPosition);
						dragComponent.m_TranslateOffset = transformComponent.m_Translate - hitPosition;
					}
				}
			}
		}
	}

	for (const ecs::Entity& dragEntity : world.Query<ecs::query::Include<const drag::SelectionComponent>>())
	{
		const auto& dragComponent = world.GetComponent<const drag::SelectionComponent>(dragEntity);
		const auto& inputComponent = world.GetComponent<const eng::InputComponent>(dragComponent.m_InputEntity);

		if (inputComponent.IsKeyReleased(input::EMouse::Left))
			world.DestroyEntity(dragEntity);
	}
}