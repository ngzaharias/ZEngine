#include "GameClientPCH.h"
#include "GameClient/GameStateSystem.h"

#include "Core/VariantHelpers.h"
#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "GameClient/GameStateComponents.h"
#include "GameClient/GameStateTypes.h"

void gamestate::StateSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	for (const ecs::Entity& entity : world.Query<ecs::query::Added<const gamestate::RequestComponent>>())
	{
		const auto& requestComponent = world.ReadComponent<gamestate::RequestComponent>(entity);
		auto& stateComponent = world.WriteSingleton<gamestate::StateComponent>();
		stateComponent.m_Queue.Append(requestComponent.m_Queue);
	}

	const auto& constComponent = world.ReadSingleton<gamestate::StateComponent>();
	const bool hasEmptyState = std::holds_alternative<gamestate::None>(constComponent.m_State);
	const bool hasStateQueued = !constComponent.m_Queue.IsEmpty();
	const bool hasStateFinished = world.HasAny<ecs::query::Include<gamestate::StateFinishedComponent>>();
	if ((hasEmptyState && hasStateQueued) || hasStateFinished)
	{
		auto& stateComponent = world.WriteSingleton<gamestate::StateComponent>();

		// remove old state
		stateComponent.m_State = gamestate::None();

		// append new state
		if (!stateComponent.m_Queue.IsEmpty())
		{
			stateComponent.m_State = stateComponent.m_Queue.GetFirst();
			stateComponent.m_Queue.RemoveOrderedAt(0);
		}
	}
}