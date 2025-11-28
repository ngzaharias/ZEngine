#pragma once

#include "Core/Name.h"
#include "ECS/Singleton.h"

namespace eng::settings
{
	/// \brief Launch settings for the game.
	struct LaunchSingleton final : public ecs::Singleton<LaunchSingleton>
	{
		// the level that is loaded when the game first launches
		str::Name m_Level = {};
	};
}