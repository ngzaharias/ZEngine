#pragma once

#include "ECS/Component.h"

namespace hidden::settings
{
	/// \brief Debug settings that is only used in non-gold builds.
	struct DebugComponent : public ecs::SingletonComponent<DebugComponent>
	{
		bool m_IsInputEnabled = false;
		bool m_IsObjectEnabled = false;
	};
}