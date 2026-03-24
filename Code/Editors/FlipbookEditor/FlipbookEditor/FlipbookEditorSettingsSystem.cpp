#include "FlipbookEditorPCH.h"
#include "FlipbookEditor/FlipbookEditorSettingsSystem.h"

#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/Visitor.h"
#include "FlipbookEditor/FlipbookEditorSettingsStaticComponent.h"
#include "GameState/GameStateEditorComponent.h"

namespace
{
	const str::StringView strFilename = "FlipbookEditor.toml";
}

void editor::flipbook::SettingsSystem::Initialise(World& world)
{
	PROFILE_FUNCTION();

	const str::Path filepath = str::Path(str::EPath::AppData, strFilename);

	eng::Visitor visitor;
	visitor.LoadFromFile(filepath);
	visitor.Read(world.WriteComponent<editor::flipbook::SettingsStaticComponent>());
}

void editor::flipbook::SettingsSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	if (!world.HasAny<ecs::query::Include<gamestate::EditorComponent>>())
		return;

	if (world.HasAny<ecs::query::Updated<editor::flipbook::SettingsStaticComponent>>())
	{
		const str::Path filepath = str::Path(str::EPath::AppData, strFilename);

		eng::Visitor visitor;
		visitor.Write(world.ReadComponent<editor::flipbook::SettingsStaticComponent>());
		visitor.SaveToFile(filepath);
	}
}