#include "TacticsPCH.h"
#include "Tactics/TacticsPreviewSystem.h"

#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/LinesComponent.h"
#include "Engine/TransformComponent.h"
#include "Math/VectorMath.h"
#include "Tactics/TacticsAbilityComponent.h"
#include "Tactics/TacticsAbilityTable.h"
#include "Tactics/TacticsSelectedComponent.h"
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
	using Query = ecs::query
		::Include<
		const eng::TransformComponent,
		const tactics::AbilityComponent,
		const tactics::SelectedComponent>;
	for (auto&& view : world.Query<Query>())
	{
		const Vector3f tileSize = GetTileSize(world);
		const auto& abilityTable = world.ReadResource<tactics::AbilityTable>();
		const auto& abilityComponent = view.ReadRequired<tactics::AbilityComponent>();
		const auto& transformComponent = view.ReadRequired<eng::TransformComponent>();

		auto& lines = world.WriteSingleton<eng::LinesSingleton>();
		for (const str::Name& name : abilityComponent.m_Abilities)
		{
			const tactics::Ability& ability = abilityTable.GetObject(name);
			core::VariantMatch(ability,
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
}
