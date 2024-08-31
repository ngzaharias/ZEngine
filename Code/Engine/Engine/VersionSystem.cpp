#include "EnginePCH.h"
#include "Engine/VersionSystem.h"

#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/VersionComponent.h"

namespace
{
	const char* strCommand = "git rev-parse HEAD";
}

void eng::VersionSystem::Initialise(World& world)
{
	auto& component = world.WriteSingleton<eng::VersionComponent>();

	// !!!!!DO NOT ALLOW SYSTEM COMMANDS IN SHIPPED GAMES!!!!!
	char buffer[256];
	if (FILE* pipe = _popen(strCommand, "r"))
	{
		while (fgets(buffer, sizeof buffer, pipe) != nullptr)
			component.m_Commit += buffer;
		_pclose(pipe);
	}
}