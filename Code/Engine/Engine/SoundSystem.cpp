#include "EnginePCH.h"
#include "Engine/SoundSystem.h"

#include <Core/Algorithms.h>
#include <Core/Guid.h>
#include <Core/Random.h>

#include <ECS/EntityWorld.h>
#include <ECS/NameComponent.h>
#include <ECS/QueryTypes.h>
#include <ECS/WorldView.h>

#include <SFML/Audio/SoundBuffer.hpp>

#include "Engine/AssetManager.h"
#include "Engine/SoundAsset.h"
#include "Engine/SoundComponent.h"
#include "Engine/TransformComponent.h"

// #todo: load volume from config

void eng::SoundSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	auto& assetManager = world.GetResource<eng::AssetManager>();

	for (const eng::SoundRequest& request : m_Requests)
	{
		if (!request.m_Guid.IsValid())
			continue;

		const auto& soundAsset = *assetManager.LoadAsset<eng::SoundAsset>(request.m_Guid);

		const ecs::Entity entity = world.CreateEntity();

		auto& nameComponent = world.AddComponent<ecs::NameComponent>(entity);
		nameComponent.m_Name = request.m_Guid.ToString().c_str();

		auto& soundComponent = world.AddComponent<eng::SoundComponent>(entity);
		soundComponent.m_Handle = request.m_Guid;
		soundComponent.m_Sound = m_SoundPool.RequestObject();
		soundComponent.m_Sound->setBuffer(soundAsset.m_SoundBuffer);
		soundComponent.m_Sound->setVolume(m_Volume);
		soundComponent.m_Sound->play();
	}
	m_Requests.RemoveAll();

	for (const ecs::Entity& entity : world.Query<ecs::query::Include<eng::SoundComponent>>())
	{
		auto& component = world.GetComponent<eng::SoundComponent>(entity);
		if (component.m_Sound->getStatus() == sf::Sound::Stopped)
		{
			m_SoundPool.ReleaseObject(component.m_Sound);
			world.DestroyEntity(entity);
		}
	}
}

void eng::SoundSystem::PlaySound(const str::Guid& guid)
{
	m_Requests.Append({ guid });
}

