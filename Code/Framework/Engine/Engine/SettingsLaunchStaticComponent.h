#pragma once

#include "Core/Name.h"
#include "ECS/Component.h"

namespace eng::settings
{
	/// \brief Launch settings for the game.
	struct LaunchStaticComponent final : public ecs::StaticComponent
	{
		// the level that is loaded when the game first launches
		str::Name m_Level = {};
	};
}