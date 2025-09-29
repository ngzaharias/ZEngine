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
#include "Engine/InputManager.h"
#include "Engine/PhysicsSceneComponent.h"
#include "Engine/TransformComponent.h"
#include "Engine/Window.h"
#include "Engine/WindowManager.h"
#include "GameClient/DragComponents.h"
#include "Math/CollisionMath.h"
#include "Math/Ray.h"

#include <PhysX/PxRigidActor.h>
#include <PhysX/PxScene.h>

namespace
{
	const str::Name strInput = str::Name::Create("DragSelection");
	const str::Name strSelect = str::Name::Create("DragSelection_Select");
}

void drag::SelectionSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	if (world.HasAny<ecs::query::Added<drag::IsSelectableComponent>>())
	{
		input::Layer layer;
		layer.m_Priority = eng::EInputPriority::Gameplay;
		layer.m_Bindings.Emplace(strSelect, input::EKey::Mouse_Left, false);

		auto& input = world.WriteResource<eng::InputManager>();
		input.AppendLayer(strInput, layer);
	}

	if (world.HasAny<ecs::query::Removed<drag::IsSelectableComponent>>())
	{
		auto& input = world.WriteResource<eng::InputManager>();
		input.RemoveLayer(strInput);
	}

	const auto& windowManager = world.ReadResource<eng::WindowManager>();
	const eng::Window* window = windowManager.GetWindow(0);
	if (!window)
		return;

	const auto& sceneComponent = world.ReadSingleton<eng::PhysicsSceneComponent>();
	auto& linesComponent = world.WriteSingleton<eng::LinesComponent>();

	for (const ecs::Entity& cameraEntity : world.Query<ecs::query::Include<const eng::camera::ProjectionComponent, const eng::TransformComponent>>())
	{
		const auto& cameraComponent = world.ReadComponent<eng::camera::ProjectionComponent>(cameraEntity);
		const auto& cameraTransform = world.ReadComponent<eng::TransformComponent>(cameraEntity);

		{
			const auto& input = world.ReadResource<eng::InputManager>();

			// mouse
			constexpr float distance = 100000.f;
			const Ray3f ray = eng::camera::ScreenToRay(
				cameraComponent,
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

					if (input.IsPressed(strSelect))
					{
						const ecs::Entity dragEntity = world.CreateEntity();
						auto& nameComponent = world.AddComponent<ecs::NameComponent>(dragEntity);
						nameComponent.m_Name = "Drag Entity";

						auto& dragComponent = world.AddComponent<drag::SelectionComponent>(dragEntity);
						dragComponent.m_CameraEntity = cameraEntity;
						dragComponent.m_SelectedEntity = selectedEntity;
						dragComponent.m_TranslatePlane = Plane3f(-ray.m_Direction, hitPosition);
						dragComponent.m_TranslateOffset = transformComponent.m_Translate - hitPosition;
					}
				}
			}
		}
	}

	for (const ecs::Entity& dragEntity : world.Query<ecs::query::Include<const drag::SelectionComponent>>())
	{
		const auto& input = world.ReadResource<eng::InputManager>();
		if (!input.IsHeld(strSelect))
			world.DestroyEntity(dragEntity);
	}
}