#include "GameClientPCH.h"
#include "GameClient/HexmapLoadSystem.h"

#include "Core/Algorithms.h"
#include "Core/Colour.h"
#include "ECS/EntityWorld.h"
#include "ECS/NameComponent.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/LevelComponents.h"
#include "Engine/TransformComponent.h"
#include "GameClient/HexmapHelpers.h"
#include "GameClient/HexmapLayerComponent.h"
#include "GameClient/HexmapRootComponent.h"
#include "Math/Algorithms.h"
#include "Math/CollisionMath.h"
#include "Math/Hexagon.h"
#include "Math/HexagonHelpers.h"

void hexmap::LoadSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	struct LayerInfo
	{
		bool operator<(const LayerInfo& rhs) const 
		{
			if (m_Depth != rhs.m_Depth)
				return m_Depth < rhs.m_Depth;
			return m_LayerPos < rhs.m_LayerPos;
		}

		int32 m_Depth = 0;
		LayerPos m_LayerPos = {};
	};

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

		Set<LayerInfo> inRange;
		const LayerPos layerMin = ToLayerPos(hexMin, root.m_HexCount) - LayerPos(1);
		const LayerPos layerMax = ToLayerPos(hexMax, root.m_HexCount) + LayerPos(1);
		for (const Vector2i& gridPos : enumerate::Vector(layerMin, layerMax))
		{
			const LayerPos layerPos = LayerPos(gridPos);
			inRange.Add({ root.m_Depth, layerPos });
		}

		Set<LayerInfo> loaded;
		for (const ecs::Entity& layerEntity : world.Query<ecs::query::Include<eng::TransformComponent, hexmap::LayerComponent>>())
		{
			const auto& transform = world.ReadComponent<eng::TransformComponent>(layerEntity);
			const auto& layer = world.ReadComponent<hexmap::LayerComponent>(layerEntity);

			LayerInfo info = { layer.m_Depth, layer.m_Origin };
			if (inRange.Contains(info))
			{
				loaded.Add(info);
			}
			else
			{
				world.DestroyEntity(layerEntity);
			}
		}

		Set<LayerInfo> toCreate;
		enumerate::Difference(inRange, loaded, toCreate);
		for (const LayerInfo& info : toCreate)
		{
			const int32 depth = root.m_Depth;
			const HexPos hexPos = ToHexPos(info.m_LayerPos, root.m_HexCount);
			const Vector3f worldPos = hexagon::ToWorldPos(hexPos, root.m_HexRadius).X0Y();

			const ecs::Entity entity = world.CreateEntity();
			auto& layer = world.AddComponent<hexmap::LayerComponent>(entity);
			layer.m_Depth = info.m_Depth;
			layer.m_Origin = info.m_LayerPos;
			layer.m_Root = rootEntity;
			layer.m_HexCount = root.m_HexCount;

			const int32 count = root.m_HexCount.x * root.m_HexCount.y;
			layer.m_HexData.Resize(count);
			for (int32 i = 0; i < count; ++i)
				layer.m_HexData[i] = { 0 };

			auto& name = world.AddComponent<ecs::NameComponent>(entity);
			name.m_Name = std::format("Fragment: {} - {}, {}", depth, info.m_LayerPos.x, info.m_LayerPos.y);

			auto& transform = world.AddComponent<eng::TransformComponent>(entity);
			transform.m_Translate = worldPos;
			transform.m_Rotate = Rotator(90.f, 0.f, 0.f);

			world.AddComponent<eng::level::EntityComponent>(entity, level.m_Name);
		}
	}

	//// #temp: level - 1
	//if (false)
	//{
	//	auto& lines = world.WriteSingleton<eng::LinesComponent>();

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
	//	auto& lines = world.WriteSingleton<eng::LinesComponent>();

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