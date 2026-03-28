#include "GameDebugPCH.h"
#include "GameDebug/SettingsDebugSystem.h"

#include "ClientHidden/HiddenDebugComponent.h"
#include "Core/Path.h"
#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/SettingsDebugComponent.h"
#include "GameClient/SettingsDebugComponent.h"
#include "Serialize/Visitor.h"

namespace
{
	const str::StringView strFilename = "DebugSettings.toml";
}

void debug::settings::DebugSystem::Initialise(World& world)
{
	PROFILE_FUNCTION();

	const str::Path filepath = str::Path(str::EPath::AppData, strFilename);

	Visitor visitor;
	visitor.LoadFromFile(filepath);
	visitor.Read(world.WriteComponent<eng::settings::DebugComponent>());
	visitor.Read(world.WriteComponent<client::settings::DebugComponent>());
	visitor.Read(world.WriteComponent<client::hidden::DebugComponent>());
}

void debug::settings::DebugSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	const bool hasChanged =
		world.HasAny<ecs::query::Updated<eng::settings::DebugComponent>>() ||
		world.HasAny<ecs::query::Updated<client::settings::DebugComponent>>() ||
		world.HasAny<ecs::query::Updated<client::hidden::DebugComponent>>();
	if (hasChanged)
	{
		const str::Path filepath = str::Path(str::EPath::AppData, strFilename);

		Visitor visitor;
		visitor.Write(world.ReadComponent<eng::settings::DebugComponent>());
		visitor.Write(world.ReadComponent<client::settings::DebugComponent>());
		visitor.Write(world.ReadComponent<client::hidden::DebugComponent>());
		visitor.SaveToFile(filepath);
	}
}

