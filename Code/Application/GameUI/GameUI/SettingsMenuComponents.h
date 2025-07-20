#pragma once

#include "ECS/Component.h"

namespace gui::settings_menu
{
	struct WindowComponent final : public ecs::Component<WindowComponent> { };

	struct CloseRequest final : public ecs::EventComponent<CloseRequest> { };
	struct OpenRequest final : public ecs::EventComponent<OpenRequest> { };
}