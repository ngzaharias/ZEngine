#include "GameClientPCH.h"
#include "GameClient/HiddenSoundSystem.h"

#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/SoundComponents.h"
#include "GameClient/HiddenObjectComponents.h"

namespace
{
	const str::Guid strSoundSequence = GUID("2bde153c-851a-429c-8895-7b6b0fd482fb");
}

void hidden::SoundSystem::Initialise(World& world)
{
	m_SoundEntity = world.CreateEntity();
}

void hidden::SoundSystem::Shutdown(World& world)
{
	world.DestroyEntity(m_SoundEntity);
}

void hidden::SoundSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	for (const ecs::Entity& cameraEntity : world.Query<ecs::query::Added<const hidden::RevealedComponent>>())
	{
		auto& soundComponent = world.AddComponent<eng::sound::SequenceRequestComponent>(m_SoundEntity);
		soundComponent.m_Handle = strSoundSequence;
	}

	for (const ecs::Entity& entity : world.Query<ecs::query::Include<const eng::sound::SequenceRequestComponent>>())
		world.RemoveComponent<eng::sound::SequenceRequestComponent>(m_SoundEntity);
}