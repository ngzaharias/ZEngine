#include "GameClientPCH.h"
#include "GameClient/NetworkJoinSystem.h"

#include "Core/GameTime.h"
#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/LevelLoadRequest.h"
#include "Engine/NetworkChangeRequest.h"
#include "Engine/NetworkManager.h"
#include "GameClient/GameStateChangeFinished.h"
#include "GameClient/GameStateChangeRequest.h"
#include "GameClient/GameStateNetworkJoinSingleton.h"
#include "GameClient/GameStateStateSingleton.h"
#include "GameClient/ModalMessageComponent.h"

void gamestate::NetworkJoinSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	if (world.HasAny<gamestate::StateSingleton>())
	{
		const auto& stateComponent = world.ReadSingleton<gamestate::StateSingleton>();
		if (std::holds_alternative<gamestate::NetworkJoin>(stateComponent.m_State))
		{
			auto& joinComponent = world.WriteSingleton<gamestate::NetworkJoinSingleton>();
			joinComponent.m_State = NetworkJoinSingleton::EState::Connect;

			const auto& request = std::get<gamestate::NetworkJoin>(stateComponent.m_State);
			auto& requestComponent = world.AddEvent<eng::network::ChangeRequest>();

			eng::network::Startup startup;
			startup.m_Mode = request.m_Mode;
			startup.m_ClientAddress = request.m_ClientAddress;
			startup.m_ClientPort = request.m_ClientPort;
			startup.m_ServerAddress = request.m_ServerAddress;
			startup.m_ServerPort = request.m_ServerPort;
			requestComponent.m_Request = std::move(startup);
		}
	}

	const auto& readComponent = world.ReadSingleton<gamestate::NetworkJoinSingleton>();
	if (readComponent.m_State != NetworkJoinSingleton::EState::None)
	{
		auto& writeComponent = world.WriteSingleton<gamestate::NetworkJoinSingleton>();
		switch (writeComponent.m_State)
		{
		case NetworkJoinSingleton::EState::Connect:
		{
			const auto& networkManager = world.ReadResource< eng::NetworkManager>();
			const auto& networkPeer = networkManager.GetPeer();
			if (networkPeer.IsConnected())
			{
				writeComponent.m_State = NetworkJoinSingleton::EState::LoadLevel;
			}
			else if (networkPeer.HasConnectionFailed())
			{
				writeComponent.m_Result = NetworkJoinSingleton::EResult::Failure;
				writeComponent.m_State = NetworkJoinSingleton::EState::Finished;

				auto& messageComponent = world.AddComponent<gui::modal::MessageComponent>(world.CreateEntity());
				messageComponent.m_Title = "Network: Error";
				messageComponent.m_Message = "Failed to Connect to Server";
			}
		} break;

		case NetworkJoinSingleton::EState::LoadLevel:
		{
			world.AddEvent<eng::level::LoadRequest>();
			writeComponent.m_State = NetworkJoinSingleton::EState::SyncWorld;
		} break;

		case NetworkJoinSingleton::EState::SyncWorld:
		{
			writeComponent.m_State = NetworkJoinSingleton::EState::Finished;
		} break;

		case NetworkJoinSingleton::EState::Finished:
		{
			world.AddEvent<gamestate::ChangeFinished>();
			writeComponent.m_State = NetworkJoinSingleton::EState::None;
		} break;
		}
	}
}