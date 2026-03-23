#pragma once

#include "ClientHidden/HiddenDebugSingleton.h"
#include "ECS/Component.h"
#include "Engine/SettingsDebugSingleton.h"
#include "GameClient/SettingsDebugSingleton.h"

namespace debug::settings
{
	struct WindowComponent final : public ecs::Component
	{
		client::settings::DebugSingleton m_Client;
		eng::settings::DebugSingleton m_Engine;
		client::hidden::DebugSingleton m_Hidden;
	};
}