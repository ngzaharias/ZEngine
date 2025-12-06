#pragma once

#include "ECS/Component.h"

namespace gui::settings_menu
{
	struct WindowComponent final : public ecs::Component<WindowComponent> { };
}