#pragma once

#include "ECS/Singleton.h"

namespace clt::settings
{
	/// \brief Debug settings that is only used in non-gold builds.
	struct DebugSingleton : public ecs::Singleton<DebugSingleton>
	{
		bool m_IsHexmapEnabled = false;
	};
}