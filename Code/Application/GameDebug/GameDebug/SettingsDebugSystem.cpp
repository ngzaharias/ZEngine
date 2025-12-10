#include "GameDebugPCH.h"
#include "GameDebug/SettingsDebugSystem.h"

#include "Core/Path.h"
#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/SettingsDebugSingleton.h"
#include "Engine/Visitor.h"
#include "GameClient/SettingsDebugComponent.h"
#include "Hidden/HiddenDebugSettingsSingleton.h"

namespace
{
	const str::StringView strFilename = "DebugSettings.toml";
}

void debug::settings::DebugSystem::Initialise(World& world)
{
	const str::Path filepath = str::Path(str::EPath::AppData, strFilename);

	eng::Visitor visitor;
	visitor.LoadFromFile(filepath);
	visitor.Read(world.WriteSingleton<clt::settings::DebugSingleton>());
	visitor.Read(world.WriteSingleton<eng::settings::DebugSingleton>());
	visitor.Read(world.WriteSingleton<hidden::settings::DebugSingleton>());

#ifdef Z_EDITOR
	auto& component = world.WriteSingleton<eng::settings::DebugSingleton>();
	component.m_IsEditorModeEnabled = true;
#endif
}

void debug::settings::DebugSystem::Update(World& world, const GameTime& gameTime)
{
	const bool hasChanged =
		world.HasAny<clt::settings::DebugSingleton>() ||
		world.HasAny<eng::settings::DebugSingleton>() ||
		world.HasAny<hidden::settings::DebugSingleton>();
	if (hasChanged)
	{
		const str::Path filepath = str::Path(str::EPath::AppData, strFilename);

		eng::Visitor visitor;
		visitor.Write(world.ReadSingleton<clt::settings::DebugSingleton>());
		visitor.Write(world.ReadSingleton<eng::settings::DebugSingleton>());
		visitor.Write(world.ReadSingleton<hidden::settings::DebugSingleton>());
		visitor.SaveToFile(filepath);
	}
}

