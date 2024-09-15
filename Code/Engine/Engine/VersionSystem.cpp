#include "EnginePCH.h"
#include "Engine/VersionSystem.h"

#include "Core/StringHelpers.h"
#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/VersionComponent.h"

namespace
{
	const char* strBranch = "git rev-parse --abbrev-ref HEAD";
	const char* strCommit = "git rev-parse HEAD";
}

void eng::VersionSystem::Initialise(World& world)
{
	auto& component = world.WriteSingleton<eng::VersionComponent>();

	// #todo: do not allow system commands in shipped games
	char buffer[256];
	if (FILE* pipe = _popen(strBranch, "r"))
	{
		while (fgets(buffer, sizeof buffer, pipe) != nullptr)
			component.m_Branch += buffer;
		_pclose(pipe);
		str::TrimRight(component.m_Branch, "\n");
	}

	if (FILE* pipe = _popen(strCommit, "r"))
	{
		while (fgets(buffer, sizeof buffer, pipe) != nullptr)
			component.m_Commit += buffer;
		_pclose(pipe);
	}
}