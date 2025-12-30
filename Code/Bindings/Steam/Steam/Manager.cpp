#include "Steam/Manager.h"

#include "Core/Profiler.h"
#include "Steam/Achievements.h"

bool steam::Manager::Initialise()
{
	PROFILE_FUNCTION();

	if (SteamAPI_Init())
	{
		m_IsConnected = true;
		m_Achievements = new steam::Achievements();
	}

	return m_IsConnected;
}

bool steam::Manager::Shutdown()
{
	PROFILE_FUNCTION();

	if (!m_IsConnected)
		return false;

	m_IsConnected = false;
	SteamAPI_Shutdown();

	delete m_Achievements;
	m_Achievements = nullptr;

	return true;
}

void steam::Manager::Update()
{
	PROFILE_FUNCTION();

	SteamAPI_RunCallbacks();
}

bool steam::Manager::UnlockAchievement(const char* achievementId)
{
	if (!m_Achievements)
		return false;

	return m_Achievements->UnlockAchievement(achievementId);
}
