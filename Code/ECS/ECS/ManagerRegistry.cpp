#include "ECS/ManagerRegistry.h"

#include "ECS/Manager.h"

void ecs::ManagerRegistry::Initialise()
{
	for (ecs::ManagerEntry& entry : m_Entries.GetValues())
	{
		if (!entry.m_IsExternal)
			entry.m_Manager->Initialise();
	}
}

void ecs::ManagerRegistry::Shutdown()
{
	for (ecs::ManagerEntry& entry : m_Entries.GetValues())
	{
		if (!entry.m_IsExternal)
		{
			entry.m_Manager->Shutdown();
			delete entry.m_Manager;
		}
	}
	m_Entries.RemoveAll();
}