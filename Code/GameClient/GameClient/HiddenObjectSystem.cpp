#include "GameClientPCH.h"
#include "GameClient/HiddenObjectSystem.h"

#include <Core/AABB.h>
#include <Core/CollisionMath.h>
#include <Core/Ray.h>
#include <Core/Sphere.h>

#include <ECS/EntityWorld.h>
#include <ECS/QueryTypes.h>
#include <ECS/WorldView.h>

#include <Engine/CameraComponent.h>
#include <Engine/CameraHelpers.h>
#include <Engine/LinesComponent.h>
#include <Engine/InputComponent.h>
#include <Engine/PhysicsSceneComponent.h>
#include <Engine/RigidStaticComponent.h>
#include <Engine/Screen.h>
#include <Engine/SpriteComponent.h>
#include <Engine/TransformComponent.h>

#include <PhysX/PxRigidActor.h>
#include <PhysX/PxScene.h>

#include "GameClient/HiddenObjectComponents.h"

namespace
{
	constexpr Vector4f s_ColourM = Vector4f(1.f, 0.f, 1.f, 1.f);
	constexpr Vector4f s_ColourW = Vector4f(1.f, 1.f, 1.f, 1.f);
}

void hidden::ObjectSystem::Update(World& world, const GameTime& gameTime)
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
		const Matrix4x4 cameraView = cameraTransform.ToTransform();

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
			constexpr float s_Distance = 100000.f;
			const Vector3f mousePosition = camera::ScreenToWorld(inputComponent.m_MousePosition, cameraComponent.m_Projection, cameraView);

			const physx::PxVec3 position = { 
				mousePosition.x,
				mousePosition.y,
				mousePosition.z };
			const physx::PxVec3 direction = { 
				cameraForward.x,
				cameraForward.y,
				cameraForward.z };

			physx::PxRaycastBuffer hitcall;
			sceneComponent.m_PhysicsScene->raycast(position, direction, s_Distance, hitcall);
			if (hitcall.hasAnyHits())
			{
				const physx::PxRaycastHit& closestHit = hitcall.block;
				const Vector3f hitPosition = Vector3f(closestHit.position.x, closestHit.position.y, closestHit.position.z);

				const ecs::Entity selectedEntity = reinterpret_cast<uint64>(closestHit.actor->userData);
				if (world.HasComponent<hidden::ObjectComponent>(selectedEntity))
				{
					const auto& hiddenComponent = world.GetComponent<const hidden::ObjectComponent>(selectedEntity);
					if (world.HasComponent<eng::TransformComponent>(selectedEntity))
					{
						constexpr Sphere3f s_Sphere = Sphere3f(Vector3f::Zero, 10.f);

						const float sizeX = (float)hiddenComponent.m_Size.x;
						const float sizeY = (float)hiddenComponent.m_Size.y;
						const AABB s_Extents = AABB::FromExtents(Vector3f(sizeX, sizeY, 1.f));

						const auto& transformComponent = world.GetComponent<const eng::TransformComponent>(selectedEntity);
						linesComponent.AddAABB(transformComponent.m_Translate, s_Extents, s_ColourM);
						linesComponent.AddSphere(hitPosition, s_Sphere, s_ColourM);
					}

					const bool isRevealed = world.HasComponent<eng::SpriteComponent>(selectedEntity);
					if (!isRevealed && inputComponent.IsKeyPressed(input::EMouse::Left))
					{
						auto& spriteComponent = world.AddComponent<eng::SpriteComponent>(selectedEntity);
						spriteComponent.m_Size = hiddenComponent.m_Size;
						spriteComponent.m_Sprite = hiddenComponent.m_Sprite;

						world.RemoveComponent<eng::RigidStaticComponent>(selectedEntity);
					}
				}
			}
		}
	}
}