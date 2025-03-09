#pragma once

#include "Core/String.h"
#include "ECS/Component.h"
#include "Engine/SettingsComponents.h"
#include "GameClient/SettingsComponents.h"
#include "Hidden/HiddenSettingsComponents.h"

namespace gui::settings
{
	struct CloseRequestComponent final : public ecs::EventComponent<CloseRequestComponent> { };
	struct OpenRequestComponent final : public ecs::EventComponent<OpenRequestComponent> { };

	struct WindowComponent final : public ecs::Component<WindowComponent>
	{
		::clt::settings::DebugComponent m_DebugClient;
		::eng::settings::DebugComponent m_DebugEngine;
		::hidden::settings::DebugComponent m_DebugHidden;
		::eng::settings::LocalComponent m_Local;
		::eng::settings::GraphicsComponent m_Graphics;
	};
}