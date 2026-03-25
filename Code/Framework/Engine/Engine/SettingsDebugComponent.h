#pragma once

#include "ECS/Component.h"

namespace eng::settings
{
	/// \brief Debug settings that is only used in non-gold builds.
	struct DebugComponent final : public ecs::StaticComponent
	{
		bool m_AreLinesEnabled = false;
		bool m_ArePhysicsEnabled = false;
		bool m_AreSoftbodyEnabled = false;
	};
}