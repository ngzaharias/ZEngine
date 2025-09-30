#include "TilemapPCH.h"
#include "Tilemap/TilemapDebugSystem.h"

#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/CameraComponent.h"
#include "Engine/CameraHelpers.h"
#include "Engine/InputManager.h"
#include "Engine/LinesComponent.h"
#include "Engine/SettingsComponents.h"
#include "Engine/TransformComponent.h"
#include "Engine/Window.h"
#include "Engine/WindowManager.h"
#include "Math/CollisionMath.h"
#include "Math/Plane.h"
#include "Math/Ray.h"
#include "Math/VectorMath.h"
#include "Tilemap/TilemapAgentComponent.h"

void tilemap::DebugSystem::Update(World& world, const GameTime& gameTime)
{
	const auto& debugSettings = world.ReadSingleton<eng::settings::DebugComponent>();
	const auto& inputManager = world.ReadResource<eng::InputManager>();
	const auto& windowManager = world.ReadResource<eng::WindowManager>();
	const eng::Window* window = windowManager.GetWindow(0);
	if (!window)
		return;

	using CameraQuery = ecs::query
		::Include<
		const eng::camera::ProjectionComponent,
		const eng::TransformComponent>;
	for (const ecs::Entity& cameraEntity : world.Query<CameraQuery>())
	{
		const bool isEditorActive = debugSettings.m_IsEditorModeEnabled;
		const bool isEditorCamera = world.HasComponent<eng::camera::EditorComponent>(cameraEntity);
		if (isEditorActive != isEditorCamera)
			continue;

		const auto& cameraProjection = world.ReadComponent<eng::camera::ProjectionComponent>(cameraEntity);
		const auto& cameraTransform = world.ReadComponent<eng::TransformComponent>(cameraEntity);

		const Plane3f plane = Plane3f(Vector3f::AxisZ, Vector3f::Zero);
		const Ray3f ray = eng::camera::ScreenToRay(
			cameraProjection, 
			cameraTransform, 
			*window, 
			inputManager.m_MousePosition);

		Vector3f intersectPos;
		if (math::Intersection(ray, plane, intersectPos))
		{
			constexpr Vector3f cellSize = Vector3f(100.f);
			const Vector3i tilePos = math::ToGridPos(intersectPos, cellSize);
			const Vector3f snapPos = math::ToWorldPos(tilePos, cellSize);

			auto& lines = world.WriteSingleton<eng::LinesComponent>();
			lines.AddAABB(snapPos, cellSize * 0.5f, Colour::White);
		}
	}
}
