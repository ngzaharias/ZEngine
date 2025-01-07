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
		int32 m_Identifier = 0;
		str::String m_Label = {};

		::clt::settings::DebugComponent m_DebugClient;
		::eng::settings::DebugComponent m_DebugEngine;
		::hidden::settings::DebugComponent m_DebugHidden;
		::eng::settings::LocalComponent m_Local;
	};
}