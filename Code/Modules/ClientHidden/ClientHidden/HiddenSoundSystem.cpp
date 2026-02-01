#include "ClientHiddenPCH.h"
#include "ClientHidden/HiddenSoundSystem.h"

#include "ClientHidden/HiddenRevealComponent.h"
#include "ECS/EntityWorld.h"
#include "ECS/NameComponent.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/SoundSequenceRequestComponent.h"

namespace
{
	const str::Guid strSoundSequence = GUID("2bde153c851a429c88957b6b0fd482fb");
}

void client::hidden::SoundSystem::Initialise(World& world)
{
	PROFILE_FUNCTION();

	m_SoundEntity = world.CreateEntity();
	world.AddComponent<ecs::NameComponent>(m_SoundEntity, "Hidden Sound");
}

void client::hidden::SoundSystem::Shutdown(World& world)
{
	PROFILE_FUNCTION();

	world.DestroyEntity(m_SoundEntity);
}

void client::hidden::SoundSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	// #crash:
	for (auto&& view : world.Query<ecs::query::Added<const client::hidden::RevealComponent>::Exclude<const eng::SavegameComponent>>())
	{
		auto& soundComponent = world.AddComponent<eng::sound::SequenceRequestComponent>(m_SoundEntity);
		soundComponent.m_Asset = strSoundSequence;
	}

	// #crash:
	for (auto&& view : world.Query<ecs::query::Include<const eng::sound::SequenceRequestComponent>>())
		world.RemoveComponent<eng::sound::SequenceRequestComponent>(m_SoundEntity);
}