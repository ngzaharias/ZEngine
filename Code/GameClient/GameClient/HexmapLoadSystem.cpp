#include "GameClientPCH.h"
#include "GameClient/HexmapLoadSystem.h"

#include "ECS/EntityWorld.h"
#include "ECS/NameComponent.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/ColourHelpers.h"
#include "Engine/InputComponent.h"
#include "Engine/LinesComponent.h"
#include "Engine/StaticMeshComponent.h"
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

namespace
{
	const str::Guid strSpriteA = str::Guid::Create("686c500eca54494785f189dc49016a4c");
	const str::Guid strSpriteB = str::Guid::Create("173e2734a8454e13abec1cd18d00caf2");
}

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

			const float ratio = static_cast<float>(chart.m_Ratio);
			const ecs::Entity entity = world.CreateEntity();
			auto& fragment = world.AddComponent<hexmap::FragmentComponent>(entity);
			fragment.m_Level = math::Floor<int32>(chart.m_Level);
			fragment.m_Position = gridPos;
			fragment.m_Radius = chart.GetRadiusMin();

			// #temp:
			fragment.m_Sprite = (j % 5) == 0 ? strSpriteA : strSpriteB;

			const Vector2i rangeMin = -Vector2i(math::Floor<int32>(ratio * 0.5f));
			const Vector2i rangeMax = +Vector2i(math::Ceiling<int32>(ratio * 0.5f));
			fragment.m_Dimensions.x = rangeMax.x - rangeMin.x;
			fragment.m_Dimensions.y = rangeMax.y - rangeMin.y;

			const int32 count = fragment.m_Dimensions.x * fragment.m_Dimensions.y;
			fragment.m_Data.Resize(count);
			for (int32 i = 0; i < count; ++i)
				fragment.m_Data[i] = j;

			auto& name = world.AddComponent<ecs::NameComponent>(entity);
			name.m_Name = std::format("Fragment: {}, {}", itr.x, itr.y);

			auto& transform = world.AddComponent<eng::TransformComponent>(entity);
			transform.m_Translate = hexagon::ToWorldPos(gridPos, radiusMax).X0Y();
			transform.m_Rotate = Rotator(90.f, 0.f, 0.f);

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
			lines.AddHexagon(worldPos + Vector3f::AxisY, radiusMax, Vector4f(1.f));
		}
	}
}