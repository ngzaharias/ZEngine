#pragma once

#include "ECS/Component.h"

namespace client::settings
{
	/// \brief Debug settings that is only used in non-gold builds.
	struct DebugComponent final : public ecs::StaticComponent
	{
		bool m_IsHexmapEnabled = false;
	};
}