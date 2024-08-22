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
		const auto& stateComponent = world.ReadSingleton<gamestate::StateComponent>();
		if (std::holds_alternative<gamestate::NetworkStop>(stateComponent.m_State))
		{
			auto& component = world.AddEventComponent<eng::network::RequestComponent>();
			component.m_Request = eng::network::Shutdown();
		}
	}

	if (world.HasAny<ecs::query::Include<const eng::network::RequestFinishedComponent>>())
	{
		const auto& stateComponent = world.ReadSingleton<gamestate::StateComponent>();
		if (std::holds_alternative<gamestate::NetworkStop>(stateComponent.m_State))
			world.AddEventComponent<gamestate::StateFinishedComponent>();
	}
}