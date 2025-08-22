#pragma once

#include "ECS/Component.h"

namespace eng::settings
{
	/// \brief Shared audio settings for all local users that is saved to a config file.
	struct AudioComponent : public ecs::SingletonComponent<AudioComponent>
	{
		float m_EffectVolume = 1.f;
		float m_MasterVolume = 0.5f;
		float m_MusicVolume = 1.f;
	};
}