#include "EnginePCH.h"
#include "Engine/PlatformManager.h"

namespace
{
	constexpr uint32 s_AppId = 3208630;
}

void eng::PlatformManager::Initialise()
{
	if (SteamAPI_Init())
	{
		m_IsConnected = true;
		Z_LOG(ELog::Debug, "Platform: Initialise - Steamworks.");
	}
	else
	{
		m_IsConnected = false;
		Z_LOG(ELog::Debug, "Platform: Failure - Steamworks.");
	}

	GetNumberOfCurrentPlayers();
}

void eng::PlatformManager::Shutdown()
{
	if (m_IsConnected)
	{
		SteamAPI_Shutdown();
		Z_LOG(ELog::Debug, "Platform: Shutdown - Steamworks.");
	}
}

void eng::PlatformManager::Update(const GameTime& gameTime)
{
	SteamAPI_RunCallbacks();
}

void eng::PlatformManager::GetNumberOfCurrentPlayers()
{
	SteamAPICall_t hSteamAPICall = SteamUserStats()->GetNumberOfCurrentPlayers();
	m_NumberOfCurrentPlayersCallResult.Set(hSteamAPICall, this, &PlatformManager::OnGetNumberOfCurrentPlayers);
}

void eng::PlatformManager::OnGetNumberOfCurrentPlayers(NumberOfCurrentPlayers_t* pCallback, bool bIOFailure)
{
	if (bIOFailure || !pCallback->m_bSuccess)
	{
		Z_LOG(ELog::Debug, "NumberOfCurrentPlayers_t failed.");
		return;
	}

	Z_LOG(ELog::Debug, "Number of players currently playing: {}.", pCallback->m_cPlayers);
}

void eng::PlatformManager::OnGameOverlayActivated(GameOverlayActivated_t* pCallback)
{
	if (pCallback->m_bActive)
	{
		Z_LOG(ELog::Debug, "Steam overlay now active.");
	}
	else
	{
		Z_LOG(ELog::Debug, "Steam overlay now inactive.");
	}
}