#pragma once

#pragma warning(push)
#pragma warning(disable: 4996)
#include "Steam/steam_api.h"
#pragma warning(pop)

namespace steam
{
	class Achievements final
	{
	public:
		Achievements();

		bool UnlockAchievement(const char* achievementId);

	private:
		bool RequestStats();

		STEAM_CALLBACK(Achievements, OnUserStatsReceived, UserStatsReceived_t, m_CallbackUserStatsReceived);
		STEAM_CALLBACK(Achievements, OnUserStatsStored, UserStatsStored_t, m_CallbackUserStatsStored);
		STEAM_CALLBACK(Achievements, OnAchievementStored, UserAchievementStored_t, m_CallbackAchievementStored);

	private:
		int64 m_AppId = -1;
		bool m_IsInitialized = false;
	};
}