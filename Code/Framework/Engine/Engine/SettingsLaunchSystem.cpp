#include "EnginePCH.h"
#include "Engine/SettingsLaunchSystem.h"

#include "Core/Path.h"
#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/LevelComponents.h"
#include "Engine/SettingsComponents.h"
#include "Engine/Visitor.h"

namespace
{
	const str::StringView strFilename = "launch.toml";
}

void eng::settings::LaunchSystem::Initialise(World& world)
{
	const str::Path filepath = str::Path(str::EPath::Levels, strFilename);
	auto& settings = world.WriteSingleton<eng::settings::LaunchComponent>();

	eng::Visitor visitor;
	visitor.LoadFromFile(filepath);
	visitor.Read(settings);

	if (!settings.m_Level.IsEmpty())
	{
		auto& request = world.AddEventComponent<eng::level::LoadRequest>();
		request.m_Name = settings.m_Level;
		request.m_IsSplash = true;
	}
}

