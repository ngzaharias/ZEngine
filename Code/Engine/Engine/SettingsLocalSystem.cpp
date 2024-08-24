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
	const str::StringView strFilepath = "Settings.toml";

	const str::StringView strAudio = "m_Audio";
	const str::StringView strCamera = "m_Camera";
	const str::StringView strEffectVolume = "m_EffectVolume";
	const str::StringView strMusicVolume = "m_MusicVolume";
	const str::StringView strZoomSpeed = "m_ZoomSpeed";
}

template<>
void eng::Visitor::ReadCustom(eng::settings::LocalComponent& value) const
{
	Read(strAudio, value.m_Audio, value.m_Audio);
	Read(strCamera, value.m_Camera, value.m_Camera);
}

template<>
void eng::Visitor::ReadCustom(eng::settings::Audio& value) const
{
	Read(strEffectVolume, value.m_EffectVolume, value.m_EffectVolume);
	Read(strMusicVolume, value.m_MusicVolume, value.m_MusicVolume);
}

template<>
void eng::Visitor::ReadCustom(eng::settings::Camera& value) const
{
	Read(strZoomSpeed, value.m_ZoomSpeed, value.m_ZoomSpeed);
}

template<>
void eng::Visitor::WriteCustom(const eng::settings::LocalComponent& value)
{
	Write(strAudio, value.m_Audio);
	Write(strCamera, value.m_Camera);
}

template<>
void eng::Visitor::WriteCustom(const eng::settings::Audio& value)
{
	Write(strEffectVolume, value.m_EffectVolume);
	Write(strMusicVolume, value.m_MusicVolume);
}

template<>
void eng::Visitor::WriteCustom(const eng::settings::Camera& value)
{
	Write(strZoomSpeed, value.m_ZoomSpeed);
}

void eng::settings::LocalSystem::Initialise(World& world)
{
	const str::Path filepath = str::Path(str::EPath::AppData, strFilepath);

	eng::Visitor visitor;
	visitor.LoadFromFile(filepath);
	visitor.Read(world.WriteSingleton<eng::settings::LocalComponent>());
}

void eng::settings::LocalSystem::Update(World& world, const GameTime& gameTime)
{
	if (world.HasAny<ecs::query::Updated<const eng::settings::LocalComponent>>())
	{
		const str::Path filepath = str::Path(str::EPath::AppData, strFilepath);

		eng::Visitor visitor;
		visitor.Write(world.ReadSingleton<eng::settings::LocalComponent>());
		visitor.SaveToFile(filepath);
	}
}

