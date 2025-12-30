#include "TilemapPCH.h"
#include "Tilemap/TilemapDebugSystem.h"

#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/CameraComponent.h"
#include "Engine/CameraHelpers.h"
#include "Engine/InputManager.h"
#include "Engine/LinesComponent.h"
#include "Engine/SettingsDebugSingleton.h"
#include "Engine/TransformComponent.h"
#include "Engine/Window.h"
#include "Engine/WindowManager.h"
#include "Math/CollisionMath.h"
#include "Math/Plane.h"
#include "Math/Quaternion.h"
#include "Math/Ray.h"
#include "Math/VectorMath.h"
#include "Tilemap/TilemapAgentComponent.h"
#include "Tilemap/TilemapGridComponent.h"

void tilemap::DebugSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	const auto& debugSettings = world.ReadSingleton<eng::settings::DebugSingleton>();
	const auto& inputManager = world.ReadResource<eng::InputManager>();
	const auto& windowManager = world.ReadResource<eng::WindowManager>();
	const eng::Window* window = windowManager.GetWindow(0);
	if (!window)
		return;

	using CameraQuery = ecs::query
		::Include<
		const eng::ActiveComponent,
		const eng::CameraComponent,
		const eng::TransformComponent>;
	for (auto&& cameraView : world.Query<CameraQuery>())
	{
		const auto& cameraProjection = cameraView.ReadRequired<eng::CameraComponent>();
		const auto& cameraTransform = cameraView.ReadRequired<eng::TransformComponent>();

		using GridQuery = ecs::query
			::Include<
			const eng::TransformComponent,
			const tilemap::GridComponent>;
		for (auto&& gridView : world.Query<GridQuery>())
		{
			const auto& gridComponent = gridView.ReadRequired<tilemap::GridComponent>();
			const auto& gridTransform = gridView.ReadRequired<eng::TransformComponent>();
			const Quaternion rotate = Quaternion::FromRotator(gridTransform.m_Rotate);
			const Vector3f normal = Vector3f::AxisZ * rotate;

			const Plane3f plane = Plane3f(normal, gridTransform.m_Translate);
			const Ray3f ray = eng::ScreenToRay(
				cameraProjection,
				cameraTransform,
				*window,
				inputManager.m_MousePosition);

			Vector3f intersectPos;
			if (math::Intersection(ray, plane, intersectPos))
			{
				const Vector3f& tileSize = gridComponent.m_TileSize;
				const Vector3i tilePos = math::ToGridPos(intersectPos, tileSize);
				const Vector3f snapPos = math::ToWorldPos(tilePos, tileSize);

				auto& lines = world.WriteSingleton<eng::LinesSingleton>();
				lines.AddAABB(snapPos, tileSize * 0.5f, Colour::Magenta);
			}
		}
	}
}
