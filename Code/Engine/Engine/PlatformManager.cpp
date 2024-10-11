#include "EnginePCH.h"
#include "Engine/PlatformManager.h"

namespace
{
	constexpr uint32 s_AppId = 3208630;
}

void eng::PlatformManager::Initialise()
{
	if (m_SteamManager.Initialise())
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
	if (m_SteamManager.Shutdown())
	{
		Z_LOG(ELog::Debug, "Platform: Shutdown - Steamworks.");
	}
}

void eng::PlatformManager::Update(const GameTime& gameTime)
{
	m_SteamManager.Update();
}