#include "HiddenPCH.h"
#include "Hidden/HiddenRevealSystem.h"

#include "Core/Colour.h"
#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/CameraHelpers.h"
#include "Engine/CameraComponent.h"
#include "Engine/InputManager.h"
#include "Engine/LinesComponent.h"
#include "Engine/PhysicsSceneComponent.h"
#include "Engine/RigidStaticComponent.h"
#include "Engine/TransformComponent.h"
#include "Engine/Window.h"
#include "Engine/WindowManager.h"
#include "Hidden/HiddenDebugSettingsSingleton.h"
#include "Hidden/HiddenObjectComponent.h"
#include "Hidden/HiddenRevealComponent.h"
#include "Math/AABB.h"
#include "Math/CollisionMath.h"
#include "Math/Ray.h"
#include "Math/Sphere.h"

#include <PhysX/PxRigidActor.h>
#include <PhysX/PxScene.h>


namespace
{
	const str::Name strInput = str::Name::Create("HiddenReveal");
	const str::Name strSelect = str::Name::Create("HiddenReveal_Select");

	Vector3f ToMouseDirection(const Vector3f& mousePosition, const eng::CameraComponent& camera, const eng::TransformComponent& transform)
	{
		if (std::holds_alternative<eng::Orthographic>(camera.m_Projection))
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

	if (world.HasAny<ecs::query::Added<hidden::ObjectComponent>>())
	{
		input::Layer layer;
		layer.m_Priority = eng::EInputPriority::Gameplay;
		layer.m_Bindings.Emplace(strSelect, input::EKey::Mouse_Left, false);

		auto& input = world.WriteResource<eng::InputManager>();
		input.AppendLayer(strInput, layer);
	}

	if (world.HasAny<ecs::query::Removed<hidden::ObjectComponent>>())
	{
		auto& input = world.WriteResource<eng::InputManager>();
		input.RemoveLayer(strInput);
	}

	const auto& windowManager = world.ReadResource<eng::WindowManager>();
	const eng::Window* window = windowManager.GetWindow(0);
	if (!window)
		return;

	const auto& settings = world.ReadSingleton<hidden::settings::DebugSingleton>();
	const auto& physics = world.ReadSingleton<eng::PhysicsSceneSingleton>();

	using Query = ecs::query
		::Include<
		const eng::ActiveComponent,
		const eng::CameraComponent,
		const eng::TransformComponent>;
	for (auto&& view : world.Query<Query>())
	{
		const auto& cameraProjection = view.ReadRequired<eng::CameraComponent>();
		const auto& cameraTransform = view.ReadRequired<eng::TransformComponent>();

		{
			const auto& input = world.ReadResource<eng::InputManager>();

			// mouse
			constexpr float s_Distance = 100000.f;
			const Ray3f ray = eng::ScreenToRay(
				cameraProjection,
				cameraTransform,
				*window,
				input.m_MousePosition);

			const physx::PxVec3 position = {
				ray.m_Position.x,
				ray.m_Position.y,
				ray.m_Position.z };
			const physx::PxVec3 direction = {
				ray.m_Direction.x,
				ray.m_Direction.y,
				ray.m_Direction.z };

			physx::PxRaycastBuffer hitcall;
			physics.m_PhysicsScene->raycast(position, direction, s_Distance, hitcall);
			if (hitcall.hasAnyHits())
			{
				const physx::PxRaycastHit& closestHit = hitcall.block;
				const Vector3f hitPosition = Vector3f(closestHit.position.x, closestHit.position.y, closestHit.position.z);

				const ecs::Entity selectedEntity = reinterpret_cast<uint64>(closestHit.actor->userData);
				if (world.HasComponent<hidden::ObjectComponent>(selectedEntity))
				{
					const bool isRevealed = world.HasComponent<hidden::RevealComponent>(selectedEntity);
					if (!isRevealed && input.IsPressed(strSelect))
						world.AddComponent<hidden::RevealComponent>(selectedEntity);
				}

				if (settings.m_IsInputEnabled)
				{
					auto& lines = world.WriteSingleton<eng::LinesSingleton>();
					lines.AddSphere(hitPosition, Sphere3f(10.f), Colour::Red);
				}
			}
			else
			{
				if (settings.m_IsInputEnabled)
				{
					auto& lines = world.WriteSingleton<eng::LinesSingleton>();
					lines.AddSphere(ray.m_Position + ray.m_Direction * 100.f, Sphere3f(10.f), Colour::Green);
				}
			}
		}
	}
}