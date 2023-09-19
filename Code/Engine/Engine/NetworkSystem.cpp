#include "EnginePCH.h"
#include "Engine/NetworkSystem.h"

#include <Core/GameTime.h>

#include <ECS/EntityWorld.h>
#include <ECS/QueryTypes.h>
#include <ECS/WorldView.h>

#include <Network/Host.h>
#include <Network/Peer.h>

#include "Engine/NetworkComponents.h"
#include "Engine/NetworkManager.h"

namespace
{
	bool IsClient(const net::EMode value)
	{
		switch (value)
		{
		case net::EMode::Standalone:
		case net::EMode::RemoteClient:
		case net::EMode::ListenServer:
			return true;
		}
		return false;
	}

	bool IsServer(const net::EMode value)
	{
		switch (value)
		{
		case net::EMode::Standalone:
		case net::EMode::ListenServer:
		case net::EMode::DedicatedServer:
			return true;
		}
		return false;
	}
}

void network::NetworkSystem::Initialise(World& world)
{
	world.AddSingleton<network::StateComponent>();
}

void network::NetworkSystem::Shutdown(World& world)
{
	world.RemoveSingleton<network::StateComponent>();
}

void network::NetworkSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	auto& networkManager = world.GetManager<eng::NetworkManager>();
	auto& host = networkManager.GetHost();
	auto& peer = networkManager.GetPeer();

	for (const ecs::Entity& entity : world.Query<ecs::query::Include<const network::RequestComponent>>())
	{
		const auto& eventComponent = world.GetComponent<const network::RequestComponent>(entity);
		auto& stateComponent = world.GetSingleton<network::StateComponent>();

		if (IsClient(stateComponent.m_Mode))
			peer.Shutdown();

		if (IsServer(stateComponent.m_Mode))
			host.Shutdown();

		if (std::holds_alternative<network::Startup>(eventComponent.m_Request))
		{
			const auto& request = std::get<network::Startup>(eventComponent.m_Request);

			// update new values
			stateComponent.m_Mode = request.m_Mode;
			stateComponent.m_ClientAddress = request.m_ClientAddress;
			stateComponent.m_ClientPort = request.m_ClientPort;
			stateComponent.m_ServerAddress = request.m_ServerAddress;
			stateComponent.m_ServerPort = request.m_ServerPort;

			if (IsServer(stateComponent.m_Mode))
				host.Startup(stateComponent.m_ServerAddress, stateComponent.m_ServerPort, gameTime.m_TotalTime);

			if (IsClient(stateComponent.m_Mode))
			{
				peer.Startup(stateComponent.m_ClientAddress, stateComponent.m_ClientPort, gameTime.m_TotalTime);
				peer.Connect(stateComponent.m_ServerAddress, stateComponent.m_ServerPort);
			}
		}

		world.AddEventComponent<network::RequestFinishedComponent>();
	}
}