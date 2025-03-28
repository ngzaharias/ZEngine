#pragma once

#include "ECS/Component.h"

namespace gui::game_menu
{
	struct OpenRequestComponent final : public ecs::EventComponent<OpenRequestComponent> { };

	struct WindowComponent final : public ecs::Component<WindowComponent> { };
}