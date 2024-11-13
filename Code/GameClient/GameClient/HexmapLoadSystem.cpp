#include "GameClientPCH.h"
#include "GameClient/HexmapLoadSystem.h"

#include "ECS/EntityWorld.h"
#include "ECS/NameComponent.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/ColourHelpers.h"
#include "Engine/InputComponent.h"
#include "Engine/LinesComponent.h"
#include "Engine/TransformComponent.h"
#include "GameClient/HexmapChartComponent.h"
#include "GameClient/HexmapFragmentComponent.h"
#include "GameClient/HexmapRequestComponent.h"
#include "GameClient/SettingsComponents.h"
#include "Math/Algorithms.h"
#include "Math/CollisionMath.h"
#include "Math/HexagonHelpers.h"
#include "Math/Math.h"
#include "Math/Matrix.h"
#include "Math/Sphere.h"
#include "Math/Vector.h"

void hexmap::LoadSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	const auto& settings = world.ReadSingleton<clt::settings::DebugComponent>();
	if (!settings.m_IsHexmapEnabled)
		return;

	const auto& chart = world.ReadSingleton<hexmap::ChartComponent>();
	auto& lines = world.WriteSingleton<eng::LinesComponent>();

	// load
	for (const ecs::Entity& requestEntity : world.Query<ecs::query::Added<hexmap::RequestComponent>>())
	{
		const auto& request = world.ReadComponent<hexmap::RequestComponent>(requestEntity);

		int32 j = 0;
		const float radiusMin = chart.GetRadiusMin();
		const float radiusMax = chart.GetRadiusMax();
		const Vector2i min = hexagon::ToOffset(chart.m_Zone.m_Min, radiusMax);
		const Vector2i max = hexagon::ToOffset(chart.m_Zone.m_Max, radiusMax);
		for (const Vector2i& itr : enumerate::Vector(min - Vector2i(1), max + Vector2i(2)))
		{
			const hexagon::Offset gridPos = hexagon::Offset{ itr.x, itr.y };
			const Vector2f worldPos = hexagon::ToWorldPos(gridPos, radiusMax);
			const AABB2f hexSize = AABB2f::FromExtents(worldPos, Vector2f(radiusMax));
			if (math::IsOverlapping(request.m_ZoneOld, hexSize))
				continue;
			if (!math::IsOverlapping(request.m_ZoneNew, hexSize))
				continue;

			const ecs::Entity entity = world.CreateEntity();
			auto& fragment = world.AddComponent<hexmap::FragmentComponent>(entity);
			fragment.m_Level = math::Floor<int32>(chart.m_Level);
			fragment.m_Position = gridPos;
			fragment.m_Range.m_Min = -Vector2i(chart.m_Ratio / 2);
			fragment.m_Range.m_Max = +Vector2i(chart.m_Ratio / 2);

			const int32 width = fragment.m_Range.m_Max.x - fragment.m_Range.m_Min.x;
			const int32 height = fragment.m_Range.m_Max.y - fragment.m_Range.m_Min.y;
			const int32 count = width * height;
			fragment.m_Data.Resize(count);
			for (int32 i = 0; i < count; ++i)
				fragment.m_Data[i] = j;

			auto& name = world.AddComponent<ecs::NameComponent>(entity);
			name.m_Name = std::format("Fragment: {}, {}", itr.x, itr.y);

			auto& transform = world.AddComponent<eng::TransformComponent>(entity);
			transform.m_Translate = hexagon::ToWorldPos(gridPos, radiusMax).X0Y();

			j++;
		}
	}

	// unload
	if (world.HasAny<ecs::query::Updated<hexmap::ChartComponent>>())
	{
		for (const ecs::Entity& entity : world.Query<ecs::query::Include<eng::TransformComponent, hexmap::FragmentComponent>>())
		{
			const auto& transform = world.ReadComponent<eng::TransformComponent>(entity);

			const float radiusMin = chart.GetRadiusMin();
			const float radiusMax = chart.GetRadiusMax();
			const AABB2f hexSize = AABB2f::FromExtents(transform.m_Translate.XZ(), Vector2f(radiusMax));
			if (!math::IsOverlapping(chart.m_Zone, hexSize))
				world.DestroyEntity(entity);
		}

	}

	// #temp: draw major
	{
		const float radiusMin = chart.GetRadiusMin();
		const float radiusMax = chart.GetRadiusMax();

		const Vector2f worldMin = math::Min(chart.m_Zone.m_Min, chart.m_Zone.m_Max);
		const Vector2f worldMax = math::Max(chart.m_Zone.m_Min, chart.m_Zone.m_Max);

		const Vector2i gridMin = hexagon::ToOffset(worldMin, radiusMax);
		const Vector2i gridMax = hexagon::ToOffset(worldMax, radiusMax);
		for (const Vector2i& itr : enumerate::Vector(gridMin - Vector2i(1), gridMax + Vector2i(2)))
		{
			const hexagon::Offset gridPos = hexagon::Offset{ itr.x, itr.y };
			const Vector3f worldPos = hexagon::ToWorldPos(gridPos, radiusMax).X0Y();
			lines.AddHexagon(worldPos, radiusMax, Vector4f(1.f));
		}
	}

	// #temp: draw minor
	{
		const float radiusMin = chart.GetRadiusMin();
		const float radiusMax = chart.GetRadiusMax();
		for (const ecs::Entity& entity : world.Query<ecs::query::Include<eng::TransformComponent, hexmap::FragmentComponent>>())
		{
			const auto& fragment = world.ReadComponent<hexmap::FragmentComponent>(entity);
			const auto& transform = world.ReadComponent<eng::TransformComponent>(entity);

			int32 i = 0;
			const int32 count = fragment.m_Data.GetCount();
			for (const Vector2i& itr : enumerate::Vector(fragment.m_Range.m_Min, fragment.m_Range.m_Max))
			{
				const hexagon::Offset gridPos = { itr.x, itr.y };
				const Vector4f& colour = colour::From(fragment.m_Data[i]);

				const Vector2f localPos = hexagon::ToWorldPos(gridPos, radiusMin);
				lines.AddHexagon(transform.m_Translate + localPos.X0Y(), radiusMin, colour);
				i++;
			}
		}
	}
}