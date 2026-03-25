#pragma once

#include "ClientHidden/HiddenDebugComponent.h"
#include "ECS/Component.h"
#include "Engine/SettingsDebugComponent.h"
#include "GameClient/SettingsDebugComponent.h"

namespace debug::settings
{
	struct WindowComponent final : public ecs::Component
	{
		client::settings::DebugComponent m_Client;
		eng::settings::DebugComponent m_Engine;
		client::hidden::DebugComponent m_Hidden;
	};
}