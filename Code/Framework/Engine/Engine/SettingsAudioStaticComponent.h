#pragma once

#include "ECS/Component.h"

namespace eng::settings
{
	/// \brief Shared audio settings for all local users that is saved to a config file.
	struct AudioStaticComponent final : public ecs::StaticComponent
	{
		float m_EffectVolume = 1.f;
		float m_MasterVolume = 1.f;
		float m_MusicVolume = 0.5f;
	};
}