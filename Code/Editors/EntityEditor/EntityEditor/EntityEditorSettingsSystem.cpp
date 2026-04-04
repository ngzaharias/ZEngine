#include "EntityEditorPCH.h"
#include "EntityEditor/EntityEditorSettingsSystem.h"

#include "Core/Path.h"
#include "ECS/EntityWorld.h"
#include "ECS/NameComponent.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "GameState/GameStateEditorComponent.h"
#include "EntityEditor/EntityEditorSettingsComponent.h"
#include "Serialize/Visitor.h"

namespace
{
	const str::StringView strFilename = "EntityEditor.toml";
}

void editor::entity::SettingsSystem::Initialise(World& world)
{
	PROFILE_FUNCTION();

	const str::Path filepath = str::Path(str::EPath::AppData, strFilename);

	Visitor visitor;
	visitor.LoadFromFile(filepath);
	visitor.Read(world.WriteComponent<editor::entity::SettingsComponent>());
}

void editor::entity::SettingsSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	if (!world.HasComponent<gamestate::EditorComponent>())
		return;

	if (world.HasAny<ecs::query::Updated<editor::entity::SettingsComponent>>())
	{
		const str::Path filepath = str::Path(str::EPath::AppData, strFilename);

		Visitor visitor;
		visitor.Write(world.ReadComponent<editor::entity::SettingsComponent>());
		visitor.SaveToFile(filepath);
	}
}

