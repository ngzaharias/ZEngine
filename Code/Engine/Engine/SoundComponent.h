#pragma once

#include <Core/Guid.h>

#include <ECS/Component.h>

namespace sf
{
	class Sound;
}

namespace eng
{
	struct SoundComponent : public ecs::Component<SoundComponent>
	{
		str::Guid m_Handle;
		sf::Sound* m_Sound;
	};
}