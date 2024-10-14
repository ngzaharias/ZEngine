#pragma once

class GameTime;

namespace eng
{
	struct Achievement;
}

namespace steam
{
	class Manager;
}

namespace eng
{
	class PlatformManager final
	{
	public:
		void Initialise();
		void Shutdown();

		void Update(const GameTime& gameTime);

		bool UnlockAchievement(const eng::Achievement& achievement);

	private:
		steam::Manager* m_SteamManager = nullptr;
	};
}