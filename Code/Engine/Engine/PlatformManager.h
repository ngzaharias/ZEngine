#pragma once

#pragma warning(push)
#pragma warning(disable: 4996)
#include <SteamworksSDK/Include/steam_api.h>
#pragma warning(pop)

class GameTime;

namespace eng
{
	class PlatformManager final
	{
	public:
		void Initialise();
		void Shutdown();

		void Update(const GameTime& gameTime);

	private:
		void GetNumberOfCurrentPlayers();
		void OnGetNumberOfCurrentPlayers(NumberOfCurrentPlayers_t* pCallback, bool bIOFailure);
		CCallResult<PlatformManager, NumberOfCurrentPlayers_t > m_NumberOfCurrentPlayersCallResult;

		STEAM_CALLBACK(PlatformManager, OnGameOverlayActivated, GameOverlayActivated_t);

	private:
		bool m_IsConnected = false;
	};
}