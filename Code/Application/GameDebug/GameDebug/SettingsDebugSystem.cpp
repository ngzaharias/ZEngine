#include "GameDebugPCH.h"
#include "GameDebug/SettingsDebugSystem.h"

#include "Core/Path.h"
#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/SettingsComponents.h"
#include "Engine/Visitor.h"
#include "GameClient/SettingsComponents.h"
#include "Hidden/HiddenDebugSettingsComponent.h"

namespace
{
	const str::StringView strFilename = "DebugSettings.toml";
}

void dbg::settings::DebugSystem::Initialise(World& world)
{
	const str::Path filepath = str::Path(str::EPath::AppData, strFilename);

	eng::Visitor visitor;
	visitor.LoadFromFile(filepath);
	visitor.Read(world.WriteSingleton<clt::settings::DebugComponent>());
	visitor.Read(world.WriteSingleton<eng::settings::DebugComponent>());
	visitor.Read(world.WriteSingleton<hidden::settings::DebugComponent>());

#ifdef Z_EDITOR
	auto& component = world.WriteSingleton<eng::settings::DebugComponent>();
	component.m_IsEditorModeEnabled = true;
#endif
}

void dbg::settings::DebugSystem::Update(World& world, const GameTime& gameTime)
{
	const bool wasClient = world.HasAny<ecs::query::Updated<clt::settings::DebugComponent>>();
	const bool wasEngine = world.HasAny<ecs::query::Updated<eng::settings::DebugComponent>>();
	const bool wasHidden = world.HasAny<ecs::query::Updated<hidden::settings::DebugComponent>>();
	if (wasClient || wasEngine || wasHidden)
	{
		const str::Path filepath = str::Path(str::EPath::AppData, strFilename);

		eng::Visitor visitor;
		visitor.Write(world.ReadSingleton<clt::settings::DebugComponent>());
		visitor.Write(world.ReadSingleton<eng::settings::DebugComponent>());
		visitor.Write(world.ReadSingleton<hidden::settings::DebugComponent>());
		visitor.SaveToFile(filepath);
	}
}

