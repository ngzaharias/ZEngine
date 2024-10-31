#include "EnginePCH.h"
#include "Engine/SettingsDebugSystem.h"

#include "ECS/EntityWorld.h"
#include "ECS/NameComponent.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/AssetManager.h"
#include "Engine/MusicAsset.h"
#include "Engine/MusicComponents.h"
#include "Engine/SettingsComponents.h"
#include "Engine/Visitor.h"

namespace
{
	const str::StringView strFilename = "DebugSettings.toml";
}

void eng::settings::DebugSystem::Initialise(World& world)
{
	const str::Path filepath = str::Path(str::EPath::AppData, strFilename);

	eng::Visitor visitor;
	visitor.LoadFromFile(filepath);
	visitor.Read(world.WriteSingleton<eng::settings::DebugComponent>());
}

void eng::settings::DebugSystem::Update(World& world, const GameTime& gameTime)
{
	if (world.HasAny<ecs::query::Updated<const eng::settings::DebugComponent>>())
	{
		const str::Path filepath = str::Path(str::EPath::AppData, strFilename);

		eng::Visitor visitor;
		visitor.Write(world.ReadSingleton<eng::settings::DebugComponent>());
		visitor.SaveToFile(filepath);
	}
}

