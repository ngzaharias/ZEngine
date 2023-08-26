#include "GameClientPCH.h"
#include "GameClient/NetworkHostSystem.h"

#include <Core/GameTime.h>

#include <ECS/EntityWorld.h>
#include <ECS/QueryTypes.h>
#include <ECS/WorldView.h>

#include <Engine/NetworkComponents.h>
#include <Engine/NetworkManager.h>

#include "GameClient/GameStateComponents.h"

void gamestate::NetworkHostSystem::Update(World& world, const GameTime& gameTime)
{
	if (world.HasAny<ecs::query::Updated<const gamestate::StateComponent>>())
	{
		const auto& stateComponent = world.GetSingleton<const gamestate::StateComponent>();
		if (std::holds_alternative<gamestate::NetworkHost>(stateComponent.m_State))
		{
			world.AddSingleton<gamestate::NetworkHostComponent>();

			const auto& request = std::get<gamestate::NetworkHost>(stateComponent.m_State);
			auto& component = world.AddEventComponent<network::RequestComponent>();

			network::Startup startup;
			startup.m_Mode = request.m_Mode;
			startup.m_ClientAddress = request.m_ClientAddress;
			startup.m_ClientPort = request.m_ClientPort;
			startup.m_ServerAddress = request.m_ServerAddress;
			startup.m_ServerPort = request.m_ServerPort;
			component.m_Request = std::move(startup);
		}
	}

	const bool isHosting = world.HasAny<ecs::query::Include<gamestate::NetworkHostComponent>>();
	const bool isFinished = world.HasAny<ecs::query::Include<const network::RequestFinishedComponent>>();
	if (isHosting && isFinished)
	{
		world.RemoveSingleton<gamestate::NetworkHostComponent>();
		world.AddEventComponent<gamestate::StateFinishedComponent>();
	}
}