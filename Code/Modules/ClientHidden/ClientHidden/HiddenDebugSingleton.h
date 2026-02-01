#pragma once

#include "Core/Guid.h"
#include "ECS/Singleton.h"

namespace client::hidden
{
	/// \brief Debug settings that is only used in non-gold builds.
	struct DebugSingleton final : public ecs::Singleton<DebugSingleton>
	{
		bool m_IsInputEnabled = false;
		bool m_IsObjectEnabled = false;
	};
}