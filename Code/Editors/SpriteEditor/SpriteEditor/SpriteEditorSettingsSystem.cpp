#include "SpriteEditorPCH.h"
#include "SpriteEditor/SpriteEditorSettingsSystem.h"

#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/Visitor.h"
#include "GameState/GameStateEditorComponent.h"
#include "SpriteEditor/SpriteEditorSettingsComponent.h"

namespace
{
	const str::StringView strFilename = "SpriteEditor.toml";
}

void editor::sprite::SettingsSystem::Initialise(World& world)
{
	PROFILE_FUNCTION();

	const str::Path filepath = str::Path(str::EPath::AppData, strFilename);

	eng::Visitor visitor;
	visitor.LoadFromFile(filepath);
	visitor.Read(world.WriteComponent<editor::sprite::SettingsComponent>());
}

void editor::sprite::SettingsSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	if (!world.HasComponent<gamestate::EditorComponent>())
		return;

	if (world.HasAny<ecs::query::Updated<editor::sprite::SettingsComponent>>())
	{
		const str::Path filepath = str::Path(str::EPath::AppData, strFilename);

		eng::Visitor visitor;
		visitor.Write(world.ReadComponent<editor::sprite::SettingsComponent>());
		visitor.SaveToFile(filepath);
	}
}