#pragma once

#include "Core/Guid.h"
#include "ECS/Component.h"
#include "ECS/Singleton.h"

#include <SFML/Audio/Sound.hpp>

namespace eng::sound
{
	struct ObjectComponent : public ecs::Component<ObjectComponent>
	{
		// Sounds cannot be moved/deleted once they have started otherwise they will stop immediately.
		sf::Sound* m_Sound = nullptr;
		str::Guid m_Asset = {};
	};

	struct RandomComponent : public ecs::Component<RandomComponent>
	{
		str::Guid m_Asset = {};
		int32 m_Index = 0;
	};

	struct RandomBufferSingleton : public ecs::Singleton<RandomBufferSingleton>
	{
		Array<str::Guid> m_Requests = {};
	};

	struct RandomRequestComponent : public ecs::Component<RandomRequestComponent>
	{
		str::Guid m_Asset = {};
	};

	struct SequenceComponent : public ecs::Component<SequenceComponent>
	{
		str::Guid m_Asset = {};
		int32 m_Index = -1;
	};

	struct SequenceBufferSingleton : public ecs::Singleton<SequenceBufferSingleton>
	{
		Array<str::Guid> m_Requests = {};
	};

	struct SequenceRequestComponent : public ecs::Component<SequenceRequestComponent>
	{
		str::Guid m_Asset = {};
	};

	struct SingleRequestComponent : public ecs::Component<SingleRequestComponent>
	{
		str::Guid m_Asset = {};
	};
}