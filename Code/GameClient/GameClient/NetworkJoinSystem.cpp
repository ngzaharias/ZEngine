#include "GameClientPCH.h"
#include "GameClient/NetworkJoinSystem.h"

#include <Core/GameTime.h>

#include <ECS/EntityWorld.h>
#include <ECS/QueryTypes.h>
#include <ECS/WorldView.h>

#include <Engine/LevelComponents.h>
#include <Engine/NetworkComponents.h>
#include <Engine/NetworkManager.h>

#include "GameClient/GameStateComponents.h"
#include "GameClient/ModalComponents.h"

void gamestate::NetworkJoinSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	if (world.HasAny<ecs::query::Updated<const gamestate::StateComponent>>())
	{
		const auto& stateComponent = world.GetSingleton<const gamestate::StateComponent>();
		if (std::holds_alternative<gamestate::NetworkJoin>(stateComponent.m_State))
		{
			auto& joinComponent = world.AddSingleton<gamestate::NetworkJoinComponent>();
			joinComponent.m_State = NetworkJoinComponent::EState::Connect;

			const auto& request = std::get<gamestate::NetworkJoin>(stateComponent.m_State);
			auto& requestComponent = world.AddEventComponent<network::RequestComponent>();

			network::Startup startup;
			startup.m_Mode = request.m_Mode;
			startup.m_ClientAddress = request.m_ClientAddress;
			startup.m_ClientPort = request.m_ClientPort;
			startup.m_ServerAddress = request.m_ServerAddress;
			startup.m_ServerPort = request.m_ServerPort;
			requestComponent.m_Request = std::move(startup);
		}
	}

	const bool isJoining = world.HasAny<ecs::query::Include<gamestate::NetworkJoinComponent>>();
	if (world.HasSingleton<gamestate::NetworkJoinComponent>())
	{
		auto& joinComponent = world.GetSingleton<gamestate::NetworkJoinComponent>();
		switch (joinComponent.m_State)
		{
		case NetworkJoinComponent::EState::Connect:
		{
			const auto& networkManager = world.GetResource<const eng::NetworkManager>();
			const auto& networkPeer = networkManager.GetPeer();
			if (networkPeer.IsConnected())
			{
				joinComponent.m_State = NetworkJoinComponent::EState::LoadLevel;
			}
			else if (networkPeer.HasConnectionFailed())
			{
				joinComponent.m_Result = NetworkJoinComponent::EResult::Failure;
				joinComponent.m_State = NetworkJoinComponent::EState::Finished;
				
				auto& messageComponent = world.AddComponent<gui::modal::MessageComponent>(world.CreateEntity());
				messageComponent.m_Title = "Network: Error";
				messageComponent.m_Message = "Failed to Connect to Server";
			}
		} break;

		case NetworkJoinComponent::EState::LoadLevel:
		{
			world.AddEventComponent<eng::LevelLoadRequestComponent>();
			joinComponent.m_State = NetworkJoinComponent::EState::SyncWorld;
		} break;

		case NetworkJoinComponent::EState::SyncWorld:
		{
			joinComponent.m_State = NetworkJoinComponent::EState::Finished;
		} break;

		case NetworkJoinComponent::EState::Finished:
		{
			world.RemoveSingleton<gamestate::NetworkJoinComponent>();
			world.AddEventComponent<gamestate::StateFinishedComponent>();
		} break;
		}
	}
}