#include "GameDebugPCH.h"
#include "GameDebug/NetworkSystem.h"

#include "Core/GameTime.h"
#include "Core/Parse.h"
#include "Math/Vector.h"
#include "ECS/EntityWorld.h"
#include "ECS/NameComponent.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "GameClient/GameStateChangeRequest.h"
#include "GameDebug/DebugNetworkWindowRequest.h"
#include "GameDebug/NetworkWindowComponent.h"
#include "Math/Math.h"

#include "imgui/imgui.h"
#include "imgui/imgui_stdlib.h"
#include "imgui/imgui_user.h"

namespace
{
	bool SendServerData(HSteamNetConnection connection, const void* pData, uint32 nSizeOfData)
	{
		int nSendFlags = k_nSteamNetworkingSend_Unreliable;
		EResult res = SteamNetworkingSockets()->SendMessageToConnection(connection, pData, nSizeOfData, nSendFlags, nullptr);
		switch (res)
		{
		case k_EResultOK:
		case k_EResultIgnored:
			break;

		case k_EResultInvalidParam:
			Z_LOG(ELog::Network, "Failed sending data to server: Invalid connection handle, or the individual message is too big\n");
			return false;
		case k_EResultInvalidState:
			Z_LOG(ELog::Network, "Failed sending data to server: Connection is in an invalid state\n");
			return false;
		case k_EResultNoConnection:
			Z_LOG(ELog::Network, "Failed sending data to server: Connection has ended\n");
			return false;
		case k_EResultLimitExceeded:
			Z_LOG(ELog::Network, "Failed sending data to server: There was already too much data queued to be sent\n");
			return false;
		default:
		{
			Z_LOG(ELog::Network, "SendMessageToConnection returned {}", static_cast<int32>(res));
			return false;
		}
		}
		return true;
	}
}

void debug::NetworkSystem::Initialise(World& world)
{
	world.AddEvent<debug::NetworkWindowRequest>();
}

void debug::NetworkSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	if (SteamNetworkingSockets() && m_Connection != k_HSteamNetConnection_Invalid)
	{
		SteamNetworkingMessage_t* msgs[32];
		int res = SteamNetworkingSockets()->ReceiveMessagesOnConnection(m_Connection, msgs, 32);
		for (int i = 0; i < res; i++)
		{
			SteamNetworkingMessage_t* message = msgs[i];
			Z_LOG(ELog::Network, "Message from Server");
			message->Release();
		}
	}

	SteamGameServer_RunCallbacks();

	if (m_NetPollGroup)
	{
		SteamNetworkingMessage_t* msgs[128];
		int numMessages = SteamGameServerNetworkingSockets()->ReceiveMessagesOnPollGroup(m_NetPollGroup, msgs, 128);
		for (int i = 0; i < numMessages; i++)
		{
			SteamNetworkingMessage_t* message = msgs[i];
			Z_LOG(ELog::Network, "Message from Client");
			message->Release();
		}
	}

	constexpr ImGuiWindowFlags s_WindowFlags =
		ImGuiWindowFlags_NoCollapse;
	constexpr Vector2f s_DefaultPos = Vector2f(100.f, 100.f);
	constexpr Vector2f s_DefaultSize = Vector2f(300.f, 200.f);

	for (const auto& request : world.Events<debug::NetworkWindowRequest>())
	{
		const int32 identifier = m_WindowIds.Borrow();
		const ecs::Entity windowEntity = world.CreateEntity();
		world.AddComponent<ecs::NameComponent>(windowEntity, "Debug: Network Window");

		auto& window = world.AddComponent<debug::NetworkWindowComponent>(windowEntity);
		window.m_Identifier = identifier;
	}

	for (auto&& view : world.Query<ecs::query::Include<debug::NetworkWindowComponent>>())
	{
		auto& window = view.WriteRequired<debug::NetworkWindowComponent>();
		const str::String label = std::format("Network Debugger : {}", window.m_Identifier);

		bool isOpen = true;
		imgui::SetNextWindowPos(s_DefaultPos, ImGuiCond_FirstUseEver);
		imgui::SetNextWindowSize(s_DefaultSize, ImGuiCond_FirstUseEver);
		if (ImGui::Begin(label.c_str(), &isOpen, s_WindowFlags))
		{
			if (ImGui::Button("start server"))
			{
				SteamNetworkingIPAddr addr;
				addr.Clear();
				addr.m_port = 27020;

				uint32 unIP = 0;
				uint16 gamePort = 27015;
				uint16 usQueryPort = 27016;
				EServerMode eMode = eServerModeNoAuthentication;
				const char* version = "alpha-0.0.1";

				SteamErrMsg errMsg = { 0 };
				if (SteamGameServer_InitEx(unIP, gamePort, usQueryPort, eMode, version, &errMsg) == k_ESteamAPIInitResult_OK)
				{
					SteamGameServer()->LogOnAnonymous();
					//m_ListenSocket = SteamGameServerNetworkingSockets()->CreateListenSocketIP(addr, 0, nullptr);
					m_ListenSocket = SteamGameServerNetworkingSockets()->CreateListenSocketP2P(0, 0, nullptr);
					m_NetPollGroup = SteamGameServerNetworkingSockets()->CreatePollGroup();

					Z_LOG(ELog::Network, "Server Started");
				}
			}

			if (ImGui::Button("stop server"))
			{

			}

			if (ImGui::Button("join server"))
			{
				if (false)
				{
					SteamNetworkingIPAddr addr{};
					addr.ParseString("127.0.0.1:27020");
					m_Connection = SteamNetworkingSockets()->ConnectByIPAddress(addr, 0, nullptr);
				}
				else
				{
					CSteamID userId = SteamUser()->GetSteamID();
					CSteamID serverId = SteamGameServer()->GetSteamID();
					SteamNetworkingIdentity identity;
					identity.SetSteamID(serverId);
					m_Connection = SteamNetworkingSockets()->ConnectP2P(identity, 0, 0, nullptr);
				}

				if (m_Connection == k_HSteamNetConnection_Invalid)
					Z_LOG(ELog::Network, "Client failed to Connect");
			}

			if (ImGui::Button("leave server"))
			{
			}

			if (ImGui::Button("message to server"))
			{
				SendServerData(m_Connection, nullptr, 0);
			}

			/*
			ImGui::InputText("##client_address", &window.m_ClientAddress);
			ImGui::SameLine();
			ImGui::InputInt("##client_port", &window.m_ClientPort);

			ImGui::InputText("##server_address", &window.m_ServerAddress);
			ImGui::SameLine();
			ImGui::InputInt("##server_port", &window.m_ServerPort);

			if (ImGui::Button("Standalone"))
			{
				gamestate::NetworkHost request;
				request.m_Mode = net::EMode::Standalone;
				request.m_ClientAddress = window.m_ClientAddress;
				request.m_ClientPort = window.m_ClientPort;
				request.m_ServerAddress = window.m_ServerAddress;
				request.m_ServerPort = window.m_ServerPort;

				auto& requestComponent = world.AddEvent<gamestate::ChangeRequest>();
				requestComponent.m_Queue.Append(request);
			}

			if (ImGui::Button("Remote Client"))
			{
				gamestate::NetworkJoin request;
				request.m_Mode = net::EMode::RemoteClient;
				request.m_ClientAddress = window.m_ClientAddress;
				request.m_ClientPort = window.m_ClientPort;
				request.m_ServerAddress = window.m_ServerAddress;
				request.m_ServerPort = window.m_ServerPort;

				auto& requestComponent = world.AddEvent<gamestate::ChangeRequest>();
				requestComponent.m_Queue.Append(request);
			}

			if (ImGui::Button("Listen Server"))
			{
				gamestate::NetworkHost request;
				request.m_Mode = net::EMode::ListenServer;
				request.m_ClientAddress = window.m_ClientAddress;
				request.m_ClientPort = window.m_ClientPort;
				request.m_ServerAddress = window.m_ServerAddress;
				request.m_ServerPort = window.m_ServerPort;

				auto& requestComponent = world.AddEvent<gamestate::ChangeRequest>();
				requestComponent.m_Queue.Append(request);
			}

			if (ImGui::Button("Dedicated Server"))
			{
				gamestate::NetworkHost request;
				request.m_Mode = net::EMode::DedicatedServer;
				request.m_ClientAddress = window.m_ClientAddress;
				request.m_ClientPort = window.m_ClientPort;
				request.m_ServerAddress = window.m_ServerAddress;
				request.m_ServerPort = window.m_ServerPort;

				auto& requestComponent = world.AddEvent<gamestate::ChangeRequest>();
				requestComponent.m_Queue.Append(request);
			}

			if (ImGui::Button("Shutdown"))
			{
				auto& requestComponent = world.AddEvent<gamestate::ChangeRequest>();
				requestComponent.m_Queue.Append(gamestate::NetworkStop{});
			}
			*/
		}
		ImGui::End();

		if (!isOpen)
			world.DestroyEntity(view);
	}
}

void debug::NetworkSystem::OnNetConnectionStatusChanged(SteamNetConnectionStatusChangedCallback_t* pCallback)
{
	HSteamNetConnection hConn = pCallback->m_hConn;
	SteamNetConnectionInfo_t info = pCallback->m_info;
	ESteamNetworkingConnectionState eOldState = pCallback->m_eOldState;
	if (info.m_hListenSocket && eOldState == k_ESteamNetworkingConnectionState_None && info.m_eState == k_ESteamNetworkingConnectionState_Connecting)
	{
		EResult res = SteamGameServerNetworkingSockets()->AcceptConnection(hConn);
		if (res != k_EResultOK)
		{
			Z_LOG(ELog::Network, "AcceptConnection returned {}", static_cast<int32>(res));
			SteamGameServerNetworkingSockets()->CloseConnection(hConn, k_ESteamNetConnectionEnd_AppException_Generic, "Failed to accept connection", false);
			return;
		}

		Z_LOG(ELog::Network, "Client Connected");
		SteamGameServerNetworkingSockets()->SetConnectionPollGroup(hConn, m_NetPollGroup);
		SteamGameServerNetworkingSockets()->SendMessageToConnection(hConn, nullptr, 0, k_nSteamNetworkingSend_Reliable, nullptr);
	}
}

void debug::NetworkSystem::OnSteamServersConnected(SteamServersConnected_t* pCallback)
{
	SteamGameServer()->SetPasswordProtected(false);
	SteamGameServer()->SetServerName("Koala's Game");
	SteamGameServer()->SetMapName("TestLevel");
	SteamGameServer()->SetMaxPlayerCount(4);
}