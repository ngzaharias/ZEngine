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

	for (const auto& request : world.Events<gamestate::ChangeRequest>())
	{
		auto& stateComponent = world.WriteSingleton<gamestate::StateSingleton>();
		stateComponent.m_Queue.Append(request.m_Queue);
	}

	const auto& constComponent = world.ReadSingleton<gamestate::StateSingleton>();
	const bool hasEmptyState = std::holds_alternative<gamestate::None>(constComponent.m_State);
	const bool hasStateQueued = !constComponent.m_Queue.IsEmpty();
	const bool hasStateFinished = world.HasAny<gamestate::ChangeFinished>();
	if ((hasEmptyState && hasStateQueued) || hasStateFinished)
	{
		auto& stateComponent = world.WriteSingleton<gamestate::StateSingleton>();

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