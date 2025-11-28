#pragma once

#include "ECS/Singleton.h"

namespace eng::settings
{
	/// \brief Debug settings that is only used in non-gold builds.
	struct DebugSingleton final : public ecs::Singleton<DebugSingleton>
	{
		bool m_AreLinesEnabled = false;
		bool m_ArePhysicsEnabled = false;
		bool m_AreSoftbodyEnabled = false;
		bool m_IsEditorModeEnabled = false;
	};
}