#include "GameClientPCH.h"
#include "GameClient/HiddenRevealSystem.h"

#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/CameraComponent.h"
#include "Engine/CameraHelpers.h"
#include "Engine/LinesComponent.h"
#include "Engine/InputComponent.h"
#include "Engine/PhysicsSceneComponent.h"
#include "Engine/RigidStaticComponent.h"
#include "Engine/Screen.h"
#include "Engine/SoundComponents.h"
#include "Engine/SpriteComponent.h"
#include "Engine/TransformComponent.h"
#include "GameClient/HiddenObjectComponents.h"
#include "Math/AABB.h"
#include "Math/CollisionMath.h"
#include "Math/Ray.h"
#include "Math/Sphere.h"

#include <PhysX/PxRigidActor.h>
#include <PhysX/PxScene.h>


namespace
{
	constexpr Vector4f s_ColourM = Vector4f(1.f, 0.f, 1.f, 1.f);
	constexpr Vector4f s_ColourW = Vector4f(1.f, 1.f, 1.f, 1.f);

	const str::Guid strSoundSequence = GUID("2bde153c-851a-429c-8895-7b6b0fd482fb");

	Vector3f ToMouseDirection(const Vector3f& mousePosition, const eng::CameraComponent& camera, const eng::TransformComponent& transform)
	{
		if (std::holds_alternative<camera::Orthographic>(camera.m_Projection))
		{
			const Quaternion cameraRotate = Quaternion::FromRotator(transform.m_Rotate);
			return Vector3f::AxisZ * cameraRotate;
		}

		return (mousePosition - transform.m_Translate).Normalized();
	}
}

void hidden::RevealSystem::Initialise(World& world)
{
	m_SoundEntity = world.CreateEntity();
}

void hidden::RevealSystem::Shutdown(World& world)
{
	world.DestroyEntity(m_SoundEntity);
}

void hidden::RevealSystem::Update(World& world, const GameTime& gameTime)
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
			const Vector3f mouseDirection = ToMouseDirection(mousePosition, cameraComponent, cameraTransform);

			const physx::PxVec3 position = { 
				mousePosition.x,
				mousePosition.y,
				mousePosition.z };
			const physx::PxVec3 direction = { 
				mouseDirection.x,
				mouseDirection.y,
				mouseDirection.z };

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
						const AABB3f s_Extents = AABB3f::FromExtents(Vector3f(sizeX, sizeY, 1.f));

						const auto& transformComponent = world.GetComponent<const eng::TransformComponent>(selectedEntity);
						linesComponent.AddAABB(transformComponent.m_Translate, s_Extents, s_ColourM);
						linesComponent.AddSphere(hitPosition, s_Sphere, s_ColourM);
					}

					const bool isRevealed = world.HasComponent<hidden::RevealedComponent>(selectedEntity);
					if (!isRevealed && inputComponent.IsKeyPressed(input::EMouse::Left))
						world.AddComponent<hidden::RevealedComponent>(selectedEntity);
				}
			}
		}
	}
}