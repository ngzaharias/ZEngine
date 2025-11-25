#pragma once

#include "Core/String.h"
#include "ECS/Component.h"
#include "Engine/SettingsComponents.h"
#include "GameClient/SettingsComponents.h"
#include "Hidden/HiddenDebugSettingsComponent.h"

namespace dbg::settings
{
	struct WindowRequest final : public ecs::Event<WindowRequest> { };

	struct WindowComponent final : public ecs::Component<WindowComponent>
	{
		clt::settings::DebugComponent m_Client;
		eng::settings::DebugComponent m_Engine;
		::hidden::settings::DebugComponent m_Hidden;
	};
}