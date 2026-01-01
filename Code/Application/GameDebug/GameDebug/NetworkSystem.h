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

		void Initialise(World& world);
		
		void Update(World& world, const GameTime& gameTime);

	private:
		STEAM_GAMESERVER_CALLBACK(NetworkSystem, OnNetConnectionStatusChanged, SteamNetConnectionStatusChangedCallback_t);
		STEAM_GAMESERVER_CALLBACK(NetworkSystem, OnSteamServersConnected, SteamServersConnected_t);

	private:
		HSteamNetConnection m_Connection = k_HSteamNetConnection_Invalid;
		HSteamListenSocket m_ListenSocket = k_HSteamListenSocket_Invalid;
		HSteamNetPollGroup m_NetPollGroup = k_HSteamNetPollGroup_Invalid;
		imgui::Identifier m_WindowIds = {};
	};
}