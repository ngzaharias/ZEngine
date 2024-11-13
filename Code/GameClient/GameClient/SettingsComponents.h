#pragma once

#include "ECS/Component.h"

namespace clt::settings
{
	/// \brief Debug settings that is only used in non-gold builds.
	struct DebugComponent : public ecs::SingletonComponent<DebugComponent>
	{
		bool m_IsHexmapEnabled = false;
		bool m_IsHiddenEnabled = false;
	};
}