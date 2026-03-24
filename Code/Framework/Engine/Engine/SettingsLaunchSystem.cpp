#include "EnginePCH.h"
#include "Engine/SettingsLaunchSystem.h"

#include "Core/Path.h"
#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/LevelLoadEvent.h"
#include "Engine/SettingsLaunchStaticComponent.h"
#include "Engine/Visitor.h"

namespace
{
	const str::StringView strFilename = "launch.toml";
}

void eng::settings::LaunchSystem::Initialise(World& world)
{
	PROFILE_FUNCTION();

	const str::Path filepath = str::Path(str::EPath::Levels, strFilename);
	auto& settings = world.WriteComponent<eng::settings::LaunchStaticComponent>();

	eng::Visitor visitor;
	visitor.LoadFromFile(filepath);
	visitor.Read(settings);

	if (!settings.m_Level.IsEmpty())
	{
		auto& request = world.AddEvent<eng::level::LoadEvent>();
		request.m_Name = settings.m_Level;
		request.m_IsSplash = true;
	}
}

