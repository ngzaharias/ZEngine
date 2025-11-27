#pragma once

#include "Core/Guid.h"
#include "ECS/Singleton.h"

namespace hidden::settings
{
	/// \brief Debug settings that is only used in non-gold builds.
	struct DebugSingleton : public ecs::Singleton<DebugSingleton>
	{
		bool m_IsInputEnabled = false;
		bool m_IsObjectEnabled = false;
	};
}