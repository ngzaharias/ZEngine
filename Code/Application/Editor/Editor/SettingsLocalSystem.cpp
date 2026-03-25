#include "EditorPCH.h"
#include "Editor/SettingsLocalSystem.h"

#include "Core/Path.h"
#include "ECS/EntityWorld.h"
#include "ECS/NameComponent.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Editor/SettingsLocalComponent.h"
#include "Engine/Visitor.h"

namespace
{
	const str::StringView strFilename = "EditorSettings.toml";
}

void editor::settings::LocalSystem::Initialise(World& world)
{
	PROFILE_FUNCTION();

	const str::Path filepath = str::Path(str::EPath::AppData, strFilename);

	eng::Visitor visitor;
	visitor.LoadFromFile(filepath);
	visitor.Read(world.WriteComponent<editor::settings::LocalComponent>());
}

void editor::settings::LocalSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	if (world.HasAny<ecs::query::Updated<editor::settings::LocalComponent>>())
	{
		const str::Path filepath = str::Path(str::EPath::AppData, strFilename);

		eng::Visitor visitor;
		visitor.Write(world.ReadComponent<editor::settings::LocalComponent>());
		visitor.SaveToFile(filepath);
	}
}

