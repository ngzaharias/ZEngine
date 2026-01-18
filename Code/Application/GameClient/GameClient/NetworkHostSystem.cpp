#include "GameClientPCH.h"
#include "GameClient/NetworkHostSystem.h"

#include "Core/GameTime.h"
#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/NetworkChangeRequest.h"
#include "GameClient/GameStateChangeFinished.h"
#include "GameClient/GameStateChangeRequest.h"
#include "GameClient/GameStateStateSingleton.h"
#include "Network/NetworkManager.h"

void gamestate::NetworkHostSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	if (world.HasAny<gamestate::StateSingleton>())
	{
		const auto& stateComponent = world.ReadSingleton<gamestate::StateSingleton>();
		if (std::holds_alternative<gamestate::NetworkHost>(stateComponent.m_State))
		{
			const auto& request = std::get<gamestate::NetworkHost>(stateComponent.m_State);
			auto& component = world.AddEvent<eng::network::ChangeRequest>();

			eng::network::Startup startup;
			startup.m_Mode = request.m_Mode;
			startup.m_ClientAddress = request.m_ClientAddress;
			startup.m_ClientPort = request.m_ClientPort;
			startup.m_ServerAddress = request.m_ServerAddress;
			startup.m_ServerPort = request.m_ServerPort;
			component.m_Request = std::move(startup);
		}
	}

	if (world.HasAny<eng::network::ChangeRequest>())
	{
		const auto& stateComponent = world.ReadSingleton<gamestate::StateSingleton>();
		if (std::holds_alternative<gamestate::NetworkHost>(stateComponent.m_State))
			world.AddEvent<gamestate::ChangeFinished>();
	}
}