#include "EntityEditorPCH.h"
#include "EntityEditor/EntityEditorSettingsSystem.h"

#include "Core/Path.h"
#include "ECS/EntityWorld.h"
#include "ECS/NameComponent.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/Visitor.h"
#include "EntityEditor/EntityEditorSettingsSingleton.h"
#include "GameState/GameStateEditorComponent.h"

namespace
{
	const str::StringView strFilename = "EntityEditor.toml";
}

void editor::entity::SettingsSystem::Initialise(World& world)
{
	PROFILE_FUNCTION();

	const str::Path filepath = str::Path(str::EPath::AppData, strFilename);

	eng::Visitor visitor;
	visitor.LoadFromFile(filepath);
	visitor.Read(world.WriteSingleton<editor::entity::SettingsSingleton>());
}

void editor::entity::SettingsSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	if (!world.HasAny<ecs::query::Include<gamestate::EditorComponent>>())
		return;

	if (world.HasAny<editor::entity::SettingsSingleton>())
	{
		const str::Path filepath = str::Path(str::EPath::AppData, strFilename);

		eng::Visitor visitor;
		visitor.Write(world.ReadSingleton<editor::entity::SettingsSingleton>());
		visitor.SaveToFile(filepath);
	}
}

