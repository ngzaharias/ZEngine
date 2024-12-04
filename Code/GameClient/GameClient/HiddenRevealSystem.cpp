#include "GameClientPCH.h"
#include "GameClient/HiddenRevealSystem.h"

#include "Core/Colour.h"
#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/CameraComponent.h"
#include "Engine/CameraHelpers.h"
#include "Engine/InputComponent.h"
#include "Engine/LinesComponent.h"
#include "Engine/PhysicsSceneComponent.h"
#include "Engine/RigidStaticComponent.h"
#include "Engine/Screen.h"
#include "Engine/TransformComponent.h"
#include "GameClient/HiddenObjectComponent.h"
#include "GameClient/HiddenRevealComponent.h"
#include "GameClient/SettingsComponents.h"
#include "Math/AABB.h"
#include "Math/CollisionMath.h"
#include "Math/Ray.h"
#include "Math/Sphere.h"

#include <PhysX/PxRigidActor.h>
#include <PhysX/PxScene.h>


namespace
{
	const str::Guid strSoundSequence = GUID("2bde153c851a429c88957b6b0fd482fb");

	Vector3f ToMouseDirection(const Vector3f& mousePosition, const eng::camera::ProjectionComponent& camera, const eng::TransformComponent& transform)
	{
		if (std::holds_alternative<eng::camera::Orthographic>(camera.m_Projection))
		{
			const Quaternion cameraRotate = Quaternion::FromRotator(transform.m_Rotate);
			return Vector3f::AxisZ * cameraRotate;
		}

		return (mousePosition - transform.m_Translate).Normalized();
	}
}

void hidden::RevealSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	const auto& settings = world.ReadSingleton<clt::settings::DebugComponent>();
	const auto& sceneComponent = world.ReadSingleton<eng::PhysicsSceneComponent>();

	for (const ecs::Entity& cameraEntity : world.Query<ecs::query::Include<const eng::camera::ProjectionComponent, const eng::TransformComponent>>())
	{
		const auto& camera = world.ReadComponent<eng::camera::ProjectionComponent>(cameraEntity);
		const auto& transform = world.ReadComponent<eng::TransformComponent>(cameraEntity);

		const Vector2u screenSize = Vector2u(static_cast<uint32>(Screen::width), static_cast<uint32>(Screen::height));
		const Matrix4x4 cameraView = transform.ToTransform();

		for (const ecs::Entity& inputEntity : world.Query<ecs::query::Include<const eng::InputComponent>>())
		{
			const auto& inputComponent = world.ReadComponent<eng::InputComponent>(inputEntity);

			// mouse
			constexpr float s_Distance = 100000.f;
			const Vector3f mousePosition = eng::camera::ScreenToWorld(inputComponent.m_MousePosition, camera.m_Projection, cameraView);
			const Vector3f mouseDirection = ToMouseDirection(mousePosition, camera, transform);

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
					const bool isRevealed = world.HasComponent<hidden::RevealComponent>(selectedEntity);
					if (!isRevealed && inputComponent.IsKeyPressed(input::EMouse::Left))
						world.AddComponent<hidden::RevealComponent>(selectedEntity);
				}

				if (settings.m_IsHiddenInputEnabled)
				{
					auto& lines = world.WriteSingleton<eng::LinesComponent>();
					lines.AddSphere(hitPosition, Sphere3f(10.f), Colour::Red);
				}
			}
			else
			{
				if (settings.m_IsHiddenInputEnabled)
				{
					auto& lines = world.WriteSingleton<eng::LinesComponent>();
					lines.AddSphere(mousePosition + mouseDirection * 100.f, Sphere3f(10.f), Colour::Green);
				}
			}
		}
	}
}