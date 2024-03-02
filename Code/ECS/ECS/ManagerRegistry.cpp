#include "ECS/ManagerRegistry.h"

#include <Core/Profiler.h>

#include "ECS/Manager.h"

void ecs::ManagerRegistry::Initialise()
{
	PROFILE_FUNCTION();

	for (ecs::ManagerEntry& entry : m_Entries.GetValues())
		entry.m_Manager->Initialise();
}

void ecs::ManagerRegistry::Shutdown()
{
	PROFILE_FUNCTION();

	for (ecs::ManagerEntry& entry : m_Entries.GetValues())
	{
		entry.m_Manager->Shutdown();
		delete entry.m_Manager;
	}
	m_Entries.RemoveAll();
}