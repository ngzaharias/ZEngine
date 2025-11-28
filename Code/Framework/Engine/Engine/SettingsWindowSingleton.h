#pragma once

#include "ECS/Singleton.h"
#include "Engine/WindowModeEnum.h"
#include "Math/Vector.h"

namespace eng::settings
{
	/// \brief Shared window settings for all local users that is saved to a config file.
	struct WindowSingleton final : public ecs::Singleton<WindowSingleton>
	{
		int32 m_Monitor = 0;
		int32 m_RefreshRate = 59;
		Vector2u m_Resolution = Vector2u(800, 600);
		eng::EWindowMode m_WindowMode = eng::EWindowMode::Borderless;
	};
}