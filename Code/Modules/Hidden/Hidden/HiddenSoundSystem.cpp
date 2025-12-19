#include "HiddenPCH.h"
#include "Hidden/HiddenSoundSystem.h"

#include "ECS/EntityWorld.h"
#include "ECS/NameComponent.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/SoundSequenceRequestComponent.h"
#include "Hidden/HiddenRevealComponent.h"

namespace
{
	const str::Guid strSoundSequence = GUID("2bde153c851a429c88957b6b0fd482fb");
}

void hidden::SoundSystem::Initialise(World& world)
{
	m_SoundEntity = world.CreateEntity();
	world.AddComponent<ecs::NameComponent>(m_SoundEntity, "Hidden Sound");
}

void hidden::SoundSystem::Shutdown(World& world)
{
	world.DestroyEntity(m_SoundEntity);
}

void hidden::SoundSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	for (const ecs::Entity& entity : world.Query<ecs::query::Added<const hidden::RevealComponent>::Exclude<const eng::SavegameComponent>>())
	{
		auto& soundComponent = world.AddComponent<eng::sound::SequenceRequestComponent>(m_SoundEntity);
		soundComponent.m_Asset = strSoundSequence;
	}

	for (const ecs::Entity& entity : world.Query<ecs::query::Include<const eng::sound::SequenceRequestComponent>>())
		world.RemoveComponent<eng::sound::SequenceRequestComponent>(m_SoundEntity);
}