#pragma once

#include "ECS/Component.h"

#include <SFML/Audio/Music.hpp>

namespace eng
{
	/// \brief Holds the currently active music being played.
	struct MusicStaticComponent final : public ecs::StaticComponent
	{
		// Music cannot be moved in memory once it has started otherwise it will stop immediately.
		sf::Music* m_Music = nullptr;
	};
}