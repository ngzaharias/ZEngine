#include "InspectorPCH.h"
#include "Inspector/InspectorSettingsSystem.h"

#include "Core/Path.h"
#include "ECS/EntityWorld.h"
#include "ECS/NameComponent.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/Visitor.h"
#include "GameState/GameStateEditorComponent.h"
#include "Inspector/InspectorSettingsSingleton.h"

namespace
{
	const str::StringView strFilename = "Inspector.toml";
}

void editor::inspector::SettingsSystem::Initialise(World& world)
{
	PROFILE_FUNCTION();

	const str::Path filepath = str::Path(str::EPath::AppData, strFilename);

	eng::Visitor visitor;
	visitor.LoadFromFile(filepath);
	visitor.Read(world.WriteSingleton<editor::inspector::SettingsSingleton>());
}

void editor::inspector::SettingsSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	if (!world.HasAny<ecs::query::Include<gamestate::EditorComponent>>())
		return;

	if (world.HasAny<editor::inspector::SettingsSingleton>())
	{
		const str::Path filepath = str::Path(str::EPath::AppData, strFilename);

		eng::Visitor visitor;
		visitor.Write(world.ReadSingleton<editor::inspector::SettingsSingleton>());
		visitor.SaveToFile(filepath);
	}
}

