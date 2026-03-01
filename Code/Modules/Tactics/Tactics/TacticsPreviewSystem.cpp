#include "TacticsPCH.h"
#include "Tactics/TacticsPreviewSystem.h"

#include "Core/Algorithms.h"
#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/LinesComponent.h"
#include "Engine/TransformComponent.h"
#include "Math/VectorMath.h"
#include "Tactics/TacticsAbilityComponent.h"
#include "Tactics/TacticsAbilityTable.h"
#include "Tactics/TacticsPreviewComponent.h"
#include "Tactics/TacticsPreviewEvent.h"
#include "Tilemap/TilemapGridComponent.h"

namespace
{
	using World = tactics::PreviewSystem::World;
	Vector3f GetTileSize(World& world)
	{
		for (auto&& view : world.Query<ecs::query::Include<const tilemap::GridComponent>>())
		{
			const auto& grid = view.ReadRequired<tilemap::GridComponent>();
			return grid.m_TileSize;
		}
		return Vector3f::Zero;
	}
}

void tactics::PreviewSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	for (const auto& event : world.Events<tactics::PreviewEvent>())
	{
		auto& previewComponent = !world.HasComponent<tactics::PreviewComponent>(event.m_Entity)
			? world.AddComponent<tactics::PreviewComponent>(event.m_Entity)
			: world.WriteComponent<tactics::PreviewComponent>(event.m_Entity);
		previewComponent.m_Ability = event.m_Ability;
	}

	using ChangedQuery = ecs::query
		::Include<tactics::PreviewComponent>
		::Removed<tactics::SelectedComponent>;
	for (auto&& view : world.Query<ChangedQuery>())
	{
		world.RemoveComponent<tactics::PreviewComponent>(view);
	}

	using PreviewQuery = ecs::query
		::Include<
		const eng::TransformComponent,
		const tactics::AbilityComponent,
		const tactics::PreviewComponent>;
	for (auto&& view : world.Query<PreviewQuery>())
	{
		const Vector3f tileSize = GetTileSize(world);
		const auto& abilityComponent = view.ReadRequired<tactics::AbilityComponent>();
		const auto& previewComponent = view.ReadRequired<tactics::PreviewComponent>();
		const auto& transformComponent = view.ReadRequired<eng::TransformComponent>();
		if (!enumerate::Contains(abilityComponent.m_Abilities, previewComponent.m_Ability))
			return;

		const auto& table = world.ReadResource<tactics::AbilityTable>();
		const tactics::Ability& ability = table.GetObject(previewComponent.m_Ability);

		auto& lines = world.WriteSingleton<eng::LinesSingleton>();
		core::VariantMatch(ability.m_Skill,
			[&](const tactics::Path& data)
			{
				Vector3f position = transformComponent.m_Translate;
				for (const Vector3i& offset : data.m_Nodes)
				{
					position += math::Multiply(offset, tileSize);
					lines.AddAABB(position, tileSize * 0.25f, Colour::Green);
				}
			},
			[&](const tactics::Rush& data)
			{
				Vector3f position = transformComponent.m_Translate;
				for (int32 i = 1; i < 8; ++i)
				{
					position += math::Multiply(data.m_Direction, tileSize);
					lines.AddAABB(position, tileSize * 0.25f, Colour::Green);
				}
			},
			[&](const tactics::Teleport& data)
			{
				const Vector3f position = transformComponent.m_Translate + math::Multiply(data.m_Offset, tileSize);
				lines.AddAABB(position, tileSize * 0.25f, Colour::Green);
			});
	}
}
