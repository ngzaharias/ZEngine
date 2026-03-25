#pragma once

#include "Core/Guid.h"
#include "ECS/Component.h"

namespace client::hidden
{
	/// \brief Debug settings that is only used in non-gold builds.
	struct DebugComponent final : public ecs::StaticComponent
	{
		bool m_IsInputEnabled = false;
		bool m_IsObjectEnabled = false;
	};
}