#include "TextureEditorPCH.h"
#include "TextureEditor/TextureEditorSettingsSystem.h"

#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/Visitor.h"
#include "GameState/GameStateEditorComponent.h"
#include "TextureEditor/TextureEditorSettingsComponent.h"

namespace
{
	const str::StringView strFilename = "TextureEditor.toml";
}

void editor::texture::SettingsSystem::Initialise(World& world)
{
	PROFILE_FUNCTION();

	const str::Path filepath = str::Path(str::EPath::AppData, strFilename);

	eng::Visitor visitor;
	visitor.LoadFromFile(filepath);
	visitor.Read(world.WriteComponent<editor::texture::SettingsComponent>());
}

void editor::texture::SettingsSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	if (!world.HasComponent<gamestate::EditorComponent>())
		return;

	if (world.HasAny<ecs::query::Updated<editor::texture::SettingsComponent>>())
	{
		const str::Path filepath = str::Path(str::EPath::AppData, strFilename);

		eng::Visitor visitor;
		visitor.Write(world.ReadComponent<editor::texture::SettingsComponent>());
		visitor.SaveToFile(filepath);
	}
}