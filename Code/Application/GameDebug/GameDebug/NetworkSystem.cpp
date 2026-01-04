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

debug::NetworkSystem::NetworkSystem()
	: m_CallbackLobbyDataUpdated(this, &NetworkSystem::OnLobbyDataUpdatedCallback)
{
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
		const auto& window = view.ReadRequired<debug::NetworkWindowComponent>();
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
					Z_LOG(ELog::Network, "Server Started");

					SteamGameServer()->LogOnAnonymous();
					//m_ListenSocket = SteamGameServerNetworkingSockets()->CreateListenSocketIP(addr, 0, nullptr);
					m_ListenSocket = SteamGameServerNetworkingSockets()->CreateListenSocketP2P(0, 0, nullptr);
					m_NetPollGroup = SteamGameServerNetworkingSockets()->CreatePollGroup();
				}
			}

			if (ImGui::Button("stop server"))
			{
				Z_LOG(ELog::Network, "Server Stopped");
				SteamNetworkingSockets()->CloseListenSocket(m_ListenSocket);
				SteamNetworkingSockets()->DestroyPollGroup(m_NetPollGroup);
				SteamGameServer()->LogOff();
				SteamGameServer_Shutdown();

				m_NetPollGroup = k_HSteamNetPollGroup_Invalid;
				m_ListenSocket = k_HSteamListenSocket_Invalid;
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
				SteamNetworkingSockets()->CloseConnection(m_Connection, 0, nullptr, false);
				m_Connection = k_HSteamNetConnection_Invalid;
			}

			if (ImGui::Button("message to server"))
			{
				SendServerData(m_Connection, nullptr, 0);
			}

			ImGui::Separator();
			ImGui::Separator();
			ImGui::Separator();

			if (ImGui::Button("start lobby"))
			{
				Z_LOG(ELog::Network, "Lobby Started");
				SteamAPICall_t hSteamAPICall = SteamMatchmaking()->CreateLobby(k_ELobbyTypePublic, 4);
				m_SteamCallResultLobbyCreated.Set(hSteamAPICall, this, &NetworkSystem::OnLobbyCreated);
			}

			if (ImGui::Button("find lobby"))
			{
				SteamAPICall_t hSteamAPICall = SteamMatchmaking()->RequestLobbyList();
				m_SteamCallResultLobbyMatchList.Set(hSteamAPICall, this, &debug::NetworkSystem::OnLobbyMatchListCallback);
			}

			if (ImGui::CollapsingHeader("join lobby"))
			{
				ImGui::Indent();
				for (const NetworkLobby& lobby : m_Lobbies)
				{
					if (ImGui::Button(lobby.m_Name.c_str()))
						SteamMatchmaking()->JoinLobby(lobby.m_SteamId);
				}
				ImGui::Unindent();
			}

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

void debug::NetworkSystem::OnLobbyCreated(LobbyCreated_t* pCallback, bool bIOFailure)
{
	if (pCallback->m_eResult == k_EResultOK)
	{
		m_LobbyId = pCallback->m_ulSteamIDLobby;
		SteamMatchmaking()->SetLobbyData(m_LobbyId, "name", "My Lobby");
	}
}

void debug::NetworkSystem::OnLobbyMatchListCallback(LobbyMatchList_t* pCallback, bool bIOFailure)
{
	m_Lobbies.RemoveAll();

	// lobbies are returned in order of closeness to the user, so add them to the list in that order
	for (uint32 iLobby = 0; iLobby < pCallback->m_nLobbiesMatching; iLobby++)
	{
		NetworkLobby& lobby = m_Lobbies.Emplace();
		lobby.m_SteamId = SteamMatchmaking()->GetLobbyByIndex(iLobby);
		lobby.m_Name = SteamMatchmaking()->GetLobbyData(lobby.m_SteamId, "name");

		if (lobby.m_Name.empty())
			SteamMatchmaking()->RequestLobbyData(lobby.m_SteamId);
	}
}

void debug::NetworkSystem::OnLobbyDataUpdatedCallback(LobbyDataUpdate_t* pCallback)
{
	for (NetworkLobby& lobby : m_Lobbies)
	{
		if (pCallback->m_ulSteamIDLobby == lobby.m_SteamId)
		{
			lobby.m_Name = SteamMatchmaking()->GetLobbyData(lobby.m_SteamId, "name");
			break;
		}
	}
}

void debug::NetworkSystem::OnLobbyEntered(LobbyEnter_t* cb)
{
	if (cb->m_EChatRoomEnterResponse != k_EChatRoomEnterResponseSuccess)
	{
		Z_LOG(ELog::Network, "Failed to join lobby.");
		return;
	}

	const CSteamID lobbyId = cb->m_ulSteamIDLobby;
	const CSteamID hostId = SteamMatchmaking()->GetLobbyOwner(lobbyId);

	Z_LOG(ELog::Network, "Joined lobby {}.", lobbyId.ConvertToUint64());
	Z_LOG(ELog::Network, "Lobby host {}.", hostId.ConvertToUint64());
}