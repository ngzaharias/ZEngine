#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"
#include "imgui/Identifier.h"

#pragma warning(push)
#pragma warning(disable: 4996)
#include <steam_api.h>
#include <isteamgameserver.h>
#include <steam_gameserver.h>
#pragma warning(pop)

namespace gamestate
{
	struct ChangeRequest;
}

namespace ecs
{
	struct NameComponent;
}

namespace debug
{
	struct NetworkWindowComponent;
	struct NetworkWindowRequest;

	struct NetworkLobby
	{
		CSteamID m_SteamId = {};
		str::String m_Name = {};
	};

	class NetworkSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			debug::NetworkWindowComponent,
			ecs::NameComponent,
			gamestate::ChangeRequest>
			::Read<
			debug::NetworkWindowRequest>;

		NetworkSystem();

		void Initialise(World& world);
		
		void Update(World& world, const GameTime& gameTime);

	private:
		STEAM_GAMESERVER_CALLBACK(NetworkSystem, OnNetConnectionStatusChanged, SteamNetConnectionStatusChangedCallback_t);
		STEAM_GAMESERVER_CALLBACK(NetworkSystem, OnSteamServersConnected, SteamServersConnected_t);

		void OnLobbyCreated(LobbyCreated_t* pCallback, bool bIOFailure);
		CCallResult<NetworkSystem, LobbyCreated_t> m_SteamCallResultLobbyCreated;

		CCallResult<NetworkSystem, LobbyMatchList_t> m_SteamCallResultLobbyMatchList;
		void OnLobbyMatchListCallback(LobbyMatchList_t* pLobbyMatchList, bool bIOFailure);
		STEAM_CALLBACK(NetworkSystem, OnLobbyDataUpdatedCallback, LobbyDataUpdate_t, m_CallbackLobbyDataUpdated);
		STEAM_CALLBACK(NetworkSystem, OnLobbyEntered, LobbyEnter_t);

	private:
		CSteamID m_LobbyId = {};
		HSteamNetConnection m_Connection = k_HSteamNetConnection_Invalid;
		HSteamListenSocket m_ListenSocket = k_HSteamListenSocket_Invalid;
		HSteamNetPollGroup m_NetPollGroup = k_HSteamNetPollGroup_Invalid;

		imgui::Identifier m_WindowIds = {};
		Array<NetworkLobby> m_Lobbies = {};
	};
}