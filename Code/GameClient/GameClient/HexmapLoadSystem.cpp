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
	auto& lines = world.WriteSingleton<eng::LinesComponent>();

	// load
	for (const ecs::Entity& requestEntity : world.Query<ecs::query::Added<hexmap::RequestComponent>>())
	{
		const auto& request = world.ReadComponent<hexmap::RequestComponent>(requestEntity);

		const float fragmentRadius = settings.GetFragmentRadius();
		for (const hexagon::Offset& hexPos : request.m_Load)
		{
			const ecs::Entity entity = world.CreateEntity();
			auto& fragment = world.AddComponent<hexmap::FragmentComponent>(entity);
			fragment.m_HexPos = hexPos;

			// #temp:
			fragment.m_Sprite = (hexPos.row % 2) == 0 ? strSpriteA : strSpriteB;

			const int32 count = settings.m_TileCount * settings.m_TileCount;
			fragment.m_Data.Resize(count);
			for (int32 i = 0; i < count; ++i)
				fragment.m_Data[i] = 0;

			auto& name = world.AddComponent<ecs::NameComponent>(entity);
			name.m_Name = std::format("Fragment: {}, {}", hexPos.row, hexPos.col);

			auto& transform = world.AddComponent<eng::TransformComponent>(entity);
			transform.m_Translate = hexagon::ToWorldPos(hexPos, fragmentRadius).X0Y();
			transform.m_Rotate = Rotator(90.f, 0.f, 0.f);
		}

		for (const ecs::Entity& entity : world.Query<ecs::query::Include<eng::TransformComponent, hexmap::FragmentComponent>>())
		{
			const auto& fragment = world.ReadComponent<hexmap::FragmentComponent>(entity);
			if (request.m_Unload.Contains(fragment.m_HexPos))
				world.DestroyEntity(entity);
		}
	}

	// #temp: draw extents
	{
		const float fragmentRadius = settings.GetFragmentRadius();
		const Vector2f fragmentExtents = Vector2f(fragmentRadius);

		for (const ecs::Entity& entity : world.Query<ecs::query::Include<eng::TransformComponent, hexmap::FragmentComponent>>())
		{
			const auto& fragment = world.ReadComponent<hexmap::FragmentComponent>(entity);
			const auto& transform = world.ReadComponent<eng::TransformComponent>(entity);

			const AABB3f fragmentAABB = AABB3f::FromExtents(fragmentExtents.X0Y());
			lines.AddAABB(transform.m_Translate, fragmentAABB, Vector4f(1.f));
			lines.AddSphere(transform.m_Translate, 100.f, Vector4f(1.f));
		}
	}
}