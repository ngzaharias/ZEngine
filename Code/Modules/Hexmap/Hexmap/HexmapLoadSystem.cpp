#include "HexmapPCH.h"
#include "Hexmap/HexmapLoadSystem.h"

#include "Core/Algorithms.h"
#include "Core/Colour.h"
#include "ECS/EntityWorld.h"
#include "ECS/NameComponent.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/LevelEntityComponent.h"
#include "Engine/TransformComponent.h"
#include "Hexmap/HexmapHelpers.h"
#include "Hexmap/HexmapLayerComponent.h"
#include "Hexmap/HexmapRootComponent.h"
#include "Math/Algorithms.h"
#include "Math/CollisionMath.h"
#include "Math/Hexagon.h"
#include "Math/HexagonHelpers.h"

void hexmap::LoadSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	// load
	using RootQuery = ecs::query
		::Updated<hexmap::RootComponent>
		::Include<eng::level::EntityComponent>;
	for (const ecs::Entity& rootEntity : world.Query<ecs::query::Updated<hexmap::RootComponent>>())
	{
		const auto& root = world.ReadComponent<hexmap::RootComponent>(rootEntity);
		const auto& level = world.ReadComponent<eng::level::EntityComponent>(rootEntity);

		const HexPos hexMin = hexagon::ToOffset(root.m_Zone.m_Min, root.m_HexRadius);
		const HexPos hexMax = hexagon::ToOffset(root.m_Zone.m_Max, root.m_HexRadius);

		Set<LayerPos> inRange;
		const LayerPos layerMin = ToLayerPos(hexMin, root.m_HexCount) - LayerPos(1);
		const LayerPos layerMax = ToLayerPos(hexMax, root.m_HexCount) + LayerPos(1);
		for (const Vector2i& gridPos : enumerate::Vector(layerMin, layerMax))
		{
			const LayerPos layerPos = LayerPos(gridPos);
			inRange.Add(layerPos);
		}

		Set<LayerPos> loaded;
		for (const ecs::Entity& layerEntity : world.Query<ecs::query::Include<eng::TransformComponent, hexmap::LayerComponent>>())
		{
			const auto& transform = world.ReadComponent<eng::TransformComponent>(layerEntity);
			const auto& layer = world.ReadComponent<hexmap::LayerComponent>(layerEntity);

			if (inRange.Contains(layer.m_Origin))
			{
				loaded.Add(layer.m_Origin);
			}
			else
			{
				world.DestroyEntity(layerEntity);
			}
		}

		Set<LayerPos> toCreate;
		enumerate::Difference(inRange, loaded, toCreate);
		for (const LayerPos& layerPos : toCreate)
		{
			const HexPos hexPos = ToHexPos(layerPos, root.m_HexCount);
			const Vector3f worldPos = hexagon::ToWorldPos(hexPos, root.m_HexRadius).X0Y();

			const ecs::Entity entity = world.CreateEntity();
			auto& layer = world.AddComponent<hexmap::LayerComponent>(entity);
			layer.m_Origin = layerPos;
			layer.m_Root = rootEntity;
			layer.m_HexCount = root.m_HexCount;

			const int32 count = root.m_HexCount.x * root.m_HexCount.y;
			layer.m_HexData.Resize(count);
			for (int32 i = 0; i < count; ++i)
				layer.m_HexData[i] = { 0 };

			auto& name = world.AddComponent<ecs::NameComponent>(entity);
			name.m_Name = std::format("Fragment: {}, {}", layerPos.x, layerPos.y);

			auto& transform = world.AddComponent<eng::TransformComponent>(entity);
			transform.m_Translate = worldPos;
			transform.m_Rotate = Rotator(90.f, 0.f, 0.f);

			world.AddComponent<eng::level::EntityComponent>(entity, level.m_Name);
		}
	}

	//// #temp: level - 1
	//if (false)
	//{
	//	auto& lines = world.WriteSingleton<eng::LinesSingleton>();

	//	const float radiusMinor = chart.m_HexRadius / settings.m_TileRatio;
	//	const Vector2i min = hexagon::ToOffset(chart.m_Frustrum.m_Min, radiusMinor);
	//	const Vector2i max = hexagon::ToOffset(chart.m_Frustrum.m_Max, radiusMinor);
	//	for (const Vector2i& gridMinor : enumerate::Vector(min, max))
	//	{
	//		const hexagon::Offset hexPos = { gridMinor.x, gridMinor.y };
	//		const Vector2f worldPos = hexagon::ToWorldPos(hexPos, radiusMinor);
	//		lines.AddHexagon(worldPos.X0Y(), radiusMinor, Colour::Black);
	//	}
	//}

	//// #temp: level + 1
	//{
	//	auto& lines = world.WriteSingleton<eng::LinesSingleton>();

	//	const float radiusMajor = chart.m_HexRadius * settings.m_TileRatio;
	//	const Vector2i min = hexagon::ToOffset(chart.m_Frustrum.m_Min, radiusMajor);
	//	const Vector2i max = hexagon::ToOffset(chart.m_Frustrum.m_Max, radiusMajor);
	//	for (const Vector2i& gridMajor : enumerate::Vector(min, max + Vector2i::One))
	//	{
	//		const hexagon::Offset hexPos = { gridMajor.x, gridMajor.y };
	//		const Vector2f worldPos = hexagon::ToWorldPos(hexPos, radiusMajor);
	//		lines.AddHexagon(worldPos.X0Y(), radiusMajor, Colour::Black);
	//	}
	//}
}