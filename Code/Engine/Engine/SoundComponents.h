#pragma once

#include <Core/Guid.h>

#include <ECS/Component.h>

#include <SFML/Audio/Sound.hpp>

namespace eng::sound
{
	struct ObjectComponent : public ecs::Component<ObjectComponent>
	{
		// Sounds cannot be moved in memory once they have started otherwise they will stop immediately.
		sf::Sound* m_Sound = nullptr;
	};

	struct RandomComponent : public ecs::Component<RandomComponent>
	{
		str::Guid m_Handle = {};
		int32 m_Index = 0;
	};

	struct RandomBufferComponent : public ecs::SingletonComponent<RandomBufferComponent>
	{
		Array<str::Guid> m_Requests = {};
	};

	struct RandomRequestComponent : public ecs::Component<RandomRequestComponent>
	{
		str::Guid m_Handle = {};
	};

	struct SequenceComponent : public ecs::Component<SequenceComponent>
	{
		str::Guid m_Handle = {};
		int32 m_Index = -1;
	};

	struct SequenceBufferComponent : public ecs::SingletonComponent<SequenceBufferComponent>
	{
		Array<str::Guid> m_Requests = {};
	};

	struct SequenceRequestComponent : public ecs::Component<SequenceRequestComponent>
	{
		str::Guid m_Handle = {};
	};

	struct SingleRequestComponent : public ecs::Component<SingleRequestComponent>
	{
		str::Guid m_Handle = {};
	};
}