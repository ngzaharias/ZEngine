#include "SteamBinding/Manager.h"

bool steam::Manager::Initialise()
{
	m_IsConnected = SteamAPI_Init();
	return m_IsConnected;
}

bool steam::Manager::Shutdown()
{
	if (!m_IsConnected)
		return false;

	m_IsConnected = false;
	SteamAPI_Shutdown();
	return true;
}

void steam::Manager::Update()
{
	SteamAPI_RunCallbacks();
}