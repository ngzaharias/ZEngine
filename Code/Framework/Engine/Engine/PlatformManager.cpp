#include "EnginePCH.h"
#include "Engine/PlatformManager.h"

#include "Core/Name.h"
#include "Engine/AchievementTable.h"
#include "Steam/Manager.h"

void eng::PlatformManager::Initialise()
{
	PROFILE_FUNCTION();

	m_SteamManager = new steam::Manager();
	if (m_SteamManager->Initialise())
	{
		Z_LOG(ELog::Debug, "Platform: Initialise - Steamworks.");
	}
	else
	{
		Z_LOG(ELog::Debug, "Platform: Failure - Steamworks.");
	}
}

void eng::PlatformManager::Shutdown()
{
	PROFILE_FUNCTION();

	if (m_SteamManager->Shutdown())
	{
		Z_LOG(ELog::Debug, "Platform: Shutdown - Steamworks.");
	}

	delete m_SteamManager;
	m_SteamManager = nullptr;
}

void eng::PlatformManager::Update(const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	m_SteamManager->Update();
}

bool eng::PlatformManager::UnlockAchievement(const eng::Achievement& achievement)
{
	return m_SteamManager->UnlockAchievement(achievement.m_Identifier.ToChar());
}