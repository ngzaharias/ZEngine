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

	if (world.HasAny<gamestate::StateSingleton>())
	{
		const auto& stateComponent = world.ReadSingleton<gamestate::StateSingleton>();
		if (std::holds_alternative<gamestate::NetworkStop>(stateComponent.m_State))
		{
			auto& component = world.AddEvent<eng::network::ChangeRequest>();
			component.m_Request = eng::network::Shutdown();
		}
	}

	if (world.HasAny<ecs::query::Include<const eng::network::ChangeFinished>>())
	{
		const auto& stateComponent = world.ReadSingleton<gamestate::StateSingleton>();
		if (std::holds_alternative<gamestate::NetworkStop>(stateComponent.m_State))
			world.AddEvent<gamestate::ChangeFinished>();
	}
}