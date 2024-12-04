#include "GameClientPCH.h"
#include "GameClient/NetworkJoinSystem.h"

#include "Core/GameTime.h"
#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/LevelComponents.h"
#include "Engine/NetworkComponents.h"
#include "Engine/NetworkManager.h"
#include "GameClient/GameStateComponents.h"
#include "GameClient/ModalComponents.h"

void gamestate::NetworkJoinSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	if (world.HasAny<ecs::query::Updated<const gamestate::StateComponent>>())
	{
		const auto& stateComponent = world.ReadSingleton<gamestate::StateComponent>();
		if (std::holds_alternative<gamestate::NetworkJoin>(stateComponent.m_State))
		{
			auto& joinComponent = world.WriteSingleton<gamestate::NetworkJoinComponent>();
			joinComponent.m_State = NetworkJoinComponent::EState::Connect;

			const auto& request = std::get<gamestate::NetworkJoin>(stateComponent.m_State);
			auto& requestComponent = world.AddEventComponent<eng::network::RequestComponent>();

			eng::network::Startup startup;
			startup.m_Mode = request.m_Mode;
			startup.m_ClientAddress = request.m_ClientAddress;
			startup.m_ClientPort = request.m_ClientPort;
			startup.m_ServerAddress = request.m_ServerAddress;
			startup.m_ServerPort = request.m_ServerPort;
			requestComponent.m_Request = std::move(startup);
		}
	}

	const auto& readComponent = world.ReadSingleton<gamestate::NetworkJoinComponent>();
	if (readComponent.m_State != NetworkJoinComponent::EState::None)
	{
		auto& writeComponent = world.WriteSingleton<gamestate::NetworkJoinComponent>();
		switch (writeComponent.m_State)
		{
		case NetworkJoinComponent::EState::Connect:
		{
			const auto& networkManager = world.ReadResource< eng::NetworkManager>();
			const auto& networkPeer = networkManager.GetPeer();
			if (networkPeer.IsConnected())
			{
				writeComponent.m_State = NetworkJoinComponent::EState::LoadLevel;
			}
			else if (networkPeer.HasConnectionFailed())
			{
				writeComponent.m_Result = NetworkJoinComponent::EResult::Failure;
				writeComponent.m_State = NetworkJoinComponent::EState::Finished;
				
				auto& messageComponent = world.AddComponent<gui::modal::MessageComponent>(world.CreateEntity());
				messageComponent.m_Title = "Network: Error";
				messageComponent.m_Message = "Failed to Connect to Server";
			}
		} break;

		case NetworkJoinComponent::EState::LoadLevel:
		{
			world.AddEventComponent<eng::level::LoadRequestComponent>();
			writeComponent.m_State = NetworkJoinComponent::EState::SyncWorld;
		} break;

		case NetworkJoinComponent::EState::SyncWorld:
		{
			writeComponent.m_State = NetworkJoinComponent::EState::Finished;
		} break;

		case NetworkJoinComponent::EState::Finished:
		{
			writeComponent.m_State = NetworkJoinComponent::EState::None;
			world.AddEventComponent<gamestate::StateFinishedComponent>();
		} break;
		}
	}
}