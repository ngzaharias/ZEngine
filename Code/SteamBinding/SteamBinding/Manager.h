#pragma once

#pragma warning(push)
#pragma warning(disable: 4996)
#include <steam_api.h>
#pragma warning(pop)

namespace steam
{
	class Achievements;
}

namespace steam
{
	class Manager final
	{
	public:
		bool Initialise();
		bool Shutdown();

		void Update();

		bool UnlockAchievement(const char* achievementId);

	private:
		steam::Achievements* m_Achievements = nullptr;
		bool m_IsConnected = false;
	};
}