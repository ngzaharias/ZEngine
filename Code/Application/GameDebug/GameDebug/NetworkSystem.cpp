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
	bool SendMessage(HSteamNetConnection connection, const void* pData, uint32 nSizeOfData)
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

	if (m_Server != k_HSteamNetConnection_Invalid)
	{
		SteamNetworkingMessage_t* msgs[32];
		int res = SteamNetworkingSockets()->ReceiveMessagesOnConnection(m_Server, msgs, 32);
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
		//int numMessages = SteamGameServerNetworkingSockets()->ReceiveMessagesOnPollGroup(m_NetPollGroup, msgs, 128);
		int numMessages = SteamNetworkingSockets()->ReceiveMessagesOnPollGroup(m_NetPollGroup, msgs, 128);
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

				Z_LOG(ELog::Network, "Server Started");
				m_ListenSocketId = SteamNetworkingSockets()->CreateListenSocketP2P(0, 0, nullptr);
				m_ListenSocketIp = SteamNetworkingSockets()->CreateListenSocketIP(addr, 0, nullptr);
				m_NetPollGroup = SteamNetworkingSockets()->CreatePollGroup();
			}

			if (ImGui::Button("stop server"))
			{
				Z_LOG(ELog::Network, "Server Stopped");
				SteamNetworkingSockets()->CloseListenSocket(m_ListenSocketId);
				SteamNetworkingSockets()->CloseListenSocket(m_ListenSocketIp);
				SteamNetworkingSockets()->DestroyPollGroup(m_NetPollGroup);

				m_ListenSocketId = k_HSteamListenSocket_Invalid;
				m_ListenSocketIp = k_HSteamListenSocket_Invalid;
				m_NetPollGroup = k_HSteamNetPollGroup_Invalid;
			}

			if (ImGui::Button("join server (id)"))
			{
				CSteamID hostId = SteamUser()->GetSteamID();
				SteamNetworkingIdentity identity;
				identity.SetSteamID(hostId);
				m_Server = SteamNetworkingSockets()->ConnectP2P(identity, 0, 0, nullptr);
			}
			ImGui::SameLine();
			if (ImGui::Button("(ip)"))
			{
				SteamNetworkingIPAddr addr{};
				addr.ParseString("127.0.0.1:27020");
				m_Server = SteamNetworkingSockets()->ConnectByIPAddress(addr, 0, nullptr);
			}

			if (ImGui::Button("leave server"))
			{
				SteamNetworkingSockets()->CloseConnection(m_Server, 0, nullptr, false);
				m_Server = k_HSteamNetConnection_Invalid;
			}

			if (ImGui::Button("message to client(s)"))
			{
				for (const HSteamNetConnection client : m_Clients)
					SendMessage(client, nullptr, 0);
			}
			ImGui::SameLine();
			if (ImGui::Button("server"))
			{
				SendMessage(m_Server, nullptr, 0);
			}
		}
		ImGui::End();

		if (!isOpen)
			world.DestroyEntity(view);
	}
}

void debug::NetworkSystem::OnNetConnectionStatusChanged(SteamNetConnectionStatusChangedCallback_t* pCallback)
{
	const HSteamNetConnection connection = pCallback->m_hConn;
	const SteamNetConnectionInfo_t info = pCallback->m_info;
	const ESteamNetworkingConnectionState eOldState = pCallback->m_eOldState;
	const ESteamNetworkingConnectionState eNewState = info.m_eState;

	const bool isConnecting =
		eOldState != k_ESteamNetworkingConnectionState_Connecting &&
		eNewState == k_ESteamNetworkingConnectionState_Connecting;
	if (info.m_hListenSocket && isConnecting)
	{
		const EResult res = SteamNetworkingSockets()->AcceptConnection(connection);
		if (res != k_EResultOK)
		{
			Z_LOG(ELog::Network, "AcceptConnection returned {}", static_cast<int32>(res));
			SteamNetworkingSockets()->CloseConnection(connection, k_ESteamNetConnectionEnd_AppException_Generic, "Failed to accept connection", false);
			return;
		}

		Z_LOG(ELog::Network, "Client Accepted");
		SteamNetworkingSockets()->SetConnectionPollGroup(connection, m_NetPollGroup);
		SteamNetworkingSockets()->SendMessageToConnection(connection, nullptr, 0, k_nSteamNetworkingSend_Reliable, nullptr);
		m_Clients.Append(connection);
	}

	const bool hasConnected =
		eOldState != k_ESteamNetworkingConnectionState_Connected &&
		eNewState == k_ESteamNetworkingConnectionState_Connected;
	if (!info.m_hListenSocket && hasConnected)
	{
		Z_LOG(ELog::Network, "Client Connected");
		SteamNetworkingSockets()->SendMessageToConnection(connection, nullptr, 0, k_nSteamNetworkingSend_Reliable, nullptr);
	}
}