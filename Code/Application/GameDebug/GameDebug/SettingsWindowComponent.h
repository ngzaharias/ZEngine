#pragma once

#include "ClientHidden/HiddenDebugStaticComponent.h"
#include "ECS/Component.h"
#include "Engine/SettingsDebugStaticComponent.h"
#include "GameClient/SettingsDebugStaticComponent.h"

namespace debug::settings
{
	struct WindowComponent final : public ecs::Component
	{
		client::settings::DebugStaticComponent m_Client;
		eng::settings::DebugStaticComponent m_Engine;
		client::hidden::DebugStaticComponent m_Hidden;
	};
}