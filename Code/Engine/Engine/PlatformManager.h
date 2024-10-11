#pragma once

#include <SteamBinding/Manager.h>

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
		steam::Manager m_SteamManager;
	};
}