#include "FlipbookEditorPCH.h"
#include "FlipbookEditor/FlipbookEditorSettingsSystem.h"

#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "FlipbookEditor/FlipbookEditorSettingsComponent.h"
#include "GameState/GameStateEditorComponent.h"
#include "Serialize/Visitor.h"

namespace
{
	const str::StringView strFilename = "FlipbookEditor.toml";
}

void editor::flipbook::SettingsSystem::Initialise(World& world)
{
	PROFILE_FUNCTION();

	const str::Path filepath = str::Path(str::EPath::AppData, strFilename);

	Visitor visitor;
	visitor.LoadFromFile(filepath);
	visitor.Read(world.WriteComponent<editor::flipbook::SettingsComponent>());
}

void editor::flipbook::SettingsSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	if (!world.HasComponent<gamestate::EditorComponent>())
		return;

	if (world.HasAny<ecs::query::Updated<editor::flipbook::SettingsComponent>>())
	{
		const str::Path filepath = str::Path(str::EPath::AppData, strFilename);

		Visitor visitor;
		visitor.Write(world.ReadComponent<editor::flipbook::SettingsComponent>());
		visitor.SaveToFile(filepath);
	}
}