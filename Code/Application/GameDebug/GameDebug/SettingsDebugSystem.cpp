#include "GameDebugPCH.h"
#include "GameDebug/SettingsDebugSystem.h"

#include "ClientHidden/HiddenDebugStaticComponent.h"
#include "Core/Path.h"
#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/SettingsDebugStaticComponent.h"
#include "Engine/Visitor.h"
#include "GameClient/SettingsDebugStaticComponent.h"

namespace
{
	const str::StringView strFilename = "DebugSettings.toml";
}

void debug::settings::DebugSystem::Initialise(World& world)
{
	PROFILE_FUNCTION();

	const str::Path filepath = str::Path(str::EPath::AppData, strFilename);

	eng::Visitor visitor;
	visitor.LoadFromFile(filepath);
	visitor.Read(world.WriteComponent<eng::settings::DebugStaticComponent>());
	visitor.Read(world.WriteComponent<client::settings::DebugStaticComponent>());
	visitor.Read(world.WriteComponent<client::hidden::DebugStaticComponent>());
}

void debug::settings::DebugSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	const bool hasChanged =
		world.HasAny<ecs::query::Updated<eng::settings::DebugStaticComponent>>() ||
		world.HasAny<ecs::query::Updated<client::settings::DebugStaticComponent>>() ||
		world.HasAny<ecs::query::Updated<client::hidden::DebugStaticComponent>>();
	if (hasChanged)
	{
		const str::Path filepath = str::Path(str::EPath::AppData, strFilename);

		eng::Visitor visitor;
		visitor.Write(world.ReadComponent<eng::settings::DebugStaticComponent>());
		visitor.Write(world.ReadComponent<client::settings::DebugStaticComponent>());
		visitor.Write(world.ReadComponent<client::hidden::DebugStaticComponent>());
		visitor.SaveToFile(filepath);
	}
}

