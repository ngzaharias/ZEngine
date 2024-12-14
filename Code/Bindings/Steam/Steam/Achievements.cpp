#include "Steam/Achievements.h"

steam::Achievements::Achievements()
	: m_CallbackUserStatsReceived(this, &Achievements::OnUserStatsReceived)
	, m_CallbackUserStatsStored(this, &Achievements::OnUserStatsStored)
	, m_CallbackAchievementStored(this, &Achievements::OnAchievementStored)
{
	m_AppId = SteamUtils()->GetAppID();
	RequestStats();
}

bool steam::Achievements::RequestStats()
{
	ISteamUser* steamUser = SteamUser();
	ISteamUserStats* userStats = SteamUserStats();
	if (!steamUser || !userStats)
		return false;

	if (!steamUser->BLoggedOn())
		return false;

	return userStats->RequestCurrentStats();
}

bool steam::Achievements::UnlockAchievement(const char* achievementId)
{
	if (!m_IsInitialized)
		return false;

	ISteamUserStats* userStats = SteamUserStats();
	userStats->SetAchievement(achievementId);
	return userStats->StoreStats();
}

void steam::Achievements::OnUserStatsReceived(UserStatsReceived_t* pCallback)
{
	if (!pCallback->m_nGameID == m_AppId)
		return;

	m_IsInitialized = pCallback->m_eResult == k_EResultOK;
}

void steam::Achievements::OnUserStatsStored(UserStatsStored_t* pCallback)
{
	if (!pCallback->m_nGameID == m_AppId)
		return;
}

void steam::Achievements::OnAchievementStored(UserAchievementStored_t* pCallback)
{
	if (!pCallback->m_nGameID == m_AppId)
		return;
}