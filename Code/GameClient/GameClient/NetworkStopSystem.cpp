#include "GameClientPCH.h"
#include "GameClient/NetworkStopSystem.h"

#include "Core/GameTime.h"
#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/NetworkComponents.h"
#include "Engine/NetworkManager.h"
#include "GameClient/GameStateComponents.h"

void gamestate::NetworkStopSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	if (world.HasAny<ecs::query::Updated<const gamestate::StateComponent>>())
	{
		const auto& stateComponent = world.GetSingleton<const gamestate::StateComponent>();
		if (std::holds_alternative<gamestate::NetworkStop>(stateComponent.m_State))
		{
			world.AddSingleton<gamestate::NetworkStopComponent>();

			auto& component = world.AddEventComponent<network::RequestComponent>();
			component.m_Request = network::Shutdown();
		}
	}

	const bool isStopping = world.HasAny<ecs::query::Include<gamestate::NetworkStopComponent>>();
	const bool isFinished = world.HasAny<ecs::query::Include<const network::RequestFinishedComponent>>();
	if (isStopping && isFinished)
	{
		world.RemoveSingleton<gamestate::NetworkStopComponent>();
		world.AddEventComponent<gamestate::StateFinishedComponent>();
	}
}