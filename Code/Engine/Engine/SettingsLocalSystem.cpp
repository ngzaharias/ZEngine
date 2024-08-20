#include "EnginePCH.h"
#include "Engine/SettingsLocalSystem.h"

#include "ECS/EntityWorld.h"
#include "ECS/NameComponent.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/AssetManager.h"
#include "Engine/MusicAsset.h"
#include "Engine/MusicComponents.h"
#include "Engine/SettingsComponents.h"
#include "Engine/Visitor.h"

namespace
{
	const str::StringView strAudio = "Audio";
	const str::StringView strEffectVolume = "m_EffectVolume";
	const str::StringView strFilepath = "Settings.toml";
	const str::StringView strMusicVolume = "m_MusicVolume";
}

template<>
void eng::Visitor::VisitCustom<eng::settings::LocalComponent>(eng::settings::LocalComponent& value)
{
	Visit(strEffectVolume, value.m_EffectVolume, value.m_EffectVolume);
	Visit(strMusicVolume, value.m_MusicVolume, value.m_MusicVolume);
}

void eng::settings::LocalSystem::Initialise(World& world)
{
	const str::Path filepath = str::Path(str::EPath::AppData, strFilepath);

	eng::Visitor visitor;
	visitor.LoadFromFile(filepath);
	visitor.Visit(strAudio, world.AddSingleton<eng::settings::LocalComponent>(), {});
}

void eng::settings::LocalSystem::Shutdown(World& world)
{
	const str::Path filepath = str::Path(str::EPath::AppData, strFilepath);

	eng::Visitor visitor;
	visitor.SetMode(eng::Visitor::Write);
	visitor.Visit(strAudio, world.GetSingleton<eng::settings::LocalComponent>(), {});
	visitor.SaveToFile(filepath);

	world.RemoveSingleton<eng::settings::LocalComponent>();
}

void eng::settings::LocalSystem::Update(World& world, const GameTime& gameTime)
{
	if (world.HasAny<ecs::query::Updated<const eng::settings::LocalComponent>>())
	{
		const str::Path filepath = str::Path(str::EPath::AppData, strFilepath);

		// #hack: take a copy so the visitor can use it without us needing to take write access
		auto component = world.GetSingleton<const eng::settings::LocalComponent>();

		eng::Visitor visitor;
		visitor.SetMode(eng::Visitor::Write);
		visitor.Visit(strAudio, world.GetSingleton<eng::settings::LocalComponent>(), {});
		visitor.SaveToFile(filepath);
	}
}

