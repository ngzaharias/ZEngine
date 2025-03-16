#pragma once

#include "Core/String.h"
#include "ECS/Component.h"
#include "Engine/SettingsComponents.h"
#include "GameClient/SettingsComponents.h"
#include "Hidden/HiddenSettingsComponents.h"

namespace dbg::settings
{
	struct WindowRequestComponent final : public ecs::EventComponent<WindowRequestComponent> { };

	struct WindowComponent final : public ecs::Component<WindowComponent>
	{
		clt::settings::DebugComponent m_Client;
		eng::settings::DebugComponent m_Engine;
		::hidden::settings::DebugComponent m_Hidden;
	};
}