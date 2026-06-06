#include "TacticsPCH.h"
#include "ClientTactics/TacticsAbilityPreviewSystem.h"

#include "ClientTactics/TacticsAbilityPreviewComponent.h"
#include "ClientTactics/TacticsAbilityPreviewEvent.h"
#include "ClientTactics/TacticsInputPriorityEnum.h"
#include "ClientTactics/TacticsPawnSelectedComponent.h"
#include "Core/Algorithms.h"
#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/InputManager.h"
#include "Engine/LinesComponent.h"
#include "Engine/TransformComponent.h"
#include "Math/VectorMath.h"
#include "SharedTactics/TacticsAbilityTable.h"
#include "SharedTactics/TacticsPawnAbilitiesTemplate.h"
#include "SharedTilemap/TilemapGridComponent.h"

namespace
{
	const str::Name strCancel = str::Name::Create("Cancel");
	const str::Name strExecute = str::Name::Create("Execute");
	const str::Name strInput = str::Name::Create("AbilityPreviewSystem");

	using World = client::tactics::AbilityPreviewSystem::World;
	Vector3f GetTileSize(World& world)
	{
		for (auto&& view : world.Query<ecs::query::Include<const shared::tilemap::GridComponent>>())
		{
			const auto& grid = view.ReadRequired<shared::tilemap::GridComponent>();
			return grid.m_TileSize;
		}
		return Vector3f::Zero;
	}

	void ProcessInput(World& world)
	{
		for (auto&& view : world.Query<ecs::query::Added<client::tactics::AbilityPreviewComponent>>())
		{
			input::Layer layer;
			layer.m_Priority = client::tactics::EInputPriority::AbilityPreview;
			layer.m_Bindings.Emplace(strExecute, input::EKey::Mouse_Left, true);
			layer.m_Bindings.Emplace(strCancel, input::EKey::Mouse_Right, true);

			auto& input = world.WriteResource<eng::InputManager>();
			input.AppendLayer(strInput, layer);
		}

		using RemovedQuery = ecs::query
			::Condition<ecs::Alive, ecs::Dead>
			::Removed<client::tactics::AbilityPreviewComponent>;
		for (auto&& view : world.Query<RemovedQuery>())
		{
			auto& input = world.WriteResource<eng::InputManager>();
			input.RemoveLayer(strInput);
		}
	}

	void ProcessRequests(World& world)
	{
		for (const auto& event : world.Events<const client::tactics::AbilityPreviewEvent>())
		{
			auto& previewComponent = !world.HasComponent<client::tactics::AbilityPreviewComponent>(event.m_Entity)
				? world.AddComponent<client::tactics::AbilityPreviewComponent>(event.m_Entity)
				: world.WriteComponent<client::tactics::AbilityPreviewComponent>(event.m_Entity);
			previewComponent.m_Ability = event.m_Ability;
		}

		using ChangedQuery = ecs::query
			::Include<client::tactics::AbilityPreviewComponent>
			::Removed<client::tactics::PawnSelectedComponent>;
		for (auto&& view : world.Query<ChangedQuery>())
		{
			world.RemoveComponent<client::tactics::AbilityPreviewComponent>(view);
		}
	}

	void ProcessPreviews(World& world)
	{
		using Query = ecs::query
			::Include<
			const client::tactics::AbilityPreviewComponent,
			const eng::TransformComponent,
			const shared::tactics::PawnAbilitiesTemplate>;
		for (auto&& view : world.Query<Query>())
		{
			const Vector3f tileSize = GetTileSize(world);
			const auto& abilityTemplate = view.ReadRequired<shared::tactics::PawnAbilitiesTemplate>();
			const auto& previewComponent = view.ReadRequired<client::tactics::AbilityPreviewComponent>();
			const auto& transformComponent = view.ReadRequired<eng::TransformComponent>();
			if (!enumerate::Contains(abilityTemplate.m_Abilities, previewComponent.m_Ability))
				return;

			const auto& table = world.ReadResource<shared::tactics::AbilityTable>();
			const shared::tactics::Ability& ability = table.GetObject(previewComponent.m_Ability);

			auto& lines = world.WriteComponent<eng::LinesComponent>();
			core::VariantMatch(ability.m_Skill,
				[&](const shared::tactics::Path& data)
				{
					Vector3f position = transformComponent.m_Translate;
					for (const Vector3i& offset : data.m_Nodes)
					{
						position += math::Multiply(offset, tileSize);
						lines.AddAABB(position, tileSize * 0.25f, Colour::Green);
					}
				},
				[&](const shared::tactics::Rush& data)
				{
					Vector3f position = transformComponent.m_Translate;
					for (int32 i = 1; i < 8; ++i)
					{
						position += math::Multiply(data.m_Direction, tileSize);
						lines.AddAABB(position, tileSize * 0.25f, Colour::Green);
					}
				},
					[&](const shared::tactics::Teleport& data)
				{
					const Vector3f position = transformComponent.m_Translate + math::Multiply(data.m_Offset, tileSize);
					lines.AddAABB(position, tileSize * 0.25f, Colour::Green);
				});

			const auto& inputManager = world.ReadResource<eng::InputManager>();
			if (inputManager.IsPressed(strExecute))
			{
				world.RemoveComponent<client::tactics::AbilityPreviewComponent>(view);
			}
			else if (inputManager.IsPressed(strCancel))
			{
				world.RemoveComponent<client::tactics::AbilityPreviewComponent>(view);
			}
		}
	}
}

void client::tactics::AbilityPreviewSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	ProcessInput(world);
	ProcessRequests(world);
	ProcessPreviews(world);
}
