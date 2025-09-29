#include "TilemapPCH.h"
#include "Tilemap/TilemapDebugSystem.h"

#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/CameraComponent.h"
#include "Engine/CameraHelpers.h"
#include "Engine/InputManager.h"
#include "Engine/TransformComponent.h"
#include "Engine/Window.h"
#include "Engine/WindowManager.h"
#include "Tilemap/TilemapAgentComponent.h"

void tilemap::DebugSystem::Update(World& world, const GameTime& gameTime)
{
	const auto& inputManager = world.ReadResource<eng::InputManager>();
	const auto& windowManager = world.ReadResource<eng::WindowManager>();
	const eng::Window* window = windowManager.GetWindow(0);
	if (!window)
		return;

	using CameraQuery = ecs::query::Include<
		const eng::camera::ProjectionComponent,
		const eng::TransformComponent>;
	for (const ecs::Entity& cameraEntity : world.Query<CameraQuery>())
	{
		const auto& cameraProjection = world.ReadComponent<eng::camera::ProjectionComponent>(cameraEntity);
		const auto& cameraTransform = world.ReadComponent<eng::TransformComponent>(cameraEntity);
		const Ray3f ray = eng::camera::ScreenToRay(
			cameraProjection, 
			cameraTransform, 
			*window, 
			inputManager.m_MousePosition);
	}
}
