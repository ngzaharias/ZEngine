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
	constexpr float s_HexagonRatio = SQUARE_ROOT_THREE / 2.f;

	const str::Guid strSpriteA = str::Guid::Create("686c500eca54494785f189dc49016a4c");
	const str::Guid strSpriteB = str::Guid::Create("173e2734a8454e13abec1cd18d00caf2");
}

void hexmap::LoadSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	auto& lines = world.WriteSingleton<eng::LinesComponent>();

	//for (int32 x = 0; x < 3; ++x)
	//{
	//	for (int32 y = 0; y < 3; ++y)
	//	{
	//		constexpr float tileRadius = 100.f;
	//		constexpr Vector2f tileRatio = Vector2f(1.f, s_HexagonRatio);
	//		constexpr Vector2f tileSize = math::Multiply(Vector2f(tileRadius * 2.f), tileRatio);
	//		constexpr Vector2i tileCount = Vector2i(5, 3);

	//		constexpr Vector2f tileOffset = math::Multiply(Vector2f(tileRadius), tileRatio);

	//		const hexagon::Offset fragmentHex = { tileCount.x * x, tileCount.y * y };
	//		const Vector2f fragmentSize = hexagon::ToAABB(tileCount, tileRadius).m_Max;
	//		const Vector2f fragmentPos = hexagon::ToWorldPos(fragmentHex, tileRadius);
	//		const Vector2f fragmentMin = fragmentPos;
	//		const Vector2f fragmentMax = fragmentPos + fragmentSize;

	//		const AABB3f fragmentAABB = AABB3f(fragmentMin.X0Y(), fragmentMax.X0Y());
	//		lines.AddAABB(Vector3f::Zero, fragmentAABB, Vector4f(1.f, 0.f, 0.f, 1.f));

	//		for (const Vector2i& gridPos : enumerate::Vector(tileCount))
	//		{
	//			const hexagon::Offset tileHex = { gridPos.x, gridPos.y };
	//			const Vector2f tilePos = tileOffset + hexagon::ToWorldPos(tileHex, tileRadius);
	//			const Vector3f worldPos = (fragmentPos + tilePos).X0Y();

	//			lines.AddHexagon(worldPos, tileRadius, Vector4f(0.f, 1.f, 0.f, 1.f));
	//		}
	//	}
	//}

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

	//// #temp: extents
	//{
	//	for (const ecs::Entity& entity : world.Query<ecs::query::Include<eng::TransformComponent, hexmap::FragmentComponent>>())
	//	{
	//		const auto& fragment = world.ReadComponent<hexmap::FragmentComponent>(entity);
	//		const auto& transform = world.ReadComponent<eng::TransformComponent>(entity);

	//		const AABB2f fragmentAABB = hexagon::ToAABB(settings.m_TileCount, settings.m_TileRadius);
	//		const AABB3f worldAABB = AABB3f(fragmentAABB.m_Min.X0Y(), fragmentAABB.m_Max.X0Y());
	//		lines.AddAABB(transform.m_Translate, worldAABB, Vector4f(1.f));
	//		lines.AddSphere(transform.m_Translate, 100.f, Vector4f(1.f));
	//	}
	//}
}