#pragma once

#include "Core/String.h"
#include "ECS/Component.h"
#include "Engine/SettingsDebugSingleton.h"
#include "GameClient/SettingsDebugComponent.h"
#include "Hidden/HiddenDebugSettingsSingleton.h"

namespace dbg::settings
{
	struct WindowRequest final : public ecs::Event<WindowRequest> { };

	struct WindowComponent final : public ecs::Component<WindowComponent>
	{
		clt::settings::DebugSingleton m_Client;
		eng::settings::DebugSingleton m_Engine;
		::hidden::settings::DebugSingleton m_Hidden;
	};
}