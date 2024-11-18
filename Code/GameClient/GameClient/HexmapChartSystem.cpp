#include "GameClientPCH.h"
#include "GameClient/HexmapChartSystem.h"

#include "Core/Algorithms.h"
#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/CameraComponent.h"
#include "Engine/CameraHelpers.h"
#include "Engine/InputComponent.h"
#include "Engine/Screen.h"
#include "Engine/TransformComponent.h"
#include "GameClient/HexmapChartComponent.h"
#include "GameClient/HexmapRequestComponent.h"
#include "GameClient/HexmapSettingsComponent.h"
#include "Math/AABB.h"
#include "Math/Algorithms.h"
#include "Math/CollisionMath.h"
#include "Math/Hexagon.h"
#include "Math/HexagonHelpers.h"
#include "Math/Plane.h"
#include "Math/Vector.h"

namespace
{
	AABB2f GetCameraZone(const eng::camera::ProjectionComponent& camera, const eng::TransformComponent& transform)
	{
		constexpr Plane3f plane = Plane3f(Vector3f::AxisY, Vector3f::Zero);

		const Quaternion cameraRotate = Quaternion::FromRotator(transform.m_Rotate);
		const Vector3f cameraForward = Vector3f::AxisZ * cameraRotate;

		const Vector2f screenCentre = Vector2f(Screen::width, Screen::height) * 0.5f;
		const Vector2f pixelPosA = screenCentre - screenCentre;
		const Vector2f pixelPosB = screenCentre + screenCentre;

		const Matrix4x4 cameraView = Matrix4x4::FromTransform(transform.m_Translate, Quaternion::FromRotator(Rotator(90.f, 0.f, 0.f)), 1.f);
		const Vector3f worldPosA = eng::camera::ScreenToWorld(pixelPosA, camera.m_Projection, cameraView);
		const Vector3f worldPosB = eng::camera::ScreenToWorld(pixelPosB, camera.m_Projection, cameraView);

		Vector3f forwardA = cameraForward;
		Vector3f forwardB = cameraForward;
		if (!std::holds_alternative<eng::camera::Orthographic>(camera.m_Projection))
		{
			forwardA = (worldPosA - transform.m_Translate).Normalized();
			forwardB = (worldPosB - transform.m_Translate).Normalized();
		}

		Vector3f intersectA, intersectB;
		const Ray3f rayA = Ray3f(worldPosA, forwardA);
		const Ray3f rayB = Ray3f(worldPosB, forwardB);
		if (math::Intersection(rayA, plane, intersectA) && math::Intersection(rayB, plane, intersectB))
		{
			AABB2f aabb;
			aabb.m_Min.x = math::Min(intersectA.x, intersectB.x);
			aabb.m_Min.y = math::Min(intersectA.z, intersectB.z);
			aabb.m_Max.x = math::Max(intersectA.x, intersectB.x);
			aabb.m_Max.y = math::Max(intersectA.z, intersectB.z);
			return aabb;
		}
		return {};
	}
}

void hexmap::ChartSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	const auto& settings = world.ReadSingleton<hexmap::SettingsComponent>();

	bool hasChanged = false;
	for (const ecs::Entity& inputEntity : world.Query<ecs::query::Include<eng::InputComponent>>())
	{
		const auto& input = world.ReadComponent<eng::InputComponent>(inputEntity);
		if (input.m_ScrollDelta.y == 0)
			continue;

		hasChanged = true;
		auto& chart = world.WriteSingleton<hexmap::ChartComponent>();
		chart.m_Level -= input.m_ScrollDelta.y * 0.1f;
	}

	const bool cameraAdded = world.HasAny<ecs::query::Added<eng::camera::ProjectionComponent>::Include<eng::TransformComponent>>();
	const bool cameraChanged = world.HasAny<ecs::query::Updated<eng::camera::ProjectionComponent>::Include<eng::TransformComponent>>();
	const bool transformAdded = world.HasAny<ecs::query::Added<eng::TransformComponent>::Include<eng::camera::ProjectionComponent>>();
	const bool transformChanged = world.HasAny<ecs::query::Updated<eng::TransformComponent>::Include<eng::camera::ProjectionComponent>>();
	if (hasChanged || cameraAdded || cameraChanged || transformAdded || transformChanged)
	{
		auto& chart = world.WriteSingleton<hexmap::ChartComponent>();
		const int32 level = math::Floor<int32>(chart.m_Level);

		for (const ecs::Entity& entity : world.Query<ecs::query::Include<eng::camera::ProjectionComponent, eng::TransformComponent>>())
		{
			auto& camera = world.WriteComponent<eng::camera::ProjectionComponent>(entity);
			if (std::holds_alternative<eng::camera::Orthographic>(camera.m_Projection))
			{
				const float t = chart.m_Level - math::Floor(chart.m_Level);
				auto& projection = std::get<eng::camera::Orthographic>(camera.m_Projection);
				projection.m_Size = math::Lerp(1000.f, 12000.f, t);
			}

			const auto& transform = world.ReadComponent<eng::TransformComponent>(entity);
			chart.m_Frustrum = GetCameraZone(camera, transform);
		}

		Set<Hexagon> load;
		Set<Hexagon> unload;
		Set<Hexagon> inrange;

		Vector2i min = hexagon::ToOffset(chart.m_Frustrum.m_Min, chart.m_TileRadius);
		Vector2i max = hexagon::ToOffset(chart.m_Frustrum.m_Max, chart.m_TileRadius);
		min = math::Divide(min, settings.m_TileCount) - Vector2i(2);
		max = math::Divide(max, settings.m_TileCount) + Vector2i(2);

		for (const Vector2i& gridMajor : enumerate::Vector(min, max))
		{
			const Vector2i gridMinor = math::Multiply(gridMajor, settings.m_TileCount);
			const hexagon::Offset hexPos = { gridMinor.x, gridMinor.y };
			const Vector2f worldPos = hexagon::ToWorldPos(hexPos, chart.m_TileRadius);
			
			AABB2f fragmentAABB = hexagon::ToAABB(settings.m_TileCount, chart.m_TileRadius);
			fragmentAABB.m_Min += worldPos;
			fragmentAABB.m_Max += worldPos;

			if (math::IsOverlapping(chart.m_Frustrum, fragmentAABB))
				inrange.Add({ level, hexPos });
		}

		enumerate::Difference(inrange, chart.m_Loaded, load);
		enumerate::Difference(chart.m_Loaded, inrange, unload);
		if (!load.IsEmpty() || !unload.IsEmpty())
		{
			auto& request = world.AddEventComponent<hexmap::RequestComponent>();
			request.m_Load = std::move(load);
			request.m_Unload = std::move(unload);
		}
		chart.m_Loaded = std::move(inrange);
	}
}