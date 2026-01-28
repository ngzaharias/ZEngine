#include "TextureEditorPCH.h"
#include "TextureEditor/TextureEditorSettingsSystem.h"

#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/Visitor.h"
#include "TextureEditor/TextureEditorSettingsSingleton.h"

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
	visitor.Read(world.WriteSingleton<editor::texture::SettingsSingleton>());
}

void editor::texture::SettingsSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	if (world.HasAny<editor::texture::SettingsSingleton>())
	{
		const str::Path filepath = str::Path(str::EPath::AppData, strFilename);

		eng::Visitor visitor;
		visitor.Write(world.ReadSingleton<editor::texture::SettingsSingleton>());
		visitor.SaveToFile(filepath);
	}
}