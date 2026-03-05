#pragma once

#include "ECS/Singleton.h"

namespace client::settings
{
	/// \brief Debug settings that is only used in non-gold builds.
	struct DebugSingleton final : public ecs::Singleton<DebugSingleton>
	{
		bool m_IsHexmapEnabled = false;
	};
}