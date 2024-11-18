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
#include "GameClient/HexmapSettingsComponent.h"
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


	const auto& debugSettings = world.ReadSingleton<clt::settings::DebugComponent>();
	if (!debugSettings.m_IsHexmapEnabled)
		return;

	const auto& chart = world.ReadSingleton<hexmap::ChartComponent>();
	const auto& settings = world.ReadSingleton<hexmap::SettingsComponent>();

	// load
	for (const ecs::Entity& requestEntity : world.Query<ecs::query::Added<hexmap::RequestComponent>>())
	{
		const auto& request = world.ReadComponent<hexmap::RequestComponent>(requestEntity);

		for (const Hexagon& hex : request.m_Load)
		{
			const ecs::Entity entity = world.CreateEntity();
			auto& fragment = world.AddComponent<hexmap::FragmentComponent>(entity);
			fragment.m_Level = hex.m_Level;
			fragment.m_TileRadius = chart.m_TileRadius;
			fragment.m_HexPos = hex.m_Offset;

			// #temp:
			fragment.m_Sprite = ((hex.m_Offset.row ^ hex.m_Offset.col ^ hex.m_Level) % 3) == 0 ? strSpriteA : strSpriteB;

			const int32 count = settings.m_TileCount.x * settings.m_TileCount.y;
			fragment.m_Data.Resize(count);
			for (int32 i = 0; i < count; ++i)
				fragment.m_Data[i] = 0;

			auto& name = world.AddComponent<ecs::NameComponent>(entity);
			name.m_Name = std::format("Fragment: {}, {}", hex.m_Offset.row, hex.m_Offset.col);

			auto& transform = world.AddComponent<eng::TransformComponent>(entity);
			transform.m_Translate = hexagon::ToWorldPos(hex.m_Offset, chart.m_TileRadius).X0Y();
			transform.m_Rotate = Rotator(90.f, 0.f, 0.f);
		}

		for (const ecs::Entity& entity : world.Query<ecs::query::Include<eng::TransformComponent, hexmap::FragmentComponent>>())
		{
			const auto& fragment = world.ReadComponent<hexmap::FragmentComponent>(entity);
			const Hexagon hex = { fragment.m_Level, fragment.m_HexPos };
			if (request.m_Unload.Contains(hex))
				world.DestroyEntity(entity);
		}
	}

	// #temp: level + 1
	{
		auto& lines = world.WriteSingleton<eng::LinesComponent>();

		const float radiusMajor = chart.m_TileRadius * settings.m_TileRatio;
		const Vector2i min = hexagon::ToOffset(chart.m_Frustrum.m_Min, radiusMajor);
		const Vector2i max = hexagon::ToOffset(chart.m_Frustrum.m_Max, radiusMajor);
		for (const Vector2i& gridMajor : enumerate::Vector(min, max))
		{
			const hexagon::Offset hexPos = { gridMajor.x, gridMajor.y };
			const Vector2f worldPos = hexagon::ToWorldPos(hexPos, radiusMajor);
			lines.AddHexagon(worldPos.X0Y(), radiusMajor, Vector4f(1.f));
		}
	}
}