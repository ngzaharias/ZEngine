#include "FlipbookEditorPCH.h"
#include "FlipbookEditor/FlipbookEditorSettingsSystem.h"

#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "FlipbookEditor/FlipbookEditorSettingsSingleton.h"

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
	visitor.Read(world.WriteSingleton<editor::flipbook::SettingsSingleton>());
}

void editor::flipbook::SettingsSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	if (world.HasAny<editor::flipbook::SettingsSingleton>())
	{
		const str::Path filepath = str::Path(str::EPath::AppData, strFilename);

		eng::Visitor visitor;
		visitor.Write(world.ReadSingleton<editor::flipbook::SettingsSingleton>());
		visitor.SaveToFile(filepath);
	}
}