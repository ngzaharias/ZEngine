#pragma once

#include "ECS/Component.h"

namespace gui::game_menu
{
	struct WindowComponent final : public ecs::Component<WindowComponent> { };
}