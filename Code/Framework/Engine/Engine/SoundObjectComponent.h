#pragma once

#include "Core/Guid.h"
#include "ECS/Component.h"

#include <SFML/Audio/Sound.hpp>

namespace eng::sound
{
	struct ObjectComponent final : public ecs::Component<ObjectComponent>
	{
		// Sounds cannot be moved/deleted once they have started otherwise they will stop immediately.
		sf::Sound* m_Sound = nullptr;
		str::Guid m_Asset = {};
	};
}