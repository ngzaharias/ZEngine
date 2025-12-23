#include "TacticsPCH.h"
#include "Tactics/TacticsSelectSystem.h"

#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/CameraHelpers.h"
#include "Engine/CameraProjectionComponent.h"
#include "Engine/InputManager.h"
#include "Engine/PhysicsSceneComponent.h"
#include "Engine/SettingsDebugSingleton.h"
#include "Engine/TransformComponent.h"
#include "Engine/Window.h"
#include "Engine/WindowManager.h"
#include "Math/Ray.h"
#include "Tactics/TacticsInput.h"
#include "Tactics/TacticsSelectedComponent.h"
#include "Tilemap/TilemapAgentComponent.h"

#include <PhysX/PxRigidActor.h>
#include <PhysX/PxScene.h>

namespace
{
	ecs::Entity ToEntity(const physx::PxRigidActor* actor)
	{
		return reinterpret_cast<uint64>(actor->userData);
	}
}

void tactics::SelectSystem::Update(World& world, const GameTime& gameTime)
{
	const auto& debugSettings = world.ReadSingleton<eng::settings::DebugSingleton>();
	if (debugSettings.m_IsEditorModeEnabled)
		return;

	const auto& inputManager = world.ReadResource<eng::InputManager>();
	const auto& physicsScene = world.ReadSingleton<eng::PhysicsSceneSingleton>();
	const auto& windowManager = world.ReadResource<eng::WindowManager>();
	const eng::Window* window = windowManager.GetWindow(0);
	if (!window)
		return;

	using CameraQuery = ecs::query
		::Include<
		const eng::camera::ProjectionComponent,
		const eng::TransformComponent>
		::Exclude<
		const eng::camera::EditorComponent>;
	for (auto&& cameraView : world.Query<CameraQuery>())
	{
		if (!inputManager.IsPressed(strSelect))
			continue;

		const auto& cameraProjection = cameraView.ReadRequired<eng::camera::ProjectionComponent>();
		const auto& cameraTransform = cameraView.ReadRequired<eng::TransformComponent>();

		// mouse
		constexpr float distance = 100000.f;
		const Ray3f ray = eng::camera::ScreenToRay(
			cameraProjection,
			cameraTransform,
			*window,
			inputManager.m_MousePosition);

		const physx::PxVec3 position = {
			ray.m_Position.x,
			ray.m_Position.y,
			ray.m_Position.z };
		const physx::PxVec3 direction = {
			ray.m_Direction.x,
			ray.m_Direction.y,
			ray.m_Direction.z };

		ecs::Entity agentEntity;
		physx::PxRaycastBuffer hitcall;
		physicsScene.m_PhysicsScene->raycast(position, direction, distance, hitcall);
		if (hitcall.hasAnyHits())
		{
			const physx::PxRaycastHit& closestHit = hitcall.block;
			const ecs::Entity hitEntity = ToEntity(hitcall.block.actor);
			if (world.HasComponent<tilemap::AgentComponent>(hitEntity))
				agentEntity = hitEntity;
		}

		if (agentEntity && !world.HasComponent<tactics::SelectedComponent>(agentEntity))
			world.AddComponent<tactics::SelectedComponent>(agentEntity);

		// remove previous selection
		for (auto&& selectedView : world.Query<ecs::query::Include<const tactics::SelectedComponent>>())
		{
			if (selectedView != agentEntity)
				world.RemoveComponent<tactics::SelectedComponent>(selectedView);
		}
	}
}
