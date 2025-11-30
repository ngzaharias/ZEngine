#include "GameClientPCH.h"
#include "GameClient/NetworkStopSystem.h"

#include "Core/GameTime.h"
#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/NetworkChangeRequestEvent.h"
#include "Engine/NetworkManager.h"
#include "GameClient/GameStateChangeFinished.h"
#include "GameClient/GameStateChangeRequest.h"
#include "GameClient/GameStateStateSingleton.h"

void gamestate::NetworkStopSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	if (world.HasAny<gamestate::StateSingleton>())
	{
		const auto& stateComponent = world.ReadSingleton<gamestate::StateSingleton>();
		if (std::holds_alternative<gamestate::NetworkStop>(stateComponent.m_State))
		{
			auto& eventData = world.AddEvent<eng::network::ChangeRequestEvent>();
			eventData.m_Request = eng::network::Shutdown();
		}
	}

	if (world.HasAny<ecs::query::Include<const eng::network::ChangeFinishedEvent>>())
	{
		const auto& stateComponent = world.ReadSingleton<gamestate::StateSingleton>();
		if (std::holds_alternative<gamestate::NetworkStop>(stateComponent.m_State))
			world.AddEvent<gamestate::ChangeFinished>();
	}
}