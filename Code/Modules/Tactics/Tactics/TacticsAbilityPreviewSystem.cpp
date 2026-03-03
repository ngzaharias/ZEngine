#include "TacticsPCH.h"
#include "Tactics/TacticsAbilityPreviewSystem.h"

#include "Core/Algorithms.h"
#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/LinesComponent.h"
#include "Engine/TransformComponent.h"
#include "Math/VectorMath.h"
#include "Tactics/TacticsAbilityPreviewComponent.h"
#include "Tactics/TacticsAbilityPreviewEvent.h"
#include "Tactics/TacticsAbilityTable.h"
#include "Tactics/TacticsPawnAbilitiesComponent.h"
#include "Tactics/TacticsPawnSelectedComponent.h"
#include "Tilemap/TilemapGridComponent.h"

namespace
{
	using World = tactics::AbilityPreviewSystem::World;
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

void tactics::AbilityPreviewSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	for (const auto& event : world.Events<tactics::AbilityPreviewEvent>())
	{
		auto& previewComponent = !world.HasComponent<tactics::AbilityPreviewComponent>(event.m_Entity)
			? world.AddComponent<tactics::AbilityPreviewComponent>(event.m_Entity)
			: world.WriteComponent<tactics::AbilityPreviewComponent>(event.m_Entity);
		previewComponent.m_Ability = event.m_Ability;
	}

	using ChangedQuery = ecs::query
		::Include<tactics::AbilityPreviewComponent>
		::Removed<tactics::PawnSelectedComponent>;
	for (auto&& view : world.Query<ChangedQuery>())
	{
		world.RemoveComponent<tactics::AbilityPreviewComponent>(view);
	}

	using PreviewQuery = ecs::query
		::Include<
		const eng::TransformComponent,
		const tactics::PawnAbilitiesComponent,
		const tactics::AbilityPreviewComponent>;
	for (auto&& view : world.Query<PreviewQuery>())
	{
		const Vector3f tileSize = GetTileSize(world);
		const auto& abilityComponent = view.ReadRequired<tactics::PawnAbilitiesComponent>();
		const auto& previewComponent = view.ReadRequired<tactics::AbilityPreviewComponent>();
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
